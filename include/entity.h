#ifndef entity_H
#define entity_H

#include "global.h" //asset folder
#include "inventory.h"

struct stats{
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
    textureID textureID;
    struct stats stats; //Default stats of a entity
    const char *name;
    int scale;
};

typedef struct entity{
    uid globalid;
    uid parentid;

    GLuint textureID;
    //size_t textureOffset;

    position x, y;
    int w, h;
    angle facing;
    int hp, abi;
    struct stats stats; //Customs stats: modified at spawn time

    struct AIData *ai;
    char *name;

    struct inventory *inventory;

    struct entity *next;
    struct entity *last;
} *pent;
//I normally don't typdef structs, but I have a feeling struct entity * will be used a lot.

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
int getEntityMaxHealth(pent e); //NL
int getEntityMaxAbility(pent e); //NL

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
