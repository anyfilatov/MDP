#pragma once
#include <list>
#include <iostream>
#include "Node.h"
#include "wrapper.h"
#include "host.h"

typedef Wrapper<std::vector<Host>> OG ;
using namespace std;
template <class T> class OrGraph;

template <class T>
ostream &operator<<(ostream &os,  OrGraph<T>& graph){
    os <<  graph.getObject();
    return os;
}

template <class T> class OrGraph
{
private:
        
        Node<T> *root_;//value of node
        //std::list<OrGraph<T> > *chidren;//list of nodes
public:
    OrGraph();
    OrGraph(const T &root);
    OrGraph(Node<T> & _root);
    OrGraph(Node<T> * _root);
    ~OrGraph();
    void setRoot(Node<T> &_root);
    void setRoot(T root);
    void addChild(Node<T> &_root);
    void addChild(T graphElement);
    void addChild(Node<T> *graphElement);
    void deleteChild(int id);
    Node<T>* getRoot();
    //std::list<Node<T> >* getChildren();
    bool empty();
    friend ostream& operator<< <>(ostream& os, OrGraph<T>& graph);
    template <typename T2> void sendAllChidren(T2);
};

template <class T>
OrGraph<T>::OrGraph()
{
    
}

template <class T>
OrGraph<T>::OrGraph(Node<T> &_root)
{
    this->root_ = &_root;
    
}

template <class T>
OrGraph<T>::OrGraph(const T &root)
{
    this->root_ = new Node<T>(root);
    
}

template <class T>
OrGraph<T>::OrGraph(Node<T> *_root)
{
    this->root_ = _root;
}

template <class T>
template <typename T2> void OrGraph<T>::sendAllChidren(T2 f)
{

//    f(this->getRoot());
//    if(!this->root_->getChildren()->empty()){
//        typename list<OrGraph<T> >::iterator iter;
//        for(iter= this->root_->getChildren()->begin(); iter != this->root_->getChildren()->end(); ++iter){
//           iter->sendAllChidren(f);
//        }
//    }
    return;
}

template <class T>
Node<T>* OrGraph<T>::getRoot()
{
     return this->root_;
}

template <class T>
bool OrGraph<T>::empty()
{
    if (root_ == NULL)
        return true;
    return false;
}

template <class T>
void OrGraph<T>::setRoot(Node<T> &_root)
{
    this->root_ = _root;
}

template <class T>
void OrGraph<T>::addChild(Node<T> &node){
     this->root_->addChild(node);
}

template <class T>
void OrGraph<T>::addChild(T graphElement){
    this->root_->addChild(graphElement);
}


template <class T>
void OrGraph<T>::addChild(Node<T> *graphElement){
    Node<T> graph(*graphElement);
    this->addChild(graph);
}

template <class T>
void OrGraph<T>::deleteChild(int id){
//    if(this->getID() == id){
//        delete this;
//        return;
//    }else{
//        if(!this->root_->getChildren()->empty()){
//            typename list<OrGraph<T> >::iterator iter;
//            for(iter= this->root_->getChildren()->begin(); iter != this->root_->getChildren()->end(); ++iter){
//               iter->deleteChild(id);
//            }
//        }
//    }
}

//template <class T>
//std::list<Node<T> >* OrGraph<T>::getChildren(){
//    return this->root_->getChildrenList();
//}

template <class T>
OrGraph<T>::~OrGraph()
{
}


