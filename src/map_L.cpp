#ifndef map_L_H
#define map_L_H

#include "map.h"
#include "luaLoader.h"

class Map_L: public LunaClass<World>{
public:
    static const char className[];
    static typename Luna<Map_L>::RegType Register[];

    Map_L(lua_State *L){
        size_t size = lua_tonumber(L, -1);
        proxy = new World(size);
    }
    ~Map_L(){
        delete proxy;
    }

    int setCurrent(lua_State *L){
        (void) L;
        proxy->makeCurrent();
        return 1;
    }
    int set(lua_State *L){
        size_t x = lua_tonumber(L, -3);
        size_t y = lua_tonumber(L, -2);
        tileid t = lua_tonumber(L, -1);
        proxy->set(x, y, t);
        lua_pop(L, 3);
        return 1;
    }
    int get(lua_State *L){
        size_t x = lua_tonumber(L, -2);
        size_t y = lua_tonumber(L, -1);
        lua_pop(L, 2);
        lua_pushnumber(L, proxy->get(x, y));
        return 1;
    }
};

const char Map_L::className[] = "Map";
Luna<Map_L>::RegType Map_L::Register[] = {
    {"setCurrent", &Map_L::setCurrent},
    {"set", &Map_L::set},
    {"get", &Map_L::get},
    {NULL, NULL}
};
#endif
