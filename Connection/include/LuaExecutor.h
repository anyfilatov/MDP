#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "Logger.h"
#include "DataBase.h"
#include "OrGraph.h"
#include "Iterator.h"
#include "RbTree.h"
#include "selene.h"
#include "errors.h"
#include "host.h"
#include <memory>
typedef std::vector<int> UserDbTableIdArray;
typedef lua_State* Lua;
typedef std::map<int, std::vector<std::string>> MapDb;
typedef std::map<int, std::map<std::string, std::list<std::string>>> ReduceDb;
typedef std::vector<std::vector<QString>> KeyValuePairsArray;

static KeyValuePairsArray getKeyValuePairs(Lua l);
class LuaExecutor;

static int doMap(Lua l);
static int doReduce(Lua l);
static int doMapOnly(LuaExecutor* executor, util::Id& id);
static int doReduceOnly(LuaExecutor* executor, util::Id& id);

static const char* USER_DB_TABLE_ID_LUA_GLOBAL_VARIABLE_NAME = "in";
class LuaExecutor {
public:
    class LuaContextVariable;
    typedef std::shared_ptr<LuaContextVariable> LuaContextVariablePtr;
private:
    typedef std::vector<LuaContextVariablePtr> CVariablesInLuaContainer;
    
public:
    struct LuaContextVariable{
        
        static LuaContextVariable* getFromPeak(Lua l) {
            int top = lua_gettop(l);
            void* id = lua_touserdata(l, top);
            LOG_DEBUG("userdata");
            return static_cast<LuaContextVariable*>(id);
        }
        
        void pushGlobalByName(std::string& s) {
            auto* l = executor_->getLua();
            LOG_DEBUG("push global " << s);
            lua_pushlightuserdata(l, static_cast<void*>(this));
            lua_setglobal(l, s.c_str());
        }
        LuaExecutor* e() { return executor_;};
        std::shared_ptr<util::Id> id() { return id_;}
        static LuaContextVariablePtr create(LuaExecutor* executor, util::Id& id);

        void pushOnStack() {
            lua_pushlightuserdata(e()->getLua(), static_cast<void*> (this));
        }
        ~LuaContextVariable(){}
    private:
        LuaContextVariable(LuaExecutor* executor, util::Id& id) 
            : executor_(executor), id_(new util::Id(id))
        {}
        
        LuaExecutor* executor_;
        std::shared_ptr<util::Id> id_;
                
    };
    
    LuaExecutor(DB db, RB rb, OG og, QString code, int type, bool file = false) 
            : db_(db), rb_(rb), og_(og), code_(code), lua_(true), executionType_(type) {
        LOG_TRACE("LuaExecutor()");
        compile(file);
    }
    LuaExecutor(const LuaExecutor& orig) = delete;
    void operator=(const LuaExecutor&) = delete;
    virtual ~LuaExecutor() {
        LOG_TRACE("~LuaExecutor()");
    };
    
    int init(UserDbTableIdArray& udata) {
        if(error_ != Errors::STATUS_OK) {
            return error_;
        }
        
        for(auto id : udata) {
            std::stringstream varName;
            varName << USER_DB_TABLE_ID_LUA_GLOBAL_VARIABLE_NAME << id; 
            auto str = varName.str();
            lua_[str.c_str()] = id;
        }
    }
    
    void setId(const util::Id& id) {
        id_ = id;
    }
    
    void setFuncName(const QString& funcName) {
        func_ = funcName;
    }
    
    int execute() {
        switch(executionType_) {
            case util::CMD_MAP:
                executeMap();
                break;
            case util::CMD_REDUCE:
                executeReduce();
                break;
            case util::CMD_START_USER_SCRIPT:
                executeUserScript();
                break;
            default:
                throw UnknownParametersException(util::concat("unknown execution type:", executionType_));
                
        }
        return 0;
    }
    
    DB::WrappedType::GetAtomType getNextFromDb() {
        DB::ScopedLock lock(db_);
        return db_->getNextAtom(id_);
    }
    
