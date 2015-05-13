/* 
 * File:   Iterator.h
 * Author: alexander
 *
 * Created on May 7, 2015, 12:07 AM
 */

#include "Node.h"
#include "OrGraph.h"
#include <list>
#include <QtDebug>
#include <vector>

template<typename T>
class Iterator{
    private:
        OrGraph<T>* graph_;
        std::list<int> *markedNodes_;
        Node<T>* current_;
        
    public:
        bool contains(int id);
        Iterator(OrGraph<T> &graph);
        Node<T>* begin();
        bool end();
        Node<T>* next();
        ~Iterator();
        T* operator ->(){return current_->getValue();}
        T& operator *(){return *current_->getValue();}
        
};

template<typename T>
Iterator<T>::Iterator(OrGraph<T> &graph){
    graph_ = &graph;
    markedNodes_ = new std::list<int>;
    current_ =  graph.getRoot();
    markedNodes_->push_back(current_->getID());
}

template<typename T>
Node<T>* Iterator<T>::begin(){
    current_ = graph_->getRoot();
    markedNodes_->clear();
    markedNodes_->push_back(current_->getID());
    return current_;
}

template<typename T> 
bool Iterator<T>::end(){
    if (current_ == NULL)
        return true;
    return false;
}

template<typename T>
Node<T>* Iterator<T>::next(){
    if (current_ != NULL){
        if (!current_->getChildren().empty()){
            std::vector<Node<T>* > children = current_->getChildren();
            for(unsigned i = 0; i < children.size(); i++){
                if (!contains(children.at(i)->getID())){
                    current_ = current_->getChildren().at(i);
                    markedNodes_->push_back(current_->getID());
                    return current_;
                }
            }
            
        }
        
        if (current_ != graph_->getRoot()){
            current_ = current_->getParent();
            next();
        }
        else {
            current_ = NULL;
            return current_;
        }
            
        
    }
    return NULL;
}

template<typename T>
bool Iterator<T>::contains(int id){
        typename std::list<int>::iterator iter;
        iter = std::find(markedNodes_->begin(), markedNodes_->end(), id);
        
        if(iter != markedNodes_->end())
                return true;
        return false;
}

template<typename T>
Iterator<T>::~Iterator(){
}
