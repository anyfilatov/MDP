#pragma once
#include "GraphNode.h"
#include "allocator.h"
#include <iostream>
#include <sstream>
#include <typeinfo>

using std::vector;
using std::stringstream;

namespace integration {
    template <typename T, typename V>
    class BiGraph{
    protected:
        std::vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >* l;
        std::vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >* r;
    public:
        BiGraph();
        BiGraph(const BiGraph<T,V>& another);
        ~BiGraph();

        void addL(const T& lN);
        void addR(const V& rN);
        void setLinks(char part, int from, const vector<int>& to);
        void setLink(int from, int to);

        GraphNode<T,V>* deleteL(int pos);
        GraphNode<V,T>* deleteR(int pos);

        GraphNode<T,V>& getL(int i);
        GraphNode<V,T>& getR(int i);

        class LeftIteratorBiGraph{
            int pos;
            std::vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >* data;
        public:
            LeftIteratorBiGraph(const int& pos = 0, std::vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >* const data = 0):pos(pos),data(data){}
            LeftIteratorBiGraph(const LeftIteratorBiGraph& clone){
                pos = clone.pos;
                data = clone.data;
            }

            GraphNode<T,V>& getCurrent(){
                if(pos > data->size())
                    throw Excpt_vec_out_of_range();
                if(data->at(pos))
                    throw Excpt_null_ptr();
                return *(data->at(pos));
            }
            friend LeftIteratorBiGraph operator +(LeftIteratorBiGraph& curIt, const int& i){
                LeftIteratorBiGraph ans(curIt);
                ans.pos += i;
                return ans;
            }
            LeftIteratorBiGraph& operator =(const LeftIteratorBiGraph& clone){
                pos = clone.pos;
                data = clone.data;
                return *this;
            }
            bool operator ==(const LeftIteratorBiGraph& meq){
                return (pos==meq.pos && data==meq.data);
            }

            LeftIteratorBiGraph& goNext(){
                pos++;
                return *this;
            }
            void goToStart(){
                pos = 0;
            }
            bool isInStart(){
                return pos == 0;
            }
            bool isInEnd(){
                return pos >= (int)data->size();
            }
        };
        class RightIteratorBiGraph{
            int pos;
            std::vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >* data;
        public:
            RightIteratorBiGraph(const int& pos = 0, std::vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >* const data = 0):pos(pos),data(data){}
            RightIteratorBiGraph(const RightIteratorBiGraph& clone){
                pos = clone.pos;
                data = clone.data;
            }

            GraphNode<V,T>& getCurrent(){
                if(pos > data->size())
                    throw Excpt_vec_out_of_range();
                if(data->at(pos))
                    throw Excpt_null_ptr();
                return *(data->at(pos));
            }
            friend RightIteratorBiGraph operator +(RightIteratorBiGraph& curIt, const int& i){
                RightIteratorBiGraph ans(curIt);
                ans.pos += i;
                return ans;
            }
            RightIteratorBiGraph& operator =(const RightIteratorBiGraph& clone){
                pos = clone.pos;
                data = clone.data;
                return *this;
            }
            bool operator ==(const RightIteratorBiGraph& meq){
                return (pos==meq.pos && data==meq.data);
            }

            RightIteratorBiGraph& goNext(){
                pos++;
                return *this;
            }
            void goToStart(){
                pos = 0;
            }
            bool isInStart(){
                return pos == 0;
            }
            bool isInEnd(){
                return pos >= (int)data->size();
            }
        };


        LeftIteratorBiGraph getIteratorL();
        RightIteratorBiGraph getIteratorR();

        bool operator ==(const BiGraph<T,V>& another)const;
        BiGraph<T,V> operator =(const BiGraph<T,V>& another);
        BiGraph<T,V> operator +(GraphNode<T,V>*const add)const;
        BiGraph<T,V> operator +(GraphNode<V,T>*const add)const;
        BiGraph<T,V> operator -(GraphNode<T,V>* const sub) const;
        BiGraph<T,V> operator -(GraphNode<V,T>* const sub) const;

        bool isSubGraphOf(const BiGraph<T,V>& mother)const;
        bool biGraphIsCorrect() const;

        template <typename T1, typename V1>
        friend ostream& operator << (ostream& stream, const BiGraph<T1,V1>& graph);
    };

    template <typename T, typename V>
    BiGraph<T,V>::BiGraph(){
        l = new vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >();
        r = new vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >();
        assert("|INVARIANT|      in constructor"&&biGraphIsCorrect());
    }

