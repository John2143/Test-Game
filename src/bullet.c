#include "bullet.h"

struct bulletData *bulletDatas;
pbull worldBullets = NULL;

int currentBulletID = 0;
pbull createBullet(uid dataid, framerate appTime, pent owner, angle ang){
    pbull e = malloc(sizeof(*e));

    e->globalid = currentBulletID++;
    e->dataid = dataid;
    e->ctime = appTime;

    e->nocollideAmountMax = 4;
    e->nocollide = malloc(e->nocollideAmountMax * sizeof(uid));
    if(!e->nocollide){
        printf("Out of memory @ createBullet\n");
        exit(1);
    }
    e->nocollideAmount = 0;
    addNoCollideEntToBullet(owner->globalid, e);

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
    free(e->nocollide);
    free(e);
}

void initializeBullets(){
    bulletDatas = malloc(50 * sizeof(*bulletDatas));

    bulletDatas[0] = (struct bulletData) {
        .speed = 500,
        .damage = 5,
        .lifetime = 1.,
        .texture = loadTexture(assetFolderPath "bullet2x.png"),
        .w = 32, .h = 32,
        .flags = BFLAG_PIERCE,
    };

    bulletDatas[1] = (struct bulletData) {
        .speed = 32,
        .damage = 50,
        .lifetime = 1,
        .texture = loadTexture(assetFolderPath "bullet.png"),
        .w = 32, .h = 128,
        .flags = BFLAG_MELEE,
    };
}

bool bulletHitboxTouching(pent e, pbull b){
    position dx = e->x - b->x, dy = e->y - b->y;
    float dist = sqrt(dx * dx + dy * dy);
    //bullets have a circular hitbox of 16 px
    return dist < MIN(e->w, e->h) + 16;
}

void addNoCollideEntToBullet(uid id, pbull b){
    b->nocollide[b->nocollideAmount++] = id;
    if(b->nocollideAmount >= b->nocollideAmountMax){
        b->nocollideAmountMax = b->nocollideAmountMax * 2;
        b->nocollide = realloc(b->nocollide, b->nocollideAmountMax * sizeof(uid));
        if(!b->nocollide){
            printf("Out of memory @ addCollideEntToBullet\n");
            exit(1);
        }
    }
}

bool shouldBulletAndEntCollide(pent e, pbull b){
    for(int i = 0; i < b->nocollideAmount; i++){
        if(e->globalid == b->nocollide[i]) return false;
    }
    return true;
}

void tickBullets(framerate framems, framerate appTime){
    bool dead;
    for(pbull e = worldBullets, next; e != NULL; e = next){
        dead = false;
        struct bulletData pdata = bulletDatas[e->dataid];
        //TODO O(n^2) = bad
        for(pent ent = worldEntities.first; ent != NULL; ent = ent->next){
            if(!shouldBulletAndEntCollide(ent, e)) continue;
            if(bulletHitboxTouching(ent, e)){
                /*printf("Doing %i damage to ent %i (hp %i)\n", pdata.damage, ent->globalid, ent->hp);*/
                hurtEntity(ent, pdata.damage);
                if(pdata.flags & BFLAG_PIERCE){
                    addNoCollideEntToBullet(ent->globalid, e);
                }else{
                    dead = true;
                    goto BULLET_DEAD;
                }
            }
        }

        if(appTime - e->ctime >= pdata.lifetime){
            dead = true;
            goto BULLET_DEAD;
        }else{
            if(pdata.flags & BFLAG_MELEE){
                /*e->x  = cos(e->ang) * pdata.speed + e->owner->x;*/
                /*e->y  = sin(e->ang) * pdata.speed + e->owner->y;*/
            }else{
                e->x += cos(e->ang) * pdata.speed * framems;
                e->y += sin(e->ang) * pdata.speed * framems;
            }
        }

        BULLET_DEAD:;

        next = e->next;
        if(dead) removeBullet(e);
    }
}

void uninitializeBullets(){
    //TODO free as linked list
}
