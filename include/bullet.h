#ifndef bullet_H
#define bullet_H

#include "global.h"
#include "entity.h"

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

    position x, y;
    angle ang;
    framerate ctime;
    uid dataid;

    struct bullet *next;
    struct bullet *last;
} *pbull;

extern struct bulletData *bulletDatas;
extern pbull worldBullets;

pbull createBullet(uid dataid, framerate appTime, pent owner, angle ang);
void removeBullet(pbull e);

void initializeBullets();
void uninitializeBullets();

void tickBullets();
bool bulletHitboxTouching(pent e, pbull b);
void addNoCollideEntToBullet(uid id, pbull b);
bool shouldBulletAndEntCollide(pent e, pbull b);

#endif
