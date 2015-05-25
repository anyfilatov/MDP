#pragma once
#include <vector>
#include <map>
#include "wrapper.h"
#include "Logger.h"
#include "errors.h"
#include "util.h"
#include <list>
#include <QString>
#include "client.h"
#include "abstractexception.h"
const int BUFFER_SIZE = 1000;
class RbTree {
    std::shared_ptr<Client> client_;
    QString fileName_;
public:
    typedef std::pair<QString, QStringList> GetAtomType;
    typedef std::vector<QString> SetAtomType;
    RbTree(const QString& fileName) : client_(new Client(BUFFER_SIZE, fileName)), fileName_(fileName){
        LOG_TRACE("RBTree");
    };
    RbTree(const RbTree& ) = delete;

    QStringList getAllKeys (const util::Id& id) {
        QStringList out;
        try{
            out = client_->getBucketKeys(id.str());
        } catch ( AbstractException& e ) {
            client_.reset(new Client(BUFFER_SIZE, fileName_));
            LOG_ERROR("exception:" << e.getMessage().toStdString());
        }
        return out;
    }
    
    int setSwap(util::Id& id, SetAtomType& atom) {

        if(atom.size() == 2){
            try{
                client_->put(atom[0], atom[1], id.str());
            } catch ( AbstractException& e ) {
                client_.reset(new Client(BUFFER_SIZE, fileName_));
                LOG_ERROR("exception:" << e.getMessage().toStdString());
                return Errors::STATUS_ERROR;
            }
        } else {
            LOG_ERROR("in array size error:" << atom.size());
            return Errors::STATUS_ERROR;
        }
        return Errors::STATUS_OK;
    }

    GetAtomType getNextAtom(util::Id& id, const QString& key) {
        try{
            return std::make_pair( key, client_->get(key, id.str()));
        } catch ( AbstractException& e ) {
            client_.reset(new Client(BUFFER_SIZE, fileName_));
            LOG_ERROR("exception:" << e.getMessage().toStdString());
        }
        return GetAtomType();
    }
    void flush() {
        if(client_) {
            client_->flush();
        }
    }
    virtual ~RbTree(){
        LOG_TRACE("~RBTree");
    };
private:
};

typedef Wrapper<RbTree> RB;
