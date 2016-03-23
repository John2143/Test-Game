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

struct entityData{
    GLuint textureID;
    struct stats stats; //Default stats of a entity
    const char *name;
};

typedef struct entity{
    int globalid;

    GLuint textureID;
    //size_t textureOffset;

    double x, y;
    double facing;
    int hp;
    struct stats stats; //Customs stats: modified at spawn time

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

pent newEntity();
void deleteEntity(pent e);
void moveEntity(pent e, double x, double y);
void setEntityPos(pent e, double x, double y);
void moveEntityAng(pent e, double ang, double del);
void killEntity(pent e);

void loadEntities();
void unloadEntities();

#endif
