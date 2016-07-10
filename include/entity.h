#ifndef entity_H
#define entity_H

#include <math.h> //sin, cos
#include <stdlib.h> //malloc etc

#include "global.h" //asset folder
GLuint loadTexture(const char *name);

struct stats{
    int def, agi, vit, abi;
};

enum AI{
    AI_NONE,
    AI_WANDER,
    AI_CHASE,
    AI_BULLET,
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
    int globalid;
    int parentid;

    GLuint textureID;
    //size_t textureOffset;

    double x, y;
    int w, h;
    double facing;
    int hp, abi;
    struct stats stats; //Customs stats: modified at spawn time

    struct AIData *ai;
    char *name;

    struct entity *next;
    struct entity *last;
} *pent;
//I normally don't typdef structs, but I have a feeling struct entity * will be used a lot.

struct worldLinkedList{
    pent first;
};

extern struct worldLinkedList worldEntities;

void spawnEntity(pent e);
void unspawnEntity(pent e);

pent newEntity(int parentid);
pent newEntityShell(int parentid, pent e);
void deleteEntity(pent e);
void moveEntity(pent e, double x, double y);
void setEntityPos(pent e, double x, double y);
void moveEntityAng(pent e, double ang, double del);
void killEntity(pent e);

void setEntityHealth(pent e, int hp);
void setEntityAbility(pent e, int abi); //nl

int getEntityMovespeed(pent e);
int getEntityMaxHealth(pent e); //NL
int getEntityMaxAbility(pent e); //NL

void setEntitySize(pent e, int scale);
void embiggenEntity(pent e);
const char *getName(pent e);

pent findClosestEntity(pent to, int type);

void grantAI(pent e, enum AI method);

void loadEntities();
void unloadEntities();

#endif
