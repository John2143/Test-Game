#ifndef Bullet_L_H
#define Bullet_L_H

#include "bullet.h"
#include "entity_L.cpp"
#include "luaLoader.h"

class Bullet_L: public LunaClass<Bullet>{
public:
    static const char className[];
    static typename Luna<Bullet_L>::RegType Register[];

    Bullet_L(lua_State *L){
        uid pid = lua_tonumber(L, -3);
        Entity_L *udata = *(Entity_L **) lua_touserdata(L, -2);
        Entity *owner = udata->proxy;

        angle ang = lua_tonumber(L, -1);
        proxy = new Bullet(pid, *owner, ang);
    }
    ~Bullet_L(){
        //Bullets deconstruct themselves
    }
};

const char Bullet_L::className[] = "Bullet";
Luna<Bullet_L>::RegType Bullet_L::Register[] = {
    {NULL, NULL}
};

#endif
