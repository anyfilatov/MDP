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
#include <unistd.h>

const int BUFFER_SIZE = 7000;
class RbTree {
    static thread_local Client* client_;
    QString fileName_;
public:
    typedef std::pair<QString, QStringList> GetAtomType;
    typedef std::vector<QString> SetAtomType;
    RbTree(const QString& fileName) : fileName_(fileName){
        LOG_TRACE("RBTree");
    };
    RbTree(const RbTree& ) = delete;

    QStringList getAllKeys (const util::Id& id) {
        create();
        QStringList out;


        try{
            //sleep(10);
            out = client_->getBucketKeys(id.str());
        } catch ( AbstractException& e ) {
            reset();
            LOG_ERROR("exception:" << e.getMessage().toStdString());
        }

        for(auto& l: out)
            LOG_DEBUG(l.toStdString());
        return out;
    }
    
    int setSwap(util::Id& id, SetAtomType& atom) {
        create();
        if(atom.size() == 2){
            try{
                client_->put(atom[0], atom[1], id.str());
            } catch ( AbstractException& e ) {
                reset();
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
        create();
        try{
            return std::make_pair( key, client_->get(key, id.str()));

        } catch ( AbstractException& e ) {
            reset();
            LOG_ERROR("exception:" << e.getMessage().toStdString());
        }
        return GetAtomType();
    }
    void flush() {
        if(client_) {
            LOG_INFO("flush");
            client_->flush();
        }
    }
    virtual ~RbTree(){
        LOG_TRACE("~RBTree");
        if(client_){
            delete client_;
            client_ = nullptr;
        }
    };
private:
    void create(){
        if(!client_){
            reset();
        }
    }

    void reset(){
        if(client_){
            delete client_;
            client_ = nullptr;
        }
        client_ = new Client(BUFFER_SIZE, fileName_);
    }
};

typedef Wrapper<RbTree> RB;
