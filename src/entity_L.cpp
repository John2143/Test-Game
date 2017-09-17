#ifndef entityL_H
#define entityL_H

#include "entity.h"
#include "luaLoader.h"

class Entity_L: public LunaClass<Entity>{
public:
    static const char className[];
    static typename Luna<Entity_L>::RegType Register[];

    Entity_L(lua_State *L){
        printf("made p %p\n", this);
        uid pid = lua_tonumber(L, -1);
        proxy = new Entity(pid);
    }
    ~Entity_L(){
        delete proxy;
    }
    int move(lua_State *L){
        position x = lua_tonumber(L, -2);
        position y = lua_tonumber(L, -1);
        proxy->move(x, y);
        lua_pop(L, 2);
        return 1;
    }

    int setPos(lua_State *L){
        proxy->setPos(lua_tonumber(L, -2), lua_tonumber(L, -1));
        lua_pop(L, 2);
        return 1;
    }

    int setHealth(lua_State *L){
        proxy->setHealth(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    int changeHealth(lua_State *L){
        proxy->changeHealth(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return 1;
    }
    int setAbility(lua_State *L){
        proxy->setAbility(lua_tonumber(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    int getMovespeed(lua_State *L){
        lua_pop(L, 1);
        lua_pushnumber(L, proxy->getMovespeed());
        return 1;
    }

    int getMaxHealth(lua_State *L){
        lua_pop(L, 1);
        lua_pushnumber(L, proxy->getMaxHealth());
        return 1;
    }
    int getMaxAbility(lua_State *L){
        lua_pop(L, 1);
        lua_pushnumber(L, proxy->getMaxAbility());
        return 1;
    }
    int getRegenHealth(lua_State *L){
        lua_pop(L, 1);
        lua_pushnumber(L, proxy->getRegenHealth());
        return 1;
    }
    int getRegenAbility(lua_State *L){
        lua_pop(L, 1);
        lua_pushnumber(L, proxy->getRegenAbility());
        return 1;
    }

    int getName(lua_State *L){
        lua_pushstring(L, proxy->getName().c_str());
        return 1;
    }
    int grantAI(lua_State *L){
        proxy->grantAI((Entity::AI) lua_tonumber(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    int kill(lua_State *L){
        (void) L;
        proxy->kill();
        return 1;
    }
    int spawn(lua_State *L){
        (void) L;
        proxy->spawn();
        return 1;
    }
    int unspawn(lua_State *L){
        (void) L;
        proxy->unspawn();
        return 1;
    }
    int setControlled(lua_State *L){
        (void) L;
        setControlledEntity(proxy);
        return 1;
    }
    int getFacing(lua_State *L){
        lua_pushnumber(L, proxy->facing);
        return 1;
    }
};

const char Entity_L::className[] = "Entity";
Luna<Entity_L>::RegType Entity_L::Register[] = {
    {"move", &Entity_L::move},
    {"setPos", &Entity_L::setPos},
    {"setHealth", &Entity_L::setHealth},
    {"changeHealth", &Entity_L::changeHealth},
    {"setAbility", &Entity_L::setAbility},
    {"getMovespeed", &Entity_L::getMovespeed},
    {"getMaxHealth", &Entity_L::getMaxHealth},
    {"getMaxAbility", &Entity_L::getMaxAbility},
    {"getRegenHealth", &Entity_L::getRegenHealth},
    {"getRegenAbility", &Entity_L::getRegenAbility},
    {"getName", &Entity_L::getName},
    {"grantAI", &Entity_L::grantAI},
    {"kill", &Entity_L::kill},
    {"spawn", &Entity_L::spawn},
    {"unspawn", &Entity_L::unspawn},
    {"setControlled", &Entity_L::setControlled},
    {"facing", &Entity_L::getFacing},
    {NULL, NULL}
};

#endif
