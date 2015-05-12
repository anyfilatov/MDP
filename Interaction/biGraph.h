#pragma once
#include "GraphNode.h"
#include "allocator.h"
#include "abstractClient.h"

namespace integration {
    template <typename T, typename V>
    class BiGraph{
    protected:
        std::vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >* l;
        std::vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >* r;
    public:
        BiGraph();

        void addL(T lN);
        void addR(V rN);
        void setLinks(char part, int from, const std::vector<int>& to);

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
    };

    template <typename T, typename V>
    BiGraph<T,V>::BiGraph(){
        l = new std::vector<GraphNode<T,V>*,Allocator<GraphNode<T,V>*> >();
        r = new std::vector<GraphNode<V,T>*,Allocator<GraphNode<V,T>*> >();
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
    GraphNode<V,T>& BiGraph<T,V>::getR(int i){
        return *(r->at(i));
    }

    template <typename T,typename V>
    typename BiGraph<T,V>::LeftIteratorBiGraph BiGraph<T,V>::getIteratorL(){
        BiGraph<T,V>::LeftIteratorBiGraph ans(0,l);
        return ans;
    }

    template <typename T,typename V>
    typename BiGraph<T,V>::RightIteratorBiGraph BiGraph<T,V>::getIteratorR(){
        BiGraph<T,V>::RightIteratorBiGraph ans(0,r);
        return ans;
    }
}