    RB::WrappedType::GetAtomType getNextFromRb() {
        RB::WrappedType::GetAtomType out;
        RB::ScopedLock lock(rb_);
        if(it != keys_.end()) {
            util::Id tId(id_);
            auto i = tId.get(util::Id::dataIdIndex);
            tId.set(util::Id::dataIdIndex, i-1);
            out = rb_->getNextAtom(tId, *it);
            it++;
        }
        return out;
    }
    
    int setSwapToRb(RB::WrappedType::SetAtomType& atom) {
        RB::ScopedLock lock(rb_);
        return rb_->setSwap(id_, atom);
    }
    
    void flush() {
        RB::ScopedLock lock(rb_);
        return rb_->flush();
    }

    int setSwapToDb(DB::WrappedType::SetAtomType& atom) {
        RB::ScopedLock lock(rb_);
        db_->setSwap(id_, atom);
        return Errors::STATUS_OK;
    }
    
    void flushDb(){
        db_->flush(id_);
    }

    friend std::ostream& operator << (std::ostream& os, const LuaExecutor& ) {
        os << "not implemented";
        return os;
    }
    
    Lua getLua() {
        return lua_.getLuaState();
    }
    
    void endMap() {

        LOG_DEBUG("endMap");
    }
    
    void endReduce() {
        LOG_DEBUG("end Reduce");
    }
    
    void startMap() {
        //++std::get<0>(id_);
    }
    
    void splitKeys() {
        int size = 0;
        Iterator<Host> it(*og_);
        while(!it.end()){
            ++size;
            it.next();
        }
        LOG_INFO("Splitted size="<<size);
        if(size == 0){
            return;
        }
        splittedKeys_.resize(size);
        auto sizeList = keys_.size()/size;
        for(int i = 0; i < size; i++){
            auto it = keys_.begin();
            for( int j = 0; j < sizeList && it != keys_.end(); j++, it++){
                splittedKeys_[i].append(*it);
            }
        }
        keys_.clear();
        splittedKeys_[0].swap(keys_);
    }

    void startReduce() {
        {
            RB::ScopedLock lock(rb_);
            LOG_DEBUG("start Reduce get all keys");
            keys_ = rb_->getAllKeys(id_);
            for(auto& key : keys_) {
                LOG_DEBUG("key=" << key.toStdString());
            }
            splitKeys();
            it = keys_.begin();
        }
        int id = id_.get(util::Id::dataIdIndex);
        ++id;
        id_.set(util::Id::dataIdIndex, id);
    }
    
    void addLuaVariable(LuaContextVariablePtr variable) {
        luaVariables.push_back(variable);
    }
    
    void sendTasksToChilds() {
        //og_->getChildren()
    }
    
    std::string getFuncName() {return func_.toStdString();}
    
    template<typename T> void forEach(T action){
        OG::ScopedLock lock(og_);
        Iterator<Host> it(*og_);
        for(; !it.end(); it.next()) {
            action(*it);
        }
        
    }
    
    void setKeys(QStringList& keys ) {
        keys_.swap(keys);
        it = keys_.begin();
    }

    QStringList getKeys(int i) {
        return splittedKeys_[i];
    }

    template<typename T> void forEachWithoutRoot(T action){
        OG::ScopedLock lock(og_);
        Iterator<Host> it(*og_);
        if(it.end()) return;
        it.next();
        int i = 1;
        for(; !it.end(); it.next()) {
            action(*it, i);
            ++i;
        }
        
    }
        
