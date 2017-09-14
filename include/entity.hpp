#ifndef entity_H
#define entity_H

#include <vector>
#include "global.h"
#include "inventory.h"
#include "luaLoader.hpp"
#include "logic.h"

class Entity{
public:
    enum AI{
        AI_NONE,
        AI_WANDER,
        AI_CHASE,
    };

    struct AIData{
        enum AI currentMethod;
    } *ai;

    struct coreStats{
        int def, agi, vit, abi;
    } stats;

    struct entityData{
        textureID tid;
        Entity::coreStats stats; //Default stats of a entity
        const char *name;
        int scale;
    };

    static unsigned int currentEntityID;

    uid globalid;
    uid parentid;

    textureID tid;

    position x, y;
    int w, h;
    angle facing;
    stattype hp, abi;

    char *name;

    inventory *inv;

    static void loadData();
    static void unloadData();

    Entity(uid pid);
    ~Entity();

    invError useItem(int slot);
    void setSize(int scale);

    void move(position x, position y);
    void moveAng(angle ang, double delta);
    void setPos(position x, position y);

    void setHealth(int hp);
    void changeHealth(int hp);
    void setAbility(int abi);

    int getMovespeed();

    stattype getMaxHealth();
    stattype getMaxAbility();
    stattype getRegenHealth();
    stattype getRegenAbility();

    const char *getName();
    void grantAI(enum AI method);

    void kill();
    void spawn();
    void unspawn();
};

extern std::vector<Entity> worldEntities;
extern void setControlledEntity(Entity *e);

class Entity_L: public LunaClass<Entity>{
public:
    static const char className[];
    static typename Luna<Entity_L>::RegType Register[];

    Entity_L(lua_State *L){
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
        lua_pushstring(L, proxy->getName());
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
        setControlledEntity(proxy);
        return 1;
    }
};

#endif
