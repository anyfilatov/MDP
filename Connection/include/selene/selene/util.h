#pragma once

#include <iostream>
#include <sstream>
extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace sel {

    class CapturedStdout {
    public:

        CapturedStdout() {
            //_old = std::cout.rdbuf(_out.rdbuf());
            //_oldErr = std::cout.rdbuf(_err.rdbuf());
        }

        ~CapturedStdout() {
            //std::cout.rdbuf(_old);
            //std::cerr.rdbuf(_oldErr);
        }

        std::string getOut() const {
            return _out.str();
        }
        std::string getErr() const {
            return _err.str();
        }

    private:
        std::stringstream _out;
        std::streambuf *_old;
        std::stringstream _err;
        std::streambuf *_oldErr;
    };

inline std::ostream &operator<<(std::ostream &os, lua_State *l) {
    int top = lua_gettop(l);
    for (int i = 1; i <= top; ++i) {
        int t = lua_type(l, i);
        switch(t) {
        case LUA_TSTRING:
            os << lua_tostring(l, i);
            break;
        case LUA_TBOOLEAN:
            os << (lua_toboolean(l, i) ? "true" : "false");
            break;
        case LUA_TNUMBER:
            os << lua_tonumber(l, i);
            break;
        default:
            os << lua_typename(l, t);
            break;
        }
        os << " ";
    }
    return os;
}

inline void _print() {
    std::cout << std::endl;
}

template <typename T, typename... Ts>
inline void _print(T arg, Ts... args) {
    std::cout << arg << ", ";
    _print(args...);
}

inline bool check(lua_State *L, int code) {
#if LUA_VERSION_NUM >= 502
    if (code == LUA_OK) {
#else
    if (code == 0) {
#endif
        return true;
    } else {
        std::cout << lua_tostring(L, -1) << std::endl;
        return false;
    }
}

inline int ErrorHandler(lua_State *L) {
    // call debug.traceback
    lua_getglobal(L, "debug");
    lua_getfield(L, -1, "traceback");
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);

    // _print(<error-message> + call stack)
    const char* msg = "<not set>";
    if (!lua_isnil(L, -1)) {
        msg = lua_tostring(L, -1);
        if (!msg)
            msg = "<error object>";
    }
    _print(msg);
    return 1;
}

inline int SetErrorHandler(lua_State *L) {
    lua_pushcfunction(L, &ErrorHandler);
    return lua_gettop(L);
}
}