    QString getCode() {return code_;}
private:
    int executeUserScript() {
        using util::Id;
        auto* l = lua_.getLuaState();
        auto ids = db_->getAllTablesId(id_);
        if(ids.empty()){
            throw UnknownParametersException("No data in database");
        }
        for(auto& tableId : ids) {
            id_.i0 = 1;
            id_.i1 = 1;
            id_.i2 = 0;
//            id_.set(Id::dataIdIndex, tableId);
            auto var = LuaContextVariable::create(this, id_ );
            std::stringstream ss;
            ss << USER_DB_TABLE_ID_LUA_GLOBAL_VARIABLE_NAME << tableId;
            auto str = ss.str();
            var->pushGlobalByName(str);
        }
        LOG_DEBUG("before map");
        lua_pushcfunction(l, &doMap);
        lua_setglobal(l, "doMap");
        lua_pushcfunction(l, &doReduce);
        lua_setglobal(l, "doReduce");
        std::string s = func_.toStdString();
        LOG_DEBUG("func=" << s);
        lua_getglobal(l, s.c_str());
        lua_call(l, 0, 0);
        LOG_TRACE("execute end");
        return Errors::STATUS_OK;
    }
    
    int executeMap() {
        return doMapOnly(this, id_);
    }
    
    int executeReduce() {
        return doReduceOnly(this, id_);
    }

    
    bool compile(bool file = false) {
        if(!file) {
            if(!lua_.LoadString(code_.toStdString())){
                error_ = Errors::STATUS_COMPILATION_ERROR;
                throw LuaExecutionExeption(Errors::STATUS_COMPILATION_ERROR);
            }
        } else {
            LOG_DEBUG("load from file:" << code_.toStdString());
            if(!lua_.Load(code_.toStdString())){
                std::string err = util::concat("load from file error:" , code_.toStdString());
                error_ = Errors::STATUS_COMPILATION_ERROR;
                throw LuaExecutionExeption(err);
            }
        }
        return error_ == Errors::STATUS_OK;
    }
private:
    DB db_;
    RB rb_;
    OG og_;
    QString code_;
    sel::State lua_;
    sel::CapturedStdout captureOut_;
    int error_ = Errors::STATUS_OK;
    util::Id id_;
    QString func_;
    int executionType_;
    CVariablesInLuaContainer luaVariables;
    QStringList keys_;
    QStringList::iterator it = keys_.begin();
    std::vector<QStringList> splittedKeys_;
};

inline LuaExecutor::LuaContextVariablePtr LuaExecutor::LuaContextVariable::create(LuaExecutor* executor, util::Id& id) {
    LuaExecutor::LuaContextVariablePtr var = std::shared_ptr<LuaExecutor::LuaContextVariable>(new LuaExecutor::LuaContextVariable(executor, id));
    executor->addLuaVariable(var);
    return var;
}


static KeyValuePairsArray getKeyValuePairs(lua_State *l) {
    KeyValuePairsArray ret;
    lua_pushnil(l); /* first key */
    int i = 0;
    while (lua_next(l, -2) != 0) {
        LOG_DEBUG( lua_typename(l, lua_type(l, -2)) << " "
                << lua_typename(l, lua_type(l, -1)));
        //Array [0] - value
        //         [-1] - key
        //         .....
        //         [-2n] - value
        //         [-(2n+1)] - key
        
        // index -2 - there's index
        // index -1 - is a key, or value
        ret.push_back(KeyValuePairsArray::value_type(2));
        auto& vec = ret.back();
        if (lua_isstring(l, -1)) {
            size_t sk = 0;
            size_t sv = 0;
            auto* key = lua_tolstring(l, -2, &sk);
            auto* val = lua_tolstring(l, -1, &sv);
            LOG_DEBUG("key from lua:" << key << " s=" << sk);
            LOG_DEBUG("value from lua:" << val << " s=" << sv);
            vec[0] = QString(key);
            vec[1] = QString(val);
//            if(i%2 == 0) {//that mean key it's STACK!!!!!!!

//            } else { // that mean value
//                vec[1] = QString(val);
//            }
        } else {
            std::string err = util::concat( "User function return variable by type ", lua_typename(l, lua_type(l, -1)), ". expected: string");
            LOG_WARNING(err);
            throw LuaExecutionExeption(err, Errors::STATUS_UNEXPECTED_USER_VALUE);
        }
        i++;
        lua_pop(l, 1);
    }
    return ret;
}

using ActionFunction = std::function<void(KeyValuePairsArray::value_type&)> ;

template<typename T> 
static void CallLuaFunction(LuaExecutor::LuaContextVariable* context, std::pair<std::string, T>& ar, ActionFunction resultAction) {
    
    for( auto& val : ar.second) {
        CallLuaFunction(context, val, resultAction);
    }
}

static void l_pushtablestring(Lua l , int key , const char* value) {
    lua_pushinteger(l, key);
    lua_pushstring(l, value);
    lua_settable(l, -3);
}

//static void createTableFromDb(Lua l, DB::WrappedType::GetAtomType& content) {
static void createTableFromDb(Lua l, std::vector<QString>& content) {
    lua_newtable(l);
    int i = 0;
    for(auto& c : content) {
        LOG_TRACE("value=" << c.toStdString() );
        auto str = c.toStdString();
        l_pushtablestring(l, i, str.c_str());
        i++;
    }
}

static void createTable(Lua l, RB::WrappedType::GetAtomType& content) {
    lua_newtable(l);
    int i = 0;
    for(auto& c : content.second) {
        LOG_DEBUG("string in create:" << c.toStdString());
        if(c.isEmpty()){
            continue;
        }
        auto str = c.toStdString();
        LOG_TRACE("value=" << str );
        l_pushtablestring(l, i, str.c_str());
        i++;
    }
}


static void doMapForAtoms(LuaExecutor::LuaContextVariable* context, const char* funcName) {
    while(1){
        auto dbRow = context->e()->getNextFromDb();
        if( !dbRow || dbRow->size() == 0 ) {
            LOG_DEBUG("Null!!!");
            break;
        }
        LOG_TRACE("dbRow");
        auto* executor = context->e();
        auto* l = executor->getLua();
        auto cells = dbRow->getCells();
        for(auto& c : cells) {
            lua_getglobal(l, funcName);
            createTableFromDb(l, c);
            LOG_TRACE("call");
  //        lua_call(l, 1, 1);
            lua_call(l, 1, 1);

            LOG_DEBUG("after call" );
            if( !lua_istable(l, lua_gettop(l)) ) {
                std::string err = util::concat("user function ", " return ", lua_typename(l, lua_type(l, lua_gettop(l))), ". expected: table");
                LOG_WARNING(err);
                throw LuaExecutionExeption(err, Errors::STATUS_UNEXPECTED_USER_VALUE);
            }
            auto res = getKeyValuePairs(l);
            for(auto& r : res) {
                LOG_DEBUG("Set Key:" << r[0].toStdString());
                context->e()->setSwapToRb(r);
            }
        }

        context->e()->flush();
    }
}

static void doReduceForAtoms(LuaExecutor::LuaContextVariable* context,  const char* funcName) {
    auto* executor = context->e();
    auto* l = executor->getLua();
    auto setValue = DB::WrappedType::SetAtomType(new DB::WrappedType::SetAtomType::element_type());
    std::vector<QString> headers = {"key", "value"};
    setValue->setHeaders(headers);
    while(1){
        LOG_DEBUG("get next from rb");
        auto atom = executor->getNextFromRb();
        if( atom.second.empty() ) {
            LOG_DEBUG("rb return empty value");
            break;
        }
        LOG_DEBUG("get global");
        lua_getglobal(l, funcName);

        auto str = atom.first.toStdString();
        LOG_DEBUG("str=" + str);
        lua_pushstring(l, str.c_str());
        LOG_DEBUG("create table");
        createTable(l, atom);
        LOG_TRACE("call");
        lua_call(l, 2, 2);

        LOG_DEBUG("after call" );
        if( !lua_isstring(l, lua_gettop(l)) ) {
            std::string err = util::concat("user function ", " return ", lua_typename(l, lua_type(l, lua_gettop(l))), ". expected: table");
            LOG_WARNING(err);
            throw LuaExecutionExeption(err, Errors::STATUS_UNEXPECTED_USER_VALUE);
        }
        const char* value = lua_tostring(l, lua_gettop(l));
        LOG_DEBUG("value " << value);
        lua_pop(l, 1);
        const char* key = lua_tostring(l, lua_gettop(l));
        lua_pop(l, 1);
        vector<vector<QString> > cells(1);
        cells[0].push_back(QString(key));
        cells[0].push_back(QString(value));
        setValue->setCells(cells);
        executor->setSwapToDb(setValue);
    }
    executor->flushDb();
}

