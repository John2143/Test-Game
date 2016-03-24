#include "lua.h"

lua_State *globalLuaState;

/*#define HELLO "Hello, "*/
/*static int luaTestFunction(lua_State *L){*/
    /*const char *c = luaL_checkstring(L, 1);*/
    /*size_t length = strlen(c) + strlen(HELLO);*/
    /*char *newString = malloc(strlen(c) + strlen(HELLO));*/
    /*strcpy(newString, HELLO);*/
    /*strcat(newString, c);*/
    /*lua_pushlstring(L, newString, length);*/
    /*free(newString);*/
    /*return 1;*/
/*}*/

static int luaNewEntity(lua_State *L){
    pent e = newEntityShell((int) luaL_checknumber(L, 1), lua_newuserdata(L, sizeof(*e)));
    luaL_getmetatable(L, "entity");
    lua_setmetatable(L, -2);
    return 1;
}

static int luaSetControlledEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    setControlledEntity(e);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaSpawnEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    spawnEntity(e);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaUnspawnEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    unspawnEntity(e);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaDeleteEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    deleteEntity(e);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaMoveEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    double x = luaL_checknumber(L, 1);
    double y = luaL_checknumber(L, 1);
    moveEntity(e, x, y);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaSetEntityPos(lua_State *L){
    pent e = lua_touserdata(L, 1);
    double x = luaL_checknumber(L, 2);
    double y = luaL_checknumber(L, 3);
    setEntityPos(e, x, y);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaKillEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    killEntity(e);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaGetEntityMovespeed(lua_State *L){
    pent e = lua_touserdata(L, 1);
    lua_pushinteger(L, getEntityMovespeed(e));
    return 1;
}

static int luaSetEntitySize(lua_State *L){
    pent e = lua_touserdata(L, 1);
    setEntitySize(e, (int) luaL_checknumber(L, 2));
    lua_pushvalue(L, 1);
    return 1;
}

static int luaEmbiggenEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    embiggenEntity(e);
    lua_pushvalue(L, 1);
    return 1;
}

static int luaFindClosestEntity(lua_State *L){
    pent e = lua_touserdata(L, 1);
    int type = (int) luaL_checknumber(L, 2);
    pent ret = findClosestEntity(e, type);
    //TODO ????
    return 1;
}

static int luaGrantAI(lua_State *L){
    pent e = lua_touserdata(L, 1);
    enum AI type = (int) luaL_checknumber(L, 2);
    grantAI(e, type);
    lua_pushvalue(L, 1);
    return 1;
}

void registerAPIFunction(lua_State *L, const char *name, lua_CFunction func){
    lua_pushstring(L, name);
    lua_pushcfunction(L, func);
    lua_settable(L, -3);
}

void startNewMetatable(lua_State *L, const char *name){
    luaL_newmetatable(L, name);
    lua_pushstring(L, "__index");
    lua_newtable(L); //Unknown number of API funcs
}
void endNewMetatable(lua_State *L){
    lua_settable(L, -3);
}

void luaStart(){
    lua_State *L = globalLuaState = luaL_newstate();
    luaL_openlibs(L);
    /*lua_register(L, "helloWorld", luaTestFunction);*/

    startNewMetatable(L, "entity");
        registerAPIFunction(L, "setControlled", luaSetControlledEntity);
        registerAPIFunction(L, "spawn", luaSpawnEntity);
        registerAPIFunction(L, "unspawn", luaUnspawnEntity);
        registerAPIFunction(L, "delete", luaDeleteEntity);
        registerAPIFunction(L, "move", luaMoveEntity);
        registerAPIFunction(L, "setPos", luaSetEntityPos);
        registerAPIFunction(L, "kill", luaKillEntity);
        registerAPIFunction(L, "getMovespeed", luaGetEntityMovespeed);
        registerAPIFunction(L, "setSize", luaSetEntitySize);
        registerAPIFunction(L, "embiggen", luaEmbiggenEntity);
        registerAPIFunction(L, "findClosestEntity", luaFindClosestEntity);
        registerAPIFunction(L, "grantAI", luaGrantAI);
    endNewMetatable(L);
    /*registerAPIFunction(L, "__gc", )*/


    lua_register(L, "newEntity", luaNewEntity);

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
