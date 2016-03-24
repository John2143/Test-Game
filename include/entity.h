#ifndef entity_H
#define entity_H

#include <math.h> //sin, cos
#include <GL/GLU.h> //for GLuint textures
#include <stdlib.h> //malloc etc

#include "global.h" //asset folder
#include "graphics.h" //loadTexture

struct stats{
    int def, agi, hp;
};

enum AI{
    AI_NONE,
    AI_WANDER,
    AI_CHASE,
};

struct entityData{
    GLuint textureID;
    struct stats stats; //Default stats of a entity
    const char *name;
    int scale;
};


typedef struct entity{
    int globalid;
    int parentid;

    GLuint textureID;
    //size_t textureOffset;

    int scale;
    double x, y;
    int w, h;
    double facing;
    int hp;
    struct stats stats; //Customs stats: modified at spawn time

    enum AI ai;
    char *name;

    struct entity *next;
} *pent;
//I normally don't typdef structs, but I have a feeling struct entity * will be used a lot.

struct worldLinkedList{
    pent first;
    pent last;
};

extern struct worldLinkedList worldEntities;

void spawnEntity(pent e);
void unspawnEntity(pent e);

pent newEntity(int parentid);
void deleteEntity(pent e);
void moveEntity(pent e, double x, double y);
void setEntityPos(pent e, double x, double y);
void moveEntityAng(pent e, double ang, double del);
void killEntity(pent e);

int getEntityMovespeed(pent e);
void setEntitySize(pent e, int scale);
void embiggenEntity(pent e);
const char *getName(pent e);

pent findClosestEntity(pent to, int type);

void loadEntities();
void unloadEntities();

#endif