    template <typename T, typename V>
    BiGraph<T,V>::BiGraph(const BiGraph<T, V> &another){
        l = new vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >();
        r = new vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >();
        for(int i = 0; i < (int)another.l->size();i++){
            GraphNode<T,V>* tempL = new GraphNode<T,V>(&another.l->at(i)->getNode());
            l->push_back(tempL);
        }
        for(int i = 0; i < (int)another.r->size(); i++){
            GraphNode<V,T>* tempR = new GraphNode<V,T>(&another.r->at(i)->getNode());
            r->push_back(tempR);
            for(int j = 0; j < (int)another.r->at(i)->getLinks()->size(); j++){
                for(int k = 0; k < (int)another.l->size(); k++){
                    if(another.r->at(i)->getLinks()->at(j) == another.l->at(k))
                        setLink(k,i);
                }
            }
        }
        assert("|INVARIANT|      in copy constuctor"&&biGraphIsCorrect());
    }

    template <typename T, typename V>
    BiGraph<T,V>::~BiGraph(){
        assert("|INVARIANT|      in destructor"&&biGraphIsCorrect());
        for(int i = 0; i < (int)l->size(); i++){
            delete l->at(i);
            l->at(i) = 0;
        }
        l->clear();
        delete l;
        l = 0;
        for(int i = 0; i < (int)r->size(); i++){
            delete r->at(i);
            r->at(i) = 0;
        }
        r->clear();
        delete r;
        r = 0;
        //std::cout << "success\n";
    }

