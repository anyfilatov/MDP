#pragma once
#include <string>
#include <sstream>

namespace util{
    
    template<typename T>
    std::string concat_(const T& s) {
        std::stringstream ss;
        ss << s;
        return ss.str();
    }
    
    template<typename T1, typename... T> 
    std::string concat_(const T1& s, const T&... a) {
        std::stringstream ss;
        ss << s << concat_(a...);
        return ss.str();
    }
    
    
    
    template<typename... T> 
    std::string concat(const T&... a) {
        return concat_(a...);
    }
    
    class Id {
    public:
        Id() = default;
        Id(const Id& ) = default;
        Id( Id&& ) = default;
        Id& operator = (const Id& ) = default;
        Id& operator = ( Id&& ) = default;
        enum IdIndex{
            userIdIndex,
            scriptIdIndex,
            dataIdIndex
        };
        int get(IdIndex i) {
            switch(i) {
                case userIdIndex: return i0;
                case scriptIdIndex: return i1;
                case dataIdIndex: return i2;
            }
            return i2;
        }
        
        void set(IdIndex i, int newValue) {
            switch(i) {
                case userIdIndex: 
                    i0 = newValue;
                    break;
                case scriptIdIndex: 
                    i1 = newValue;
                    break;
                case dataIdIndex: 
                    i2 = newValue;
                    break;
            }
        }
        
        int i0;
        int i1;
        int i2;
    };
}