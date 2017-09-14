#ifndef entity_H
#define entity_H

#include "global.h" //asset folder
#include "inventory.h"

struct stats{
    //These should not be of type 'stattype'
    int def, agi, vit, abi;
};

enum AI{
    AI_NONE,
    AI_WANDER,
    AI_CHASE,
};

struct AIData{
    enum AI currentMethod;
};

struct entityData{
    textureID tid;
    struct stats stats; //Default stats of a entity
    const char *name;
    int scale;
};

typedef struct entity{
    uid globalid;
    uid parentid;

    textureID tid;
    //size_t textureOffset;

    position x, y;
    int w, h;
    angle facing;
    stattype hp, abi;
    struct stats stats; //Customs stats: modified at spawn time

    AIData *ai;
    char *name;

    inventory *inv;

    entity *next;
    entity *last;
} *pent;

//I normally don't typdef structs, but I have a feeling struct entity * will be used a lot.
//TODO nl = not implement in lua

extern pent worldEntities;

void spawnEntity(pent e);
void unspawnEntity(pent e);

pent newEntity(uid parentid);
pent newEntityShell(uid parentid, pent e);
void deleteEntity(pent e);
void moveEntity(pent e, position x, position y);
void setEntityPos(pent e, position x, position y);
void moveEntityAng(pent e, angle ang, double del);
void killEntity(pent e);

void setEntityHealth(pent e, int hp);
void changeEntityHealth(pent e, int hp); //nl
void hurtEntity(pent e, int hp); //nl
void setEntityAbility(pent e, int abi); //nl

int getEntityMovespeed(pent e);
stattype getEntityMaxHealth(pent e); //NL
stattype getEntityMaxAbility(pent e); //NL

//Returns regen per second
stattype getEntityRegenHealth(pent e); //NL
stattype getEntityRegenAbility(pent e); //NL

void setEntitySize(pent e, int scale);
void embiggenEntity(pent e);
const char *getName(pent e);

pent findClosestEntity(pent to, uid type);

void grantAI(pent e, enum AI method);
int entityUseItem(pent e, int slot);

void loadEntities();
void unloadEntities();

extern pent controlledEntity;
extern void setControlledEntity(pent e);

#endif
