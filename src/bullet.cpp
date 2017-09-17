#include "bullet.h"
#include <algorithm>

std::vector<Bullet *> worldBullets;

static uid globalID = 0;
size_t numBullets;
Bullet::bulletData *Bullet::bulletDatas;

Bullet::Bullet(uid pid, Entity &owner, angle ang): parentid(pid), globalid(globalID++), ang(ang){
    this->createTime = appTime;
    this->x = owner.x;
    this->y = owner.y;

    this->damage = bulletDatas[pid].damage;

    this->addNoCollide(owner.globalid);

    worldBullets.push_back(this);
}

Bullet::~Bullet(){
    for(size_t i = 0; i < worldBullets.size(); i++){
        if(worldBullets[i] == this){
            worldBullets[i] = worldBullets[worldBullets.size() - 1];
        }
    }
    worldBullets.pop_back();
}

void Bullet::initializeBullets(){
    lua_getfield(L, -1, "data");
    lua_getfield(L, -1, "bullets");
    lua_len(L, -1);

    numBullets = lua_tonumber(L, -1);
    lua_pop(L, 1);

    Bullet::bulletDatas = new Bullet::bulletData[numBullets];

    printf("Made %i bullets\n", numBullets);

    for(size_t i = 0; i < numBullets; i++){
        lua_geti(L, -1, i + 1);
        if(!lua_istable(L, -1)) throw "Incorrect table format in bullet data "_s;

#define defineField(name, logic) \
        lua_getfield(L, -1, #name); \
        bulletDatas[i].name = logic; \
        lua_pop(L, 1);

        defineField(texture, (textureID) lua_tonumber(L, -1));
        defineField(speed, lua_tonumber(L, -1));
        defineField(w, lua_tonumber(L, -1));
        defineField(h, lua_tonumber(L, -1));
        defineField(lifetime, lua_tonumber(L, -1));
        defineField(damage, lua_tonumber(L, -1));
        defineField(flags, 1);

        lua_pop(L, 1);
    }

    lua_pop(L, 2);
}

void Bullet::uninitializeBullets(){
    //delete bullets?
}

bool Bullet::hitboxTouching(Entity &e){
    position dx = e.x - this->x, dy = e.y - this->y;
    float dist = sqrt(dx * dx + dy * dy);
    //bullets have a circular hitbox of 16 px
    return dist < MIN(e.w, e.h) + 16;
}

void Bullet::addNoCollide(uid id){
    this->nocollide.push_back(id);
}

bool Bullet::shouldCollide(Entity &e){
    for(size_t i = 0; i < this->nocollide.size(); i++){
        if(this->nocollide[i] == e.globalid){
            return false;
        }
    }
    return true;
}

void Bullet::tick(){
    bulletData pdata = this->getBaseData();

    for(Entity *ent : worldEntities){
        if(!this->shouldCollide(*ent)) continue;
        if(this->hitboxTouching(*ent)){
            ent->changeHealth(-this->damage);

            if(pdata.flags & BFLAG_PIERCE){
                this->addNoCollide(ent->globalid);
            }else{
                delete this;
                return;
            }
        }
    }

    if(appTime - this->createTime < pdata.lifetime){
        this->x += cos(this->ang) * pdata.speed * frameTime;
        this->y += sin(this->ang) * pdata.speed * frameTime;
    }else{
        delete this;
        return;
    }
}

Bullet::bulletData &Bullet::getBaseData(){
    return bulletDatas[this->parentid];
}

void Bullet::tickAll(){
    for(Bullet *b : worldBullets){
        b->tick();
    }
}
