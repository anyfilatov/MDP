/* 
 * File:   Node.h
 * Author: alexander
 *
 * Created on May 4, 2015, 7:45 PM
 */
#pragma once
#include <list>
#include <vector>

static int idGenerator=0;

template<typename T>
class Node{
private:
    int id_;
    T value_;
    std::vector<Node<T>* > children_;
    Node<T> *parent_;
    
public:
    Node();
    Node(T value);
    int getID();
    void setValue(T value);
    T* getValue();
    void setChidren(Node<T> *children);
    std::vector<Node<T>* > getChildren();
    void addChild( T node);
    Node<T>* getParent();
    Node<T>* getLastChild();
    ~Node();
};

template<typename T>
int Node<T>::getID(){
    return this->id_;
}
template<typename T>
Node<T>::Node(): id_(++idGenerator), parent_(NULL){
}

template<typename T>
Node<T>::Node(T value): id_(++idGenerator),value_(value), parent_(NULL){
}

template<typename T>
void Node<T>::setValue(T value){
    this->value_ = value;
}

template<typename T>
T* Node<T>::getValue(){
    return &value_;
}

template<typename T>
void Node<T>::setChidren(Node<T>* children){
    children_.push_back(children);
}

template<typename T>
std::vector<Node<T>* > Node<T>::getChildren(){
    return children_;
}

template<typename T>
Node<T>* Node<T>::getLastChild(){
    return children_.back();
}


template<typename T>
void Node<T>::addChild(T node){  
    Node<T>* newObject = new Node<T>(node);
    newObject->parent_ = this;
    children_.push_back(newObject);
}

template<typename T>
Node<T>* Node<T>::getParent(){
    return parent_;
}

template<typename T>
Node<T>::~Node(){

}
