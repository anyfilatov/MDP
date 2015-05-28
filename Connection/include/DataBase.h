#pragma once
#include "wrapper.h"
#include "Logger.h"
#include "util.h"
#include <vector>
#include <map>
#include "dbclient.h"

class DataBase;

typedef Wrapper<DataBase> DB;

class DataBase {

    static thread_local DBClient* client_;
    int setLimit = 10000;
    int getLimit = 10000;
public:
    typedef std::vector<int> IntArray;
    typedef std::shared_ptr<MDPData> GetAtomType;
    typedef std::shared_ptr<MDPData> SetAtomType;
    DataBase(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort)
        : strHost_(strHost), nPort_(nPort), strSpareHost_(strSpareHost), nSparePort_(nSparePort)
    {
        LOG_TRACE("DataBase");
    };
    void create(){
        if(!client_){
            client_ = new DBClient(strHost_, nPort_, strSpareHost_, nSparePort_);
        }
    }
    DataBase(const DataBase& ) = delete;
    
    GetAtomType getNextAtom(util::Id& id) {
        create();
        LOG_DEBUG(" " << id.i0 << " " << id.i1 << " "<< id.i2);
        auto* d = client_->getNextStrings(id.i0, id.i1, id.i2, getLimit);
//        auto* d = client_.getNextStrings(0, 2, 1, 1);
        std::shared_ptr<MDPData> ptr(d);
//        std::shared_ptr<MDPData> ptr;
//        if(a==0){
//            ptr.reset(new MDPData());
//            ptr->generateRandom();
//            a++;
//        }
        return ptr;
    }
    
    IntArray getAllTablesId(util::Id& id) {
        create();
        IntArray out;
        for( auto& a : client_->getTableIds(id.i0)) {
            out.push_back( a.first);
        }
        if(out.empty()){
            out.push_back(1);
        }
        return out;
    }
    
    void setSwap(util::Id& id, SetAtomType val) {
        create();
        LOG_DEBUG("Set id: " << id.str().toStdString());
        if(!atoms){
            atoms.reset(new SetAtomType::element_type(*val));
        } else {
            if( val->getCells()[0].size() == 2 ){
                auto& cells = atoms->getCells();
                cells.push_back(vector<QString>());
                cells.back().swap(val->getCells()[0]);
            }
            if(atoms->size() >= setLimit){
                flush(id);
            }
        }
    }
    
    void flush(util::Id& id) {
        create();
        if(atoms){
            LOG_INFO("atoms size:" << atoms->getCells().size());
            client_->put(id.i0, id.i1, id.i2, atoms.get());
            atoms.reset();
        }
    }

    virtual ~DataBase(){
        if(client_){
            delete client_;
            client_ = nullptr;
        }
        LOG_TRACE("DataBase");
    }
    
private:
    int limit = 10000;
    int a =0;
    SetAtomType atoms;
    QString strHost_;
    int nPort_;
    QString strSpareHost_;
    int nSparePort_;
};

