#pragma once
#include <list>
#include <iostream>
#include "host.h"
#include "wrapper.h"

using namespace std;
static int idGenerator = 0;
template <class T> class OrGraph;

//template <typename T> using OG = Wrapper<OrGraph<Host>>;
using OG = Wrapper<std::vector<Host>>;

template <class T>
ostream &operator<<(ostream &os, OrGraph<T>& graph) {
    os << graph.getObject();
    return os;
}

template <class T> class OrGraph {
private:
    int id;
    T root; //value of node
    std::list<OrGraph<T> > chidren; //list of nodes
public:
    OrGraph();
    OrGraph(T _root);
    ~OrGraph();
    void setRoot(T _root);
    void addChild(OrGraph<T> _root);
    void addChild(T graphElement);
    void addChild(OrGraph<T> *graphElement);
    void deleteChild(int id);
    T getRoot();
    int getID();
    std::list<OrGraph<T> > getChildren();
    friend ostream& operator<< <>(ostream& os, OrGraph<T>& graph);
    template <typename T2> void sendAllChidren(T2);
public:
    friend class iterator;

    class iterator {
    public:

        iterator operator++() {
            
        }
        //++x; // calls Number::operator++(), i.e., calls x.operator++()
        //x++; // calls Number::operator++(int), i.e., calls x.operator++(0)
    private:
        T* getNext() {
            return nullptr;
        }
    private:
        T* current = nullptr;
        OrGraph<T>* graph_;
    };
private:
    template <typename T2> void allAction(T2 t) {
        t(*this);
        if(chidren){
            for(auto& c : chidren) {
                c.allAction(t);
            }
        }
    }
};

template <class T>
OrGraph<T>::OrGraph() {
    this->chidren = new std::list<OrGraph<T> >;
    this->id = ++idGenerator;
}

template <class T>
OrGraph<T>::OrGraph(T _root) {
    this->root = _root;
    this->id = ++idGenerator;
}

template <class T>
int OrGraph<T>::getID() {
    return this->id;
}

template <class T>
template <typename T2> void OrGraph<T>::sendAllChidren(T2 f) {

    f(this->getRoot());
    if (!this->chidren.empty()) {
        typename list<OrGraph<T> >::iterator iter;
        for (iter = this->chidren.begin(); iter != this->chidren.end(); ++iter) {
            iter->sendAllChidren(f);
        }
    }
    return;
}

template <class T>
T OrGraph<T>::getRoot() {
    return root;
}

template <class T>
void OrGraph<T>::setRoot(T _root) {
    this->root = _root;
}

template <class T>
void OrGraph<T>::addChild(OrGraph<T> _root) {
    this->chidren->push_back(_root);
}

template <class T>
void OrGraph<T>::addChild(T graphElement) {
    OrGraph<T> *graph = new OrGraph<T>;
    graph->setRoot(graphElement);
    this->addChild(graph);
}

template <class T>
void OrGraph<T>::addChild(OrGraph<T> *graphElement) {
    OrGraph<T> graph(*graphElement);
    this->addChild(graph);
}

template <class T>
void OrGraph<T>::deleteChild(int id) {
    if (this->getID() == id) {
        delete this;
        return;
    } else {
        if (!this->chidren->empty()) {
            typename list<OrGraph<T> >::iterator iter;
            for (iter = this->chidren->begin(); iter != this->chidren->end(); ++iter) {
                iter->deleteChild(id);
            }
        }
    }
}

template <class T>
std::list<OrGraph<T> > OrGraph<T>::getChildren() {
    return *this->chidren;
}

template <class T>
OrGraph<T>::~OrGraph() {
}

