#pragma once

extern "C"
{
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}



class Script
  {
private:
    lua_State *lua_state;		

public:
    void Create();						
    void Close();						
    int DoString(char* scriptCode);				
    template<class T>
    void RegisterConstant(T value, char* constantname);		

    void Array();									
    template<class T>
    void RegisterConstantArray(T value, int index);			
    void RegisterArray(char* arrayname);				
									
    int GetArgumentCount();						
    template<class T>
    T GetArgument(int index);					
    template<class T>
    void Return(T value);						
					
};						