static int doMapOnly(LuaExecutor* executor, util::Id& id) {
    auto* l = executor->getLua();
    auto func = executor->getFuncName();
    auto* funcName = func.c_str();
    LOG_DEBUG("funcName=" << funcName << " id=" << *executor);
    lua_getglobal(l, funcName);
    try{
        LuaExecutor::LuaContextVariablePtr luaVar 
                = LuaExecutor::LuaContextVariable::create(executor, id);
        LOG_TRACE("doMapForAtoms");
        doMapForAtoms(luaVar.get(), funcName);
    } catch(const LuaExecutionExeption& e) {
        std::string err = util::concat(e.what(), " in function ", funcName);
        throw LuaExecutionExeption(err);
    }
    return Errors::STATUS_OK;
}

static int doReduceOnly(LuaExecutor* executor, util::Id& id) {
    auto* l = executor->getLua();
    auto func = executor->getFuncName();
    auto* funcName = func.c_str();
    LOG_DEBUG("funcName=" << funcName << " id=" << *executor);
    lua_getglobal(l, funcName);
    try{
        LuaExecutor::LuaContextVariablePtr luaVar 
                = LuaExecutor::LuaContextVariable::create(executor, id);
        LOG_TRACE("doMapForAtoms");
        doReduceForAtoms(luaVar.get(), funcName);
    } catch(const LuaExecutionExeption& e) {
        std::string err = util::concat(e.what(), " in function ", funcName);
        throw LuaExecutionExeption(err);
    }
    return Errors::STATUS_OK;
}
typedef std::shared_ptr<QTcpSocket> SocketPtr;

static SocketPtr sendToHost(QString ip, int port, QByteArray& buffer) {
    LOG_DEBUG("SendTo " << ip.toStdString() << ":" << port);
    SocketPtr socketOut = std::make_shared<QTcpSocket>(new QTcpSocket());
    socketOut->connectToHost(ip, port);
    LOG_DEBUG("before wait")
    socketOut->waitForConnected(1000);
    LOG_DEBUG("after wait")
    if(socketOut->state() == QAbstractSocket::ConnectedState) {
        if (socketOut->isWritable()) {
            socketOut->write(buffer);
            socketOut->flush();
            socketOut->waitForBytesWritten();
        } else {
            throw NetworkErrorException(util::concat("send to host (", ip.toStdString(), " ", port, ") error"));
        }
        
    } else {
        LOG_DEBUG("not connected");
        socketOut.reset();
    }
    return socketOut;
}

class Waiter{
    struct WQSocket{
        int ready = 1;
        int failed = -1;
        int inWait = 0;
        WQSocket(SocketPtr _socket) : socket(_socket), status(inWait) {}
        SocketPtr socket;
        int status;
    };
    
    typedef std::vector<WQSocket> Conteiner;
public:
    ~Waiter(){
        closeAll();
    }
    
    void closeAll() {
        for(auto& s : sockets_){
            s.socket->disconnectFromHost();
            s.socket->close();
            s.socket->deleteLater();
        }
    }
    
    void clear() {
        closeAll();
        sockets_.clear();
    }
    
    typedef std::vector<SocketPtr> SocketContainer;
    SocketContainer getSockets() {
        SocketContainer out;
        out.reserve(sockets_.size());
        for(auto& s : sockets_){
            out.push_back(s.socket);
        }
        return out;
    }
    void addToWait(SocketPtr socket) {
        if(socket){
            sockets_.push_back(WQSocket(socket));
        }
    }
    
