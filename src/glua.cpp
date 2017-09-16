#include "glua.h"
#include "entity_L.cpp"

lua_State *L;

int callLuaGameFunc(const char *name, int nargs, int nret){
    //Get the function from the game table
    lua_getfield(L, -1, name);
    if(!lua_isfunction(L, -1)){
        printf("%s is not defined, but was called\n", name);
        return 1;
    }

    //Push a copy of the game table so it can use 'self' (pcall will consume it)
    lua_pushvalue(L, 1);
    //move all the passed args to the top
    int args = nargs;
    while(args--) lua_insert(L, 2);

    int ret = lua_pcall(L, nargs + 1, nret, 0);
    //lua_pop(L, 1);
    return ret;
}

//Copied from PIL 25.2
void luaStackDump(){
    int i;
    int top = lua_gettop(L);
    for (i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(L, i);
        switch (t) {
            case LUA_TSTRING:  /* strings */
                printf("`%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:  /* booleans */
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:  /* numbers */
                printf("%g", lua_tonumber(L, i));
                break;
            default:  /* other values */
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("  ");  /* put a separator */
    }
    printf("\n");  /* end the listing */
    fflush(stdin);
}

const static size_t assetFolderPathSize = strlen(assetFolderPath);

//static textureID loadTexture(const char *name); //Defined in graphics.c
static int luaLoadTexture(lua_State *L){
    const char *name = lua_tostring(L, -1);
    size_t len = strlen(name);
    char *buf = new char[len + assetFolderPathSize + 1];
    strcpy(buf, assetFolderPath);
    strcat(buf, name);

    textureID id = loadTexture(buf);
    delete[] buf;

    lua_pushnumber(L, (int) id);
    return 1;
}

int luaStart(){
    L = luaL_newstate();
    luaL_openlibs(L);

    //Define globals
    Luna<Entity_L>::Register(L);
    lua_pushcfunction(L, luaLoadTexture);
    lua_setglobal(L, "loadTexture");

    //This game.cpp should return a table with functions (the game table)
    //The returned table will be on the top of the stack
    if(luaL_dofile(L, "lua/game.lua")){
        printf("Lua failed to compile: %s\n", lua_tostring(L, -1));
        return 1;
    }

    if(!lua_istable(L, -1)){
        printf("game.lua did not return a table\n");
        return 1;
    }

    return 0;
}

int luaEnd(){
    lua_close(L);
    return 0;
}
