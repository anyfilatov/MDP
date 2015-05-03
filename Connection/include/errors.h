#pragma once
#include <exception>

namespace Errors{
    static const int STATUS_OK = 0;
    static const int STATUS_COMPILATION_ERROR = 1;
    static const int STATUS_ERROR = 2;
    static const int STATUS_MEMORY_ALLOCATION_ERROR = 3;
    static const int STATUS_UNEXPECTED_USER_VALUE = 4;
    
    static inline std::string getErrorTextForUser(int error, const std::string& additionalString = "") {
        
        std::string text;
        switch(error) {
            case STATUS_OK:
                text = "Status:OK";
                break;
            case STATUS_COMPILATION_ERROR:
                text = "Lua compilation error:";
                break;
            default:
                break;
        }
        return text + additionalString;
    }
    
    static inline std::string getErrorTextForLog(int error, const std::string& additionalString = "") {
        return getErrorTextForUser(error, additionalString);
    }
}

class LuaExecutionExeption : public std::exception {
public:
    LuaExecutionExeption(std::string& val) : err_(val){}
    LuaExecutionExeption(std::string& val, int err) : err_(Errors::getErrorTextForUser(err, val)){}
    LuaExecutionExeption(int err) : err_(Errors::getErrorTextForUser(err)){}
    const char* what() const noexcept {
        return err_.c_str();
    }
private:
    std::string err_;
};

class LuaAllocationException : public LuaExecutionExeption {
public:
    
};