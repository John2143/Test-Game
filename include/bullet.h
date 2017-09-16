#ifndef bullet_H
#define bullet_H

#include "global.h"
#include "entity.h"
#include "inventory.h"

enum bulletFlags{
    BFLAG_PIERCE = 1 << 0,
    BFLAG_MELEE = 1 << 1,
};

struct bulletData{
    int speed;
    int damage;
    framerate lifetime;
    textureID texture;
    int w, h;
    int flags;
};

typedef struct bullet{
    uid globalid;

    uid *nocollide;
    int nocollideAmount;
    int nocollideAmountMax;

    int damage;

    position x, y;
    angle ang;
    framerate ctime;
    uid dataid;

    struct bullet *next;
    struct bullet *last;
} *pbull;

extern struct bulletData *bulletDatas;
extern pbull worldBullets;

pbull createBullet    (uid dataid, framerate appTime, Entity &owner,           angle ang);
pbull createItemBullet(uid dataid, framerate appTime, Entity &owner, pitem it, angle ang);
void removeBullet(pbull e);

void initializeBullets();
void uninitializeBullets();

void tickBullets();
bool bulletHitboxTouching(Entity &e, pbull b);
void addNoCollideEntToBullet(uid id, pbull b);
bool shouldBulletAndEntCollide(Entity &e, pbull b);

#endif
