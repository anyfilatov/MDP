#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include "Logger.h"
#include "DataBase.h"
#include "RbTree.h"
#include "wrapper.h"
#include "selene.h"
#include "Server.h"
#include "errors.h"
#include <memory>

typedef std::vector<int> UserDbTableIdArray;

typedef lua_State* Lua;
typedef std::map<int, std::vector<std::string>> MapDb;
typedef std::map<int, std::map<std::string, std::list<std::string>>> ReduceDb;
typedef std::vector<std::pair<std::string, std::string>> KeyValuePairsArray;

KeyValuePairsArray getKeyValuePairs(Lua l);
int doMap(Lua l);
int doReduce(Lua l);

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
    
    LuaExecutor(DB db, RB rb, QString code, bool file = false) : db_(db), rb_(rb), code_(code), lua_(true) {
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
    
    int execute() {
        using util::Id;
        auto* l = lua_.getLuaState();
        auto ids = db_->getAllTablesId(id_);
        for(auto& tableId : ids) {
            Id id(id_);
            id.set(Id::dataIdIndex, tableId);
            auto var = LuaContextVariable::create(this, id );
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
        lua_getglobal(l, "main");
        lua_call(l, 0, 0);
        LOG_TRACE("execute end");
        return 0;
    }
    
    DB::WrappedType::GetAtomType getNextFromDb() {
        DB::ScopedLock lock(db_);
        return db_->getNextAtom(id_);
    }
    
    RB::WrappedType::GetAtomType getNextFromRb() {
        RB::ScopedLock lock(rb_);
        return rb_->getNextAtom(id_);
    }
    
    int setSwapToRb(RB::WrappedType::SetAtomType& atom) {
        RB::ScopedLock lock(rb_);
        return rb_->setSwap(id_, atom);
    }
    
    int setToRb(RB::WrappedType::SetAtomType& atom) {
        RB::ScopedLock lock(rb_);
        return rb_->set(id_, atom);
    }
    
    int setSwapToDb(DB::WrappedType::SetAtomType& atom) {
        RB::ScopedLock lock(rb_);
        return rb_->setSwap(id_, atom);
    }
    
    int setToDb(const DB::WrappedType::SetAtomType& atom) {
        RB::ScopedLock lock(rb_);
        return rb_->set(id_, atom);
    }
    
    friend std::ostream& operator << (std::ostream& os, const LuaExecutor& ) {
        os << "not implemented";
        return os;
    }
    
    Lua getLua() {
        return lua_.getLuaState();
    }
    
    void endMap() {
        //++std::get<0>(id_);
        LOG_DEBUG("endMap");
    }
    
    void endReduce() {
        LOG_DEBUG("end Reduce");
        ++id_.i0;
    }
    
    void addLuaVariable(LuaContextVariablePtr variable) {
        luaVariables.push_back(variable);
    }
    
private:
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
    QString code_;
    sel::State lua_;
    sel::CapturedStdout captureOut_;
    int error_ = Errors::STATUS_OK;
    util::Id id_;
    CVariablesInLuaContainer luaVariables;
};

LuaExecutor::LuaContextVariablePtr LuaExecutor::LuaContextVariable::create(LuaExecutor* executor, util::Id& id) {
    LuaExecutor::LuaContextVariablePtr var = std::shared_ptr<LuaExecutor::LuaContextVariable>(new LuaExecutor::LuaContextVariable(executor, id));
    executor->addLuaVariable(var);
    return var;
}


KeyValuePairsArray getKeyValuePairs(lua_State *l) {
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
        if (lua_isstring(l, -1)) {
            size_t s = 0;
            auto* val = lua_tolstring(l, -1, &s);
            
            if(i%2 == 0) {//that mean key it's STACK!!!!!!!
                ret.emplace_back(std::string(), std::string(val, s));
            } else { // that mean value
                ret.back().first = std::move(std::string(val, s));
            }
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
void CallLuaFunction(LuaExecutor::LuaContextVariable* context, std::pair<std::string, T>& ar, ActionFunction resultAction) {
    
    for( auto& val : ar.second) {
        CallLuaFunction(context, val, resultAction);
    }
}

template<>
void CallLuaFunction<std::string>(LuaExecutor::LuaContextVariable* context, std::pair<std::string, std::string>& param, ActionFunction resultAction) {
    auto* executor = context->e();
    auto* l = executor->getLua();
    lua_pushstring(l, param.first.c_str());
    lua_call(l, 1, 1);
    
    LOG_DEBUG("after call" );
    if( !lua_istable(l, lua_gettop(l)) ) {
        std::string err = util::concat("user function ", " return ", lua_typename(l, lua_type(l, lua_gettop(l))), ". expected: table");
        LOG_WARNING(err);
        throw LuaExecutionExeption(err, Errors::STATUS_UNEXPECTED_USER_VALUE);
    }
    auto res = getKeyValuePairs(l);
    for(auto r : res) {
        resultAction(r);
    }
}


void doMapForAtoms(LuaExecutor::LuaContextVariable* context) {
    auto dbRow = context->e()->getNextFromDb();
    LOG_TRACE("dbRow");
    auto* executor = context->e();
    auto* l = executor->getLua();
    LOG_TRACE("lua_pushstring");
    lua_pushstring(l, dbRow.c_str());
    LOG_TRACE("call");
    lua_call(l, 1, 1);
    
    LOG_DEBUG("after call" );
    if( !lua_istable(l, lua_gettop(l)) ) {
        std::string err = util::concat("user function ", " return ", lua_typename(l, lua_type(l, lua_gettop(l))), ". expected: table");
        LOG_WARNING(err);
        throw LuaExecutionExeption(err, Errors::STATUS_UNEXPECTED_USER_VALUE);
    }
    auto res = getKeyValuePairs(l);
    for(auto& r : res) {
        context->e()->setSwapToRb(r);
    }
}

static void l_pushtablestring(Lua l , int key , const char* value) {
    lua_pushinteger(l, key);
    lua_pushstring(l, value);
    lua_settable(l, -3);
}

void createTable(Lua l, RB::WrappedType::GetAtomType& content) {
    lua_newtable(l);
    int i = 0;
    for(auto& c : content.second) {
        LOG_TRACE("value=" << c );
        l_pushtablestring(l, i, c.c_str());
        i++;
    }
}

void doReduceForAtoms(LuaExecutor::LuaContextVariable* context, const char* funcName) {
    auto* executor = context->e();
    auto atom = executor->getNextFromRb();
    auto* l = executor->getLua();
    lua_pushstring(l, atom.first.c_str());
    createTable(l, atom);
    LOG_TRACE("get global " << funcName);
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
    LOG_DEBUG("key " << key);
    lua_pop(l, 1);
    executor->setToDb(std::make_pair(std::string(key), std::string(value)));
}

int doMap(Lua l) {
    int argc = lua_gettop(l);
    LOG_DEBUG("argc=" << argc);
    size_t s = 0;
    auto* context = LuaExecutor::LuaContextVariable::getFromPeak(l);
    auto* executor = context->e();
    lua_pop(l, 1);
    const char* funcName = lua_tolstring(l, lua_gettop(l), &s);
    LOG_DEBUG("funcName=" << funcName << " id=" << *executor);
    lua_pop(l, 1);
    
    lua_getglobal(l, funcName);
    
    try{
        LOG_TRACE("doMapForAtoms");
        doMapForAtoms(context);
    } catch(const LuaExecutionExeption& e) {
        std::string err = util::concat(e.what(), " in function ", funcName);
        throw LuaExecutionExeption(err);
    }
    executor->endMap();
    context->pushOnStack();
    return 1;//count of returns value
}

int doReduce(Lua l) {
    int argc = lua_gettop(l);
    LOG_DEBUG("argc=" << argc);
    size_t s = 0;
    auto* context = LuaExecutor::LuaContextVariable::getFromPeak(l);
    auto* executor = context->e();
    lua_pop(l, 1);
    const char* funcName = lua_tolstring(l, lua_gettop(l), &s);
    LOG_DEBUG("funcName=" << funcName << " id=" << *executor);
    lua_pop(l, 1);
    
    lua_getglobal(l, funcName);
    try{
        doReduceForAtoms(context, funcName);
    } catch(const LuaExecutionExeption& e) {
        std::string err = util::concat(e.what(), " in function ", funcName);
        throw LuaExecutionExeption(err);
    }
    LOG_TRACE("");
    executor->endReduce();
    context->pushOnStack();
    return 1;//count of returns value
}
