#pragma once
#include "GraphNode.h"
#include "abstractClient.h"

namespace integration {
    template <typename T, typename V>
    class BiGraph{
    protected:
        std::vector<GraphNode<T,V>*>* l;
        std::vector<GraphNode<V,T>*>* r;
    public:
        BiGraph();

        void addL(T lN);
        void addR(V rN);
        void setLinks(char part, int from, const std::vector<int>& to);

        GraphNode<T,V>& getL(int i);
        GraphNode<T,V>& getR(int i);
    };

    template <typename T, typename V>
    BiGraph<T,V>::BiGraph(){
        l = new std::vector<GraphNode<T,V>*>();
        r = new std::vector<GraphNode<V,T>*>();
    }

    template <typename T, typename V>
    void BiGraph<T,V>::addL(T lN){
        l->push_back(new GraphNode<T,V>(&lN));
    }

    template <typename T, typename V>
    void BiGraph<T,V>::addR(V rN){
        r->push_back(new GraphNode<V,T>(&rN));
    }

    template <typename T, typename V>
    void BiGraph<T,V>::setLinks(char part, int from, const std::vector<int>& to){
        if(part == 'l' || part == 'L'){
            if(to.size() != 0){
                for(int i = 0; i < (int)to.size(); i++){
                    l->at(from)->addLink(r->at(to[i]));
                    r->at(to[i])->addLink(l->at(from));
                }
            }
            else{
                for(int i = 0; i < (int)r->size(); i++){
                    l->at(from)->addLink(r->at(i));
                    r->at(i)->addLink(l->at(from));
                }
            }
        }
        if(part == 'r' || part == 'R'){
            if(to.size() != 0){
                for(int i = 0; i < (int)to.size(); i++){
                    r->at(from)->addLink(l->at(to[i]));
                    l->at(to[i])->addLink(r->at(from));
                }
            }
            else{
                for(int i = 0; i < (int)l->size(); i++){
                    r->at(from)->addLink(l->at(i));
                    l->at(i)->addLink(r->at(from));
                }
            }
        }
    }
    template <typename T,typename V>
    GraphNode<T,V>& BiGraph<T,V>::getL(int i){
        return *(l->at(i));
    }
    template <typename T,typename V>
    GraphNode<T,V>& BiGraph<T,V>::getR(int i){
        return *(r->at(i));
    }
}
