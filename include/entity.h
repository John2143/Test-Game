#ifndef entity_H
#define entity_H

#include <math.h>
#include <GL/GLU.h> //for GLuint textures
#include <stdlib.h>

struct stats{
    int def, agi, hp;
};

struct entityData{
    GLuint textureID;
    struct stats stats; //Default stats of a entity
    const char *const name;
};

enum entityID{
    ENTID_NONE = 0,
};

typedef struct entity{
    enum entityID id;
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
//extern struct entityData defaultEntites;

void spawnEntity(pent e);
void unspawnEntity(pent e);

pent newEntity();
void deleteEntity(pent e);
void moveEntity(pent e, double x, double y);
void setEntityPos(pent e, double x, double y);
void moveEntityAng(pent e, double ang, double del);

#endif
