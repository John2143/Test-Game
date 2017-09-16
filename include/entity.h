#ifndef entity_H
#define entity_H

#include <vector>
#include <string>
#include "global.h"
#include "inventory.h"
#include "logic.h"
#include "glua.h"

class Entity{
public:
    enum AI{
        AI_NONE = 0,
        AI_WANDER = 1,
        AI_CHASE = 2,
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
        std::string name;
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

    std::string name;

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

    std::string &getName();
    void grantAI(enum AI method);

    void kill();
    void spawn();
    void unspawn();
};

extern std::vector<Entity *> worldEntities;
extern void setControlledEntity(Entity *e);
#endif
