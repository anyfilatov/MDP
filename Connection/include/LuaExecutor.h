#pragma once

#include <iostream>
#include <lua.hpp>
#include <lua.h>
#include "Logger.h"

extern "C" {
  static int l_cppfunction(lua_State *L) {
    double arg = luaL_checknumber(L,1);
    lua_pushnumber(L, arg * 0.5);
    return 1;
  }
}


class LuaExecutor{
public:
    LuaExecutor() {
        luaState_ = luaL_newstate();
    }
    int exec() {
        LOG_DEBUG("** Load the (optional) standard libraries, to have the print function");
        luaL_openlibs(luaState_);
        LOG_DEBUG("** Load chunk. without executing it");
        if (luaL_loadfile(luaState_, "luascript.lua")) {
            LOG_DEBUG("Something went wrong loading the chunk (syntax error?)");
            LOG_DEBUG(lua_tostring(luaState_, -1));
            lua_pop(luaState_, 1);
        }

        LOG_DEBUG("** Make a insert a global var into Lua from C++");
        lua_pushnumber(luaState_, 1.1);
        lua_setglobal(luaState_, "cppvar");

        LOG_DEBUG("** Execute the Lua chunk");
        if (lua_pcall(luaState_, 0, LUA_MULTRET, 0)) {
            LOG_DEBUG("Something went wrong during execution");
            LOG_DEBUG(lua_tostring(luaState_, -1));
            lua_pop(luaState_, 1);
        }

        LOG_DEBUG("** Read a global var from Lua into C++");
        lua_getglobal(luaState_, "luavar");
        double luavar = lua_tonumber(luaState_, -1);
        lua_pop(luaState_, 1);
        LOG_DEBUG("C++ can read the value set from Lua luavar = " << luavar);

        LOG_DEBUG("** Execute a Lua function from C++");
        lua_getglobal(luaState_, "myluafunction");
        lua_pushnumber(luaState_, 5);
        lua_pcall(luaState_, 1, 1, 0);
        LOG_DEBUG("The return value of the function was " << lua_tostring(luaState_, -1));
        lua_pop(luaState_, 1);

        LOG_DEBUG("** Execute a C++ function from Lua");
        LOG_DEBUG("**** First register the function in Lua");
        lua_pushcfunction(luaState_, l_cppfunction);
        lua_setglobal(luaState_, "cppfunction");

        LOG_DEBUG("**** Call a Lua function that uses the C++ function");
        lua_getglobal(luaState_, "myfunction");
        lua_pushnumber(luaState_, 5);
        lua_pcall(luaState_, 1, 1, 0);
        LOG_DEBUG("The return value of the function was " << lua_tonumber(luaState_, -1));
        lua_pop(luaState_, 1);

        LOG_DEBUG("** Release the Lua enviroment");
        lua_close(luaState_);
        return 0;
    }
    ~LuaExecutor() {
        lua_close(luaState_);
    }
private:
    lua_State *luaState_;
};