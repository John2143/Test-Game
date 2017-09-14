#include "lua.h"

lua_State *globalLuaState;

void luaStart(){
    lua_State *L = globalLuaState = luaL_newstate();
    luaL_openlibs(L);



    if(luaL_dofile(L, "lua/start.lua")){
        printf("Failed to start lua: %s\n", lua_tostring(L, -1));
    }else{
        printf("Lua started successfully!\n");
    }
}

void luaEnd(){
    //Do errors really matter here?
    (void) luaL_dofile(globalLuaState, "lua/end.lua");
    lua_close(globalLuaState);
}