    bool empty() {
        return sockets_.empty();
    }
    
    bool hasFailed() {
        for(auto& s : sockets_){
            if(s.status == s.failed) {
                return true;
            }
        }
        return false;
    }
    
    void wait() {
        auto count = sockets_.size();
        while(1) {
            LOG_DEBUG("wait:" << count);
            for(auto& s : sockets_){
                if( s.status == s.inWait ) {
                    if(s.socket->state() == QAbstractSocket::SocketState::UnconnectedState 
                            || s.socket->state() == QAbstractSocket::SocketState::ClosingState){
                        s.status = s.failed;
                        --count;
                        continue;
                    }
                    if(s.socket->waitForReadyRead(100)){
                        s.status = s.ready;
                        --count;
                    }
                }
            }
            if(count > 0) {
                for(int i = 0; i < 100000; i++){}//sleep
            } else {
                break;
            }
        }
    }
private:
    Conteiner sockets_;
};

template<typename T>
static void executeFunction(T action, LuaExecutor::LuaContextVariable* context, const char* funcName, int type) {
    auto* executor = context->e();
    auto* l = executor->getLua();
    Waiter waiter;
    int i = 3;
    do {
        if (!waiter.empty()) {
            waiter.clear();
        }
        executor->forEachWithoutRoot([&] (Host & host, int i) -> void {
            int cmd = type;
            QString code = executor->getCode();
            auto id = *context->id();
            QString qFuncName(funcName);
            QByteArray ar;
            if( type == util::CMD_REDUCE){
               auto keys = executor->getKeys(i);
               LOG_INFO( i << " list keys.size=" << keys.size());
               ar = util::pack(cmd, code, id, qFuncName, keys);
            } else {
                ar = util::pack(cmd, code, id, qFuncName);
            }

            
            LOG_DEBUG("ip=" << host.getIP().toStdString() << " port=" << host.getPort() << " ss=" << ar.size());
            auto socket = sendToHost(host.getIP(), host.getPort(), ar);
            waiter.addToWait(socket);
        });
//        lua_getglobal(l, funcName);

        try {
            LOG_TRACE("start action");
            action(context);
        } catch (const LuaExecutionExeption& e) {
            std::string err = util::concat(e.what(), " in function ", funcName);
            throw LuaExecutionExeption(err);
        }
        waiter.wait();
        --i;
    } while (waiter.hasFailed() && i > 0);
}

static int doMap(Lua l) {
    int argc = lua_gettop(l);
    LOG_DEBUG("argc=" << argc);
    size_t s = 0;
    auto* context = LuaExecutor::LuaContextVariable::getFromPeak(l);
    auto* executor = context->e();
    executor->startMap();
    lua_pop(l, 1);
    const char* funcName = lua_tolstring(l, lua_gettop(l), &s);
    LOG_DEBUG("funcName=" << funcName << " id=" << *executor);
    lua_pop(l, 1);
    
    executeFunction([&](LuaExecutor::LuaContextVariable* context) -> void {doMapForAtoms(context, funcName);}, context, funcName, util::CMD_MAP);
    executor->endMap();
    context->pushOnStack();
    return 1;//count of returns value
}

static int doReduce(Lua l) {
    int argc = lua_gettop(l);
    LOG_DEBUG("argc=" << argc);
    size_t s = 0;
    auto* context = LuaExecutor::LuaContextVariable::getFromPeak(l);
    auto* executor = context->e();
    executor->startReduce();
    lua_pop(l, 1);
    const char* funcName = lua_tolstring(l, lua_gettop(l), &s);
    LOG_DEBUG("funcName=" << funcName << " id=" << *executor);
    //lua_pop(l, 1);
       
    executeFunction([&](LuaExecutor::LuaContextVariable* context) -> void {doReduceForAtoms(context, funcName);}, context, funcName, util::CMD_REDUCE);
    
    LOG_TRACE("");
    executor->endReduce();
    context->pushOnStack();
    return 1;//count of returns value
}
