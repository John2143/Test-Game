#include "bullet.h"

struct bulletData *bulletDatas;
pbull worldBullets = NULL;

int currentBulletID = 0;
pbull createBullet(uid dataid, framerate appTime, pent owner, angle ang){
    pbull e = malloc(sizeof(*e));

    e->globalid = currentBulletID++;
    e->dataid = dataid;
    e->ctime = appTime;

    e->owner = owner;
    e->x = owner->x;
    e->y = owner->y;
    e->ang = ang;

    e->last = NULL;
    e->next = worldBullets;
    if(worldBullets) worldBullets->last = e;
    worldBullets = e;

    return e;
}
void removeBullet(pbull e){
    if(e->last) e->last->next = e->next;
    if(e->next) e->next->last = e->last;
    if(worldBullets == e) worldBullets = e->next;
    free(e);
}

void initializeBullets(){
    bulletDatas = malloc(50 * sizeof(*bulletDatas));

    bulletDatas[0] = (struct bulletData) {
        .speed = 500,
        .damage = 10,
        .lifetime = 1.,
        .texture = loadTexture(assetFolderPath "bullet.png"),
        .w = 32, .h = 32,
        .flags = 0,
    };

    bulletDatas[1] = (struct bulletData) {
        .speed = 32,
        .damage = 50,
        .lifetime = 1,
        .texture = loadTexture(assetFolderPath "bullet.png"),
        .w = 32, .h = 32,
        .flags = BFLAG_MELEE,
    };
}

bool hitboxTouching(pent e, pbull b){
    position dx = e->x - b->x, dy = e->y - b->y;
    float dist = sqrt(dx * dx + dy * dy);
    //bullets have a circular hitbox of 16 px
    return dist < MIN(e->w, e->h) + 16;
}

void tickBullets(framerate framems, framerate appTime){
    bool dead;
    for(pbull e = worldBullets, next; e != NULL; e = next){
        dead = false;
        struct bulletData pdata = bulletDatas[e->dataid];
        //TODO O(n^2) = bad
        for(pent ent = worldEntities.first; ent != NULL; ent = ent->next){
            if(ent != e->owner && hitboxTouching(ent, e)){
                hurtEntity(ent, pdata.damage);
                dead = true;
                break;
            }
        }
        if(appTime - e->ctime >= pdata.lifetime){
            dead = true;
        }else{
            if(pdata.flags & BFLAG_MELEE){
                e->x  = cos(e->ang) * pdata.speed + e->owner->x;
                e->y  = sin(e->ang) * pdata.speed + e->owner->y;
            }else{
                e->x += cos(e->ang) * pdata.speed * framems;
                e->y += sin(e->ang) * pdata.speed * framems;
            }
        }
        next = e->next;
        if(dead) removeBullet(e);
    }
}

void uninitializeBullets(){
    free(bulletDatas);
}
