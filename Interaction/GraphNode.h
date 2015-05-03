#pragma once
#include <vector>
namespace integration {
    template <typename T, typename V>
    class GraphNode{
        T* node;
        std::vector<GraphNode<V,T>*>* links;

    public:
        GraphNode(const GraphNode<T,V>& clone);
        GraphNode(T* node = 0, std::vector<GraphNode<V,T>*>* links = 0);
        ~GraphNode();

        T& getNode();
        std::vector<GraphNode<V,T>*>* getLinks();

        void setNode(const T& node);
        void setLinks(std::vector<GraphNode<V,T>*>* links);

        void addLink(GraphNode<V,T>* anNode);
    };

    template <typename T, typename V>
    GraphNode<T,V>::GraphNode(const GraphNode<T,V>& clone){
        node = new T(*clone.node);
        links = new std::vector<GraphNode<V,T>*>();
        for(int i = 0; i < (int)clone.links->size(); i++){
            links->push_back(clone.links->at(i));
        }
    }

    template <typename T, typename V>
    GraphNode<T,V>::GraphNode(T* node, std::vector<GraphNode<V,T>*>* links){
        if(node != 0)
            this->node = new T(*node);
        else
            this->node = 0;
        if(links != 0){
            this->links = new std::vector<GraphNode<V,T>*>();
            for(int i = 0; i < (int)links->size(); i++)
                this->links->push_back(links->at(i));
        }
        else
            this->links = new std::vector<GraphNode<V,T>*>();
    }

    template <typename T, typename V>
    GraphNode<T,V>::~GraphNode(){
        delete node;
        node = 0;
        delete links;
        links = 0;
    }


    template <typename T, typename V>
    T& GraphNode<T,V>::getNode(){
        return *node;
    }

    template <typename T, typename V>
    void GraphNode<T,V>::setNode(const T& node){
        this->node = new T(node);
    }

    template <typename T, typename V>
    std::vector<GraphNode<V,T>*>* GraphNode<T,V>::getLinks(){
        return links;
    }

    template <typename T, typename V>
    void GraphNode<T,V>::setLinks(std::vector<GraphNode<V,T>*>* links){
        this->links = links;
    }

    template <typename T, typename V>
    void GraphNode<T,V>::addLink(GraphNode<V,T>* anNode){
        links->push_back(anNode);
    }
}
