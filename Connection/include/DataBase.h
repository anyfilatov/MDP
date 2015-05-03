#pragma once
#include "wrapper.h"
#include "Logger.h"
#include "util.h"
#include <vector>
#include <map>

class DataBase;

typedef Wrapper<DataBase> DB;

class DataBase {
    typedef std::map<std::string, std::string> KeepedType;
public:
    typedef std::vector<int> IntArray;
    typedef std::string GetAtomType;
    typedef std::pair<std::string, std::string> SetAtomType;
    DataBase() {
        LOG_TRACE("DataBase");
    };
    DataBase(const DataBase& ) = delete;
    GetAtomType getNextAtom(util::Id& ) {
        return "test";
    }
    
    IntArray getAllTablesId(util::Id& ) {
        IntArray vec;
        vec.push_back(0);
        return vec;
    }
    
    void setSwap(util::Id, SetAtomType val) {
        if( !db_.emplace(val).second ){
            LOG_WARNING("Set to db error. Key" << val.first << " exist");
        };
    }
    
    virtual ~DataBase(){
        LOG_TRACE("DataBase");
    }
    
private:
    KeepedType db_;
};

