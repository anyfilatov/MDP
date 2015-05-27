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

    DBClient client_;
public:
    typedef std::vector<int> IntArray;
    typedef std::shared_ptr<MDPData> GetAtomType;
    typedef std::shared_ptr<MDPData> SetAtomType;
    DataBase(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort)
        : client_( strHost, nPort, strSpareHost, nSparePort)
    {
        LOG_TRACE("DataBase");
    };

    DataBase(const DataBase& ) = delete;
    
    GetAtomType getNextAtom(util::Id& id) {
        LOG_DEBUG(" " << id.i0 << " " << id.i1 << " "<< id.i2);
        auto* d = client_.getNextStrings(id.i0, id.i1, id.i2, 10000);
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
        IntArray out;
        for( auto& a : client_.getTableIds(id.i0)) {
            out.push_back( a.first);
        }
        if(out.empty()){
            out.push_back(1);
        }
        return out;
    }
    
    void setSwap(util::Id& id, SetAtomType val) {
        LOG_DEBUG("Set id: " << id.str().toStdString());
        client_.put(id.i0, id.i1, id.i2, val.get());
    }
    
    virtual ~DataBase(){
        LOG_TRACE("DataBase");
    }
    
private:
    int a =0;
};