    template <typename T, typename V>
    void BiGraph<T,V>::addL(const T& lN){
        assert("|INVARIANT|      in addL before"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        l->push_back(new GraphNode<T,V>(&lN));
        assert("|INVARIANT|      in addL after"&&biGraphIsCorrect());
        GraphNode<T,V>* tempGN = new GraphNode<T,V>(&lN);
        assert("|POST|      addL"&&((temp+tempGN) == *this)&&temp.isSubGraphOf(*this));
        delete tempGN;
    }

    template <typename T, typename V>
    void BiGraph<T,V>::addR(const V& rN){
        assert("|INVARIANT|      in addR before"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        r->push_back(new GraphNode<V,T>(&rN));
        assert("|INVARIANT|      in addR after"&&biGraphIsCorrect());
        GraphNode<V,T>* tempGN = new GraphNode<V,T>(&rN);
        assert("|POST|      addR"&&((temp+tempGN) == *this)&&temp.isSubGraphOf(*this));
        delete tempGN;
    }

    template <typename T, typename V>
    GraphNode<T,V>* BiGraph<T,V>::deleteL(int pos){
        assert("|INVARIANT|      in deleteL before"&&biGraphIsCorrect());
        BiGraph<T,V> tempBiGraph(*this);
        if(pos >= (int)l->size()){
            throw Excpt_vec_out_of_range();
            return 0;
        }
        GraphNode<T,V>* temp = l->at(pos);
        for(int i = 0; i < (int)temp->getLinks()->size(); i++){
            int posTemp = 0;
            bool wasFoundTemp = false;
            while(!wasFoundTemp && posTemp < (int)temp->getLinks()->at(i)->getLinks()->size()){
                if(temp == temp->getLinks()->at(i)->getLinks()->at(posTemp)){
                    wasFoundTemp = true;
                    temp->getLinks()->at(i)->getLinks()->erase(temp->getLinks()->at(i)->getLinks()->begin() + posTemp);
                }
                posTemp++;
            }
        }
        //delete temp;
        l->at(pos) = 0;
        l->erase(l->begin()+pos);
        assert("|INVARIANT|      in deleteL after"&&biGraphIsCorrect());
        assert("|POST|      deleteL"&&(this->isSubGraphOf(tempBiGraph)));
        return temp;
    }
    template <typename T, typename V>
    GraphNode<V,T>* BiGraph<T,V>::deleteR(int pos){
        assert("|INVARIANT|      in deleteR before"&&biGraphIsCorrect());
        BiGraph<T,V> tempBiGraph(*this);
        if(pos >= (int)r->size()){
            throw Excpt_vec_out_of_range();
            return 0;
        }
        GraphNode<V,T>* temp = r->at(pos);
        for(int i = 0; i < (int)temp->getLinks()->size(); i++){
            int posTemp = 0;
            bool wasFoundTemp = false;
            while(!wasFoundTemp && posTemp < (int)temp->getLinks()->at(i)->getLinks()->size()){
                if(temp == temp->getLinks()->at(i)->getLinks()->at(posTemp)){
                    wasFoundTemp = true;
                    temp->getLinks()->at(i)->getLinks()->erase(temp->getLinks()->at(i)->getLinks()->begin() + posTemp);
                }
                posTemp++;
            }
        }
        //delete temp;
        r->at(pos) = 0;
        r->erase(r->begin()+pos);
        assert("|INVARIANT|      in deleteR after"&&biGraphIsCorrect());
        assert("|POST|      deleteR"&&(this->isSubGraphOf(tempBiGraph)));
        return temp;
    }

    template <typename T, typename V>
    void BiGraph<T,V>::setLinks(char part, int from, const vector<int>& to){
        assert("|INVARIANT|      in setLinks before"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
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
        assert("|INVARIANT|      in setLinks after"&&biGraphIsCorrect());
        assert("|POST|      in setLinks"&&temp.isSubGraphOf(*this));
    }

    template <typename T, typename V>
    void BiGraph<T,V>::setLink(int from, int to){
        assert("|INVARIANT|      in setLink before"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        if(from >= (int)l->size() || to >= (int)r->size()){
            throw Excpt_vec_out_of_range();
            return;
        }
        l->at(from)->addLink(r->at(to));
        r->at(to)->addLink(l->at(from));
        assert("|INVARIANT|      in setLink after"&&biGraphIsCorrect());
        assert("|POST|      in setLink"&&temp.isSubGraphOf(*this));
    }

    template <typename T,typename V>
    GraphNode<T,V>& BiGraph<T,V>::getL(int i){
        assert("|INVARIANT|      in getL"&&biGraphIsCorrect());
        if(i> (int)l->size())
            throw Excpt_vec_out_of_range();
        return *(l->at(i));
    }
    template <typename T,typename V>
    GraphNode<V,T>& BiGraph<T,V>::getR(int i){
        assert("|INVARIANT|      in getR"&&biGraphIsCorrect());
        if(i> (int)r->size())
            throw Excpt_vec_out_of_range();
        return *(r->at(i));
    }

    template <typename T,typename V>
    typename BiGraph<T,V>::LeftIteratorBiGraph BiGraph<T,V>::getIteratorL(){
        assert("|INVARIANT|      in getIteratorL"&&biGraphIsCorrect());
        BiGraph<T,V>::LeftIteratorBiGraph ans(0,l);
        return ans;
    }

    template <typename T,typename V>
    typename BiGraph<T,V>::RightIteratorBiGraph BiGraph<T,V>::getIteratorR(){
        assert("|INVARIANT|      in getIteratorR"&&biGraphIsCorrect());
        BiGraph<T,V>::RightIteratorBiGraph ans(0,r);
        return ans;
    }

    template <typename T,typename V>
    bool BiGraph<T,V>::operator==(const BiGraph<T,V>& another)const{
        assert("|INVARIANT|      in operator =="&&biGraphIsCorrect());
        if(this->l->size() != another.l->size() ||
           this->r->size() != another.r->size())
            return false;

        return this->isSubGraphOf(another) && another.isSubGraphOf(*this);

    }

    template <typename T,typename V>
    BiGraph<T,V> BiGraph<T,V>::operator = (const BiGraph<T,V>& another){
        assert("|INVARIANT|      in operator = before"&&biGraphIsCorrect());
        for(int i = 0; i < (int)l->size(); i++){
            deleteL(i);
        }
        for(int i = 0; i < (int)r->size(); i++){
            deleteR(i);
        }
        delete l;
        delete r;
        l = new vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >();
        r = new vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >();
        for(int i = 0; i < (int)another.l->size();i++){
            GraphNode<T,V>* tempL = new GraphNode<T,V>(&another.l->at(i)->getNode());
            l->push_back(tempL);
        }
        for(int i = 0; i < (int)another.r->size(); i++){
            GraphNode<V,T>* tempR = new GraphNode<V,T>(&another.r->at(i)->getNode());
            r->push_back(tempR);
            for(int j = 0; j < (int)another.r->at(i)->getLinks()->size(); j++){
                for(int k = 0; k < (int)another.l->size(); k++){
                    if(another.r->at(i)->getLinks()->at(j) == another.l->at(k))
                        setLink(k,i);
                }
            }
        }
        assert("|INVARIANT|      in operator = after"&&biGraphIsCorrect());
        assert("|POST|      in operator = "&&(another == *this));
        return *this;
    }

    template <typename T,typename V>
    BiGraph<T,V> BiGraph<T,V>::operator +(GraphNode<T,V> *const add)const{
        assert("|INVARIANT|      in operator +"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        temp.l->push_back(new GraphNode<T,V>(*add));
        assert("|INVARIANT|      in operator + for temp"&&temp.biGraphIsCorrect());
        return temp;
    }

    template <typename T,typename V>
    BiGraph<T,V> BiGraph<T,V>::operator +(GraphNode<V,T> *const add)const{
        assert("|INVARIANT|      in operator +"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        temp.r->push_back(new GraphNode<V,T>(*add));
        assert("|INVARIANT|      in operator + for temp"&&temp.biGraphIsCorrect());
        return temp;
    }

    template <typename T,typename V>
    BiGraph<T,V> BiGraph<T,V>::operator -(GraphNode<T,V>* const sub) const{
        assert("|INVARIANT|      in operator -"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        bool wasFound = false;
        for(int i = 0; i < (int)temp.l->size() && !wasFound; i++){
            if(this->l->at(i) == sub){
                temp.deleteL(i);
                wasFound = true;
            }
        }
        assert("|INVARIANT|      in operator - for temp"&&temp.biGraphIsCorrect());
        return temp;
    }

    template <typename T,typename V>
    BiGraph<T,V> BiGraph<T,V>::operator -(GraphNode<V,T>* const sub) const{
        assert("|INVARIANT|      in operator -"&&biGraphIsCorrect());
        BiGraph<T,V> temp(*this);
        bool wasFound = false;
        for(int i = 0; i < (int)temp.r->size() && !wasFound; i++){
            if(this->r->at(i) == sub){
                delete temp.deleteR(i);
                wasFound = true;
            }
        }
        assert("|INVARIANT|      in operator - for temp"&&temp.biGraphIsCorrect());
        return temp;
    }


    template <typename T,typename V>
    bool BiGraph<T,V>::isSubGraphOf(const BiGraph<T,V>& mother)const{
        assert("|INVARIANT|      in isSubGraphOf"&&biGraphIsCorrect());
        bool allLEqual = true;
        for(int i = 0; i < (int)this->l->size() && allLEqual; i++){
            bool wasFindEqual = false;
            for(int j = 0; j < (int) mother.l->size() && !wasFindEqual; j++)
                wasFindEqual = (this->l->at(i)->isSubGraphNodeOf(*(mother.l->at(j))));
            allLEqual &= wasFindEqual;
        }
        if(allLEqual){
            bool allREqual = true;
            for(int i = 0; i < (int)this->r->size() && allREqual; i++){
                bool wasFindEqual = false;
                for(int j = 0; j < (int) mother.r->size() && !wasFindEqual; j++)
                    wasFindEqual = (this->r->at(i)->isSubGraphNodeOf(*(mother.r->at(j))));
                allREqual &= wasFindEqual;
            }
            return allREqual;
        }
        else
            return false;
    }

    template <typename T,typename V>
    bool BiGraph<T,V>::biGraphIsCorrect() const{
        bool correct = l && r;
        for(int i = 0; i < (int)l->size() && correct; i++)
            correct &= l->at(i) && l->at(i)->graphNodeIsCorrect();
        if(correct)
            for(int i = 0; i < (int)r->size() && correct; i++)
                correct &= r->at(i) && r->at(i)->graphNodeIsCorrect();
        return correct;
    }

    template <typename T,typename V>
    ostream& operator << (ostream& stream,const BiGraph<T,V>& graph){
        for(int i = 0; i < (int)graph.l->size();i++){
            stringstream sstr;
            sstr << "l" << i;
            stream << sstr.str();
            for(int j = 0; j < (int)graph.l->at(i)->getLinks()->size(); j++){
                stream << out(j,sstr.str().length()) << "r" << j << std::endl;
            }
            if(graph.l->at(i)->getLinks()->size() == 0)
                stream << std::endl;
            stream << std::endl;
        }
        for(int i = 0; i < (int)graph.l->size(); i++)
        {
            if(typeid(graph.l->at(i)->getNode()).name()[0]=='P')
                stream << "l" << i << ": " << (*(graph.l->at(i)->getNode())) << std::endl;
            else
                stream << "l" << i << ": " << graph.l->at(i)->getNode() << std::endl;
        }
        for(int i = 0; i < (int)graph.r->size(); i++)
                stream << "r" << i << ": " << graph.r->at(i)->getNode() << std::endl;
        return stream;
    }
}
