#pragma once
#include <vector>
#include <map>
#include "wrapper.h"
#include "Logger.h"
#include "errors.h"
#include "util.h"
#include <list>
#include <QString>

class RbTree {
    typedef std::map<std::string, std::vector<std::string>> KeepedType;
public:
    typedef std::pair<std::string, std::vector<std::string>> GetAtomType;
    typedef std::pair<std::string, std::string> SetAtomType;
    RbTree(){
        LOG_TRACE("RBTree");
    };
    RbTree(const RbTree& ) = delete;
    
    std::list<QString> getAllKeys() {
        return std::list<QString>();
    }
    
    int setSwap(util::Id& , SetAtomType& atom) {
        rb_.insert(std::make_pair(atom.first, KeepedType::mapped_type()))
        .first->second.push_back(std::move(atom.second));
        return Errors::STATUS_OK;
    }
    
    int set(util::Id&, const SetAtomType& atom) {
        rb_.insert(std::make_pair(atom.first, KeepedType::mapped_type()))
        .first->second.push_back(atom.second);
        return Errors::STATUS_OK;
    }
    
    GetAtomType getNextAtom(util::Id&) {
        return {"test", {"test1", "test2"}};
    }
    
    virtual ~RbTree(){
        LOG_TRACE("~RBTree");
    };
private:
    KeepedType rb_;
};

typedef Wrapper<RbTree> RB;