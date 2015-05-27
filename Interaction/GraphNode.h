#pragma once
#include <vector>
#include <iostream>
#include <assert.h>
#include <sstream>
#include "mymanipulators.h"
#include "bigraphexceptions.h"

using std::stringstream;

namespace integration {
    template <typename T, typename V>
    class GraphNode{
    protected:
        T* node;
        std::vector<GraphNode<V,T>*>* links;

    public:
        GraphNode(const GraphNode<T,V>& clone);
        GraphNode(const T* node = 0, std::vector<GraphNode<V,T>*>* links = 0);
        virtual ~GraphNode();

        T& getNode();
        std::vector<GraphNode<V,T>*>* getLinks();

        void setNode(const T& node);
        void setLinks(std::vector<GraphNode<V,T>*>* links);

        void addLink(GraphNode<V,T>* anNode);

        bool operator==(const GraphNode<T,V>& another)const;
        bool graphNodeIsCorrect() const;
        bool isSubGraphNodeOf(const GraphNode<T,V>& mother) const;

        template <typename T1, typename V1>
        friend ostream& operator << (ostream& stream, const GraphNode<T1,V1>& node);
    };

    template <typename T, typename V>
    GraphNode<T,V>::GraphNode(const GraphNode<T,V>& clone){
        node = new T(*clone.node);
        links = new std::vector<GraphNode<V,T>*>();
        for(int i = 0; i < (int)clone.links->size(); i++){
            links->push_back(clone.links->at(i));
        }
        assert("|INVARIANT|\tin copy constructor"&&graphNodeIsCorrect());
    }

    template <typename T, typename V>
    GraphNode<T,V>::GraphNode(const T* node, std::vector<GraphNode<V,T>*>* links){
        if(node != 0)
            this->node = new T(*node);
        else{
            this->node = 0;
            throw Excpt_null_ptr();
        }
        if(links != 0){
            this->links = new std::vector<GraphNode<V,T>*>();
            for(int i = 0; i < (int)links->size(); i++)
                this->links->push_back(links->at(i));
        }
        else
            this->links = new std::vector<GraphNode<V,T>*>();
        assert("|INVARIANT|\tin constructor"&&graphNodeIsCorrect());
    }

    template <typename T, typename V>
    GraphNode<T,V>::~GraphNode(){
        assert("|INVARIANT|\tin destructor"&&graphNodeIsCorrect());
        if(this != 0){
            //std::cout << "begin of deleting graph node with value = " << *node << std::endl;
            delete node;
            node = 0;
            if(links != 0)
                links->clear();
            delete links;
            links = 0;
            //std::cout << "end of deleting graph node" << std::endl;
        }
        else{
            throw Excpt_null_ptr();
        }
    }


    template <typename T, typename V>
    T& GraphNode<T,V>::getNode(){
        if(node == 0)
            throw Excpt_null_ptr();
        assert("|INVARIANT|\tin getNode"&&graphNodeIsCorrect());
        return *node;
    }

    template <typename T, typename V>
    void GraphNode<T,V>::setNode(const T& node){
        if(node == 0)
            throw Excpt_null_ptr();
        assert("|INVARIANT|\tin setNode before"&&graphNodeIsCorrect());
        this->node = new T(node);
        assert("|INVARIANT|\tin setNode after"&&graphNodeIsCorrect());
    }

    template <typename T, typename V>
    std::vector<GraphNode<V,T>*>* GraphNode<T,V>::getLinks(){
        assert("|INVARIANT|\tin getLinks"&&graphNodeIsCorrect());
        return links;
    }

    template <typename T, typename V>
    void GraphNode<T,V>::setLinks(std::vector<GraphNode<V,T>*>* links){
        assert("|INVARIANT|\tin setLinks before"&&graphNodeIsCorrect());
        this->links = links;
        assert("|INVARIANT|\tin setLinks after"&&graphNodeIsCorrect());
    }

    template <typename T, typename V>
    void GraphNode<T,V>::addLink(GraphNode<V,T>* anNode){
        assert("|INVARIANT|\tin addLink before"&&graphNodeIsCorrect());
        links->push_back(anNode);
        assert("|INVARIANT|\tin addLink after"&&graphNodeIsCorrect());
    }
    template <typename T, typename V>
    bool GraphNode<T,V>::operator==(const GraphNode<T,V>& another)const{
        assert("|INVARIANT|\tin operator =="&&graphNodeIsCorrect());
        if(*(this->node) != *another.node || this->links->size() != another.links->size())
            return false;
        bool allEqual = true;
        for(int i = 0; i < (int)this->links->size() && allEqual; i++){
            int j = 0;
            bool wasFoundEqual = false;
            while(!wasFoundEqual && j < (int)another.links->size()){
                if(i > this->links->size() || j > another.links->size())
                    throw Excpt_vec_out_of_range();
                if((this->links->at(i)->getNode()) == (another.links->at(j)->getNode()))
                    wasFoundEqual = true;
                j++;
            }
            allEqual &= wasFoundEqual;
        }
        return allEqual;
    }

    template <typename T, typename V>
    bool GraphNode<T,V>::graphNodeIsCorrect() const{
        return (node)&&(links);
    }

    template <typename T, typename V>
    bool GraphNode<T,V>::isSubGraphNodeOf(const GraphNode<T,V>& mother) const{
        if(*(this->node) != *(mother.node))
            return false;
        bool allFound = true;
        for(int i = 0; i < (int)links->size(); i++){
            bool wasFound = false;
            for(int j = 0; j < (int)mother.links->size() && !wasFound; j++){

                if(links->at(i)->getNode() == mother.links->at(j)->getNode())
                    wasFound = true;
            }
            allFound &= wasFound;
        }
        return allFound;
    }

    template <typename T, typename V>
    ostream& operator << (ostream& stream, const integration::GraphNode<T,V>& node){
        stream << "node: ";
        for(int i = 0; i < (int)node.links->size(); i++){
            stream << out(i,6) << "link" << i << std::endl;
        }
        return stream;
    }


}
