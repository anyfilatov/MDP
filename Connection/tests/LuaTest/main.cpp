#include "LuaExecutor.h"
#include "Logger.h"
#include <tuple>
#include <functional>
#include <list>

using namespace std;

std::string script = "function printVar(var) print (var) end";
struct Id {
    Id() = default;
    Id(const Id&) = default;
    Id(Id&&) = default;
    Id& operator = (const Id&) = default;
    Id& operator = (Id&&) = default;
    int u0;
    int u1;
    int u2;
    template<int>
    int& get();
    template<int>
    const int& get()const ;
    bool operator < (const Id& id);
};
template<>
int& Id::get<0>(){return u0;};
template<>
int& Id::get<1>(){return u1;};
template<>
int& Id::get<2>(){return u2;};

template<>
const int& Id::get<0>()const{return u0;};
template<>
const int& Id::get<1>()const{return u1;};
template<>
const int& Id::get<2>()const{return u2;};

bool Id::operator < (const Id& id)
{
    return std::tie(get<0>(), get<1>(), get<2>()) < 
            std::tie(id.get<0>(), id.get<1>(), id.get<2>());
}

struct Cortege {
    std::string s0;
    std::string s1;
    Cortege() = default;
    Cortege(const Cortege&) = default;
    Cortege(Cortege&&) = default;
    Cortege& operator = (const Cortege&) = default;
    Cortege& operator = (Cortege&&) = default;
    template<int>
    std::string& get();
    template<int>
    const std::string& get()const;
    bool operator < (const Cortege& id) const;
};

template<>
std::string& Cortege::get<0>() {return s0;};
template<>
std::string& Cortege::get<1>() {return s1;};

template<>
const std::string& Cortege::get<0>() const {return s0;};
template<>
const std::string& Cortege::get<1>() const {return s1;};

bool Cortege::operator < (const Cortege& id) const {
    return std::tie(get<0>(), get<1>()) < 
            std::tie(id.get<0>(), id.get<1>());
}
typedef std::map<int, std::vector<std::string>> MapDb;
typedef std::map<int, std::map<std::string, std::list<std::string>>> ReduceDb;
MapDb mapDb;
ReduceDb reduceDb;
//std::map<int, std::vector<int>> mapDb;
//std::map<int, std::map<int, std::list<int>>> reduceDb;

typedef std::tuple<std::string, std::string>(*Map)(std::string);
//typedef function<std::tuple<std::string, std::string>(std::string)> Map;

//int doMap(Map map, int data) {
//    LOG_DEBUG("data=" << data);
//    int nextId(data);
//    for(auto& word : mapDb[data]) {
//        std::string key, value;
//        LOG_DEBUG("before map");
//        std::tie(key, value) = map(word);
//        LOG_DEBUG("key=" << key << " value=" << value);
//        reduceDb[nextId][key].push_back(atoi(value.c_str()));
////        reduceDb[nextId][get<0>(res)].push_back(atoi(get<1>(res).c_str()));
////        reduceDb[nextId][res].push_back(1);
//    }
//    return data;
//}

int doMap(sel::function<std::tuple<std::string, std::string>(const char*)> fun, int a) {
#warning "Check error for stack"
    fun( "1");
    for(auto& word : mapDb[a]) {
        LOG_DEBUG("word=" << word);
        auto res = fun( word.c_str());
        reduceDb[a][get<0>(res)].push_back(get<1>(res));
//        LOG_DEBUG( " fun=" <<  get<0>(res) << "," << get<1>(res) );
    }
    return a;
}

//
//class doMap{
//    
//    (function<std::string(std::string)> map, int data) {
//    int nextId(data);
//    for(auto& word : mapDb[data]) {
//        auto res = map(word);
////        reduceDb[nextId][get<0>(res)].push_back(atoi(get<1>(res).c_str()));
//        reduceDb[nextId][res].push_back(1);
//    }
//    return data;
//};

const char** getArray(list<string>& l) {
    auto** res = new const char*[l.size()];
    const char* it = *res;
    for(auto& e : l ) {
        it = e.c_str();
        it++;
    }
    return res;
}

sel::State exec{true};

typedef sel::Tuple<std::string, std::string> StringTuple;
int doReduce(sel::function<std::tuple<std::string, std::string>(const char*, const char*)> fun, int t) {
    int i = 0;
    fun("", "");
    for(auto& a : reduceDb[t]){
        auto res = fun(a.first.c_str(), "");
        LOG_DEBUG( "size=" <<  get<0>(res).size() << "," << get<1>(res).size() );
        LOG_DEBUG( "reduce=" <<  get<0>(res) << "," << get<1>(res) );
    }
    return ++t; 
}

int main(int , char *[])
{
    mapDb[1] = std::vector<std::string>{"word1", "word2", "word3", "word1"};
//    mapDb[1] = std::vector<int>{1, 2, 3, 4, 5};
    std:: tuple<int, int, int>  id {1, 1, 2};
    //exec.InteractiveDebug();
    exec["in0"] = 1;
    exec["doMap"] = &doMap;
//    exec["in1"] = 1;
    exec["doReduce"]  = &doReduce;
//    exec(script.c_str());
    //exec.Push(exec["data1"]);
    LOG_DEBUG("before");
    if( !exec.Load("script.lua") ) {
        LOG_ERROR("script load failed");
    }
    LOG_DEBUG("after");
//    exec["printVar"](1);
}
