#include "bullet.h"

bulletData *bulletDatas;
pbull worldBullets = NULL;

int currentBulletID = 0;
pbull createBullet(uid dataid, framerate appTime, Entity &owner, angle ang){
    pbull e = new bullet;
    struct bulletData bdat = bulletDatas[dataid];

    e->globalid = currentBulletID++;
    e->dataid = dataid;
    e->ctime = appTime;

    e->nocollideAmountMax = 4;
    e->nocollide = (uid *) malloc(e->nocollideAmountMax * sizeof(uid));
    if(!e->nocollide){
        printf("Out of memory @ createBullet\n");
        exit(1);
    }
    e->nocollideAmount = 0;
    addNoCollideEntToBullet(owner.globalid, e);

    e->x = owner.x;
    e->y = owner.y;
    e->ang = ang;

    e->damage = bdat.damage;

    e->last = NULL;
    e->next = worldBullets;
    if(worldBullets) worldBullets->last = e;
    worldBullets = e;

    return e;
}

pbull createItemBullet(uid dataid, framerate appTime, Entity &owner, pitem it, angle ang){
    pbull e = createBullet(dataid, appTime, owner, ang);
    e->damage += itemDatas[it->itemid].baseDamage;
    return e;
}

void removeBullet(pbull e){
    if(e->last) e->last->next = e->next;
    if(e->next) e->next->last = e->last;
    if(worldBullets == e) worldBullets = e->next;
    free(e->nocollide);
    delete e;
}

void initializeBullets(){
    bulletDatas = new bulletData[50];

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

bool bulletHitboxTouching(Entity &e, pbull b){
    position dx = e.x - b->x, dy = e.y - b->y;
    float dist = sqrt(dx * dx + dy * dy);
    //bullets have a circular hitbox of 16 px
    return dist < MIN(e.w, e.h) + 16;
}

void addNoCollideEntToBullet(uid id, pbull b){
    b->nocollide[b->nocollideAmount++] = id;
    if(b->nocollideAmount >= b->nocollideAmountMax){
        b->nocollideAmountMax = b->nocollideAmountMax * 2;
        b->nocollide = (uid *) realloc(b->nocollide, b->nocollideAmountMax * sizeof(uid));
        if(!b->nocollide){
            printf("Out of memory @ addCollideEntToBullet\n");
            exit(1);
        }
    }
}

bool shouldBulletAndEntCollide(Entity &e, pbull b){
    for(int i = 0; i < b->nocollideAmount; i++){
        if(e.globalid == b->nocollide[i]) return false;
    }
    return true;
}

void tickBullets(){
    bool dead;
    for(pbull e = worldBullets, next; e != NULL; e = next){
        dead = false;
        struct bulletData pdata = bulletDatas[e->dataid];
        //TODO O(n^2) = bad
        for(Entity *ent : worldEntities){
            if(!shouldBulletAndEntCollide(*ent, e)) continue;
            if(bulletHitboxTouching(*ent, e)){
                /*printf("Doing %i damage to ent %i (hp %i)\n", e->damage, ent->globalid, ent->hp);*/
                ent->changeHealth(-e->damage);
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
                e->x += cos(e->ang) * pdata.speed * frameTime;
                e->y += sin(e->ang) * pdata.speed * frameTime;
            }
        }

        BULLET_DEAD:;

        next = e->next;
        if(dead) removeBullet(e);
    }
}

void uninitializeBullets(){
    for(pbull b = worldBullets; b != NULL;){
        pbull next = b->next;
        delete b;
        b = next;
    }
}
