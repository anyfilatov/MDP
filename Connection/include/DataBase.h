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
    DataBase(const QString& strHost, int nPort, const QString& strSpareHost, int nSparePort) : client_( strHost, nPort, strSpareHost, nSparePort) {
        LOG_TRACE("DataBase");
    };

    DataBase(const DataBase& ) = delete;
    
    GetAtomType getNextAtom(util::Id& id) {
        auto* d = client_.getNextStrings(id.i0, id.i1, id.i2, 1);
        std::shared_ptr<MDPData> ptr(d);
        return ptr;
    }
    
    IntArray getAllTablesId(util::Id& id) {
        IntArray out;
        for( auto& a : client_.getTableIds(id.i0)) {
            out.push_back( a.first);
        }
        return out;
    }
    
    void setSwap(util::Id& id, SetAtomType val) {
        client_.put(id.i0, id.i1, id.i2, val.get());
    }
    
    virtual ~DataBase(){
        LOG_TRACE("DataBase");
    }
    
private:
};

