#ifndef bullet_H
#define bullet_H

#include "global.h"
#include "entity.h"
#include "inventory.h"

#include <cmath>
#include <vector>
#include <algorithm>

class Bullet{
public:
    enum bulletFlags{
        BFLAG_PIERCE = 1 << 0,
    };

    static struct bulletData{
        int speed;
        int damage;
        framerate lifetime;
        textureID texture;
        int w, h;
        uint_least8_t flags;
    } *bulletDatas;

    static void initializeBullets();
    static void uninitializeBullets();

    uid globalid;
    uid parentid;
    std::vector<uid> nocollide;

    int damage;

    position x, y;
    angle ang;
    framerate createTime;

    Bullet(uid pid, Entity &owner, angle ang);
    ~Bullet();

    void tick();
    static void tickAll();

    bool hitboxTouching(Entity &e);
    bool shouldCollide(Entity &e);
    void addNoCollide(uid id);
    bool testCollide(Entity &e);

    bulletData &getBaseData();
};

extern std::vector<Bullet *> worldBullets;

#endif
