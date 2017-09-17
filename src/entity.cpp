#include "entity.h"
#include <algorithm>

std::vector<Entity *> worldEntities;

static size_t numEntities;
static Entity::entityData *defaultEntites;

unsigned int Entity::currentEntityID = 0;

Entity::Entity(uid pid): parentid(pid) {
    this->globalid = Entity::currentEntityID++;

    if(numEntities < parentid) throw "No entity with ID"_s;

    this->tid = defaultEntites[parentid].tid;
    this->stats = defaultEntites[parentid].stats;

    this->setSize(4);

    this->setHealth(this->getMaxHealth());
    this->setAbility(this->getMaxAbility());

    this->inv = nullptr;
    //if(parentid == 0){
        //e->inv = createInventory(12);
                   //giveItem(e->inv, createRandomItem(1));
        //int slot = giveItem(e->inv, createRandomItem(0));
        //moveItem(e->inv, slot, 4);
    //}

    this->facing = 0;
    this->ai = nullptr;
    this->grantAI(AI_NONE);
}

Entity::~Entity(){
    if(this->inv) delete this->inv;
    if(this->ai) delete this->ai;
}

void Entity::useItem(int slot){
    if(!this->inv) return;

    Item *it = this->inv->items[slot];
    if(!it) return;

    Item::itemData &idat = it->getBaseData();
    if(this->abi < idat.abiCost) return;
    if(appTime - it->lastUse < idat.cooldown) return;

    Item::itemUseFunction iuf = ***idat.onUse;
    if(!iuf) return;

    if(iuf(*this, *it)){
        it->lastUse = appTime;
        this->abi -= idat.abiCost;
    }
    return;
}

void Entity::move(position x, position y){
    this->x += x;
    this->y += y;
}

void Entity::loadData(){
    lua_getfield(L, -1, "data");
    lua_getfield(L, -1, "entity");
    lua_len(L, -1);

    numEntities = lua_tonumber(L, -1);
    lua_pop(L, 1);

    defaultEntites = new Entity::entityData[numEntities];

    printf("Made %i entities\n", numEntities);

    for(size_t i = 0; i < numEntities; i++){
        lua_geti(L, -1, i + 1);
        if(!lua_istable(L, -1)) throw "Incorrect table format in entity data "_s;

#define defineField(name, logic, sub) \
        lua_getfield(L, -1, #name); \
        defaultEntites[i]. sub name = logic; \
        lua_pop(L, 1);

        defineField(tid, (textureID) lua_tonumber(L, -1), );
        defineField(name, std::string(lua_tostring(L, -1)), );
        defineField(scale, lua_tonumber(L, -1), );

        lua_getfield(L, -1, "stats");
            defineField(agi, lua_tonumber(L, -1), stats. );
            defineField(def, lua_tonumber(L, -1), stats. );
            defineField(abi, lua_tonumber(L, -1), stats. );
            defineField(vit, lua_tonumber(L, -1), stats. );
        lua_pop(L, 1);

        lua_pop(L, 1);
    }

    lua_pop(L, 2);
}

void Entity::unloadData(){
    delete[] defaultEntites;
}

void Entity::setSize(int scale){
    this->w = scale * 8;
    this->h = scale * 8;
}


void Entity::setPos(position x, position y){
    this->x = x;
    this->y = y;
}

void Entity::setHealth(int hp){
    this->hp = hp;
    if(this->hp <= 0){
        this->kill();
    }
}

void Entity::changeHealth(int hp){
    this->setHealth(this->hp + hp);
}

void Entity::setAbility(int abi){
    this->abi = abi;
}

int Entity::getMovespeed(){
    return this->stats.agi * 2 + 200;
}

stattype Entity::getMaxHealth(){
    return this->stats.vit * 10 + 10;
}
stattype Entity::getMaxAbility(){
    return this->stats.abi * 5;
}
stattype Entity::getRegenHealth(){
    return this->getMaxHealth() / 60;
}
stattype Entity::getRegenAbility(){
    return this->getMaxAbility() / 15;
}

std::string &Entity::getName(){
    return this->name.empty() ? defaultEntites[this->parentid].name : this->name;
}

void Entity::grantAI(enum AI method){
    if(this->ai) delete this->ai;
    if(method != AI_NONE){
        if(this->ai != NULL) free(this->ai);
        this->ai = new AIData;
        this->ai->currentMethod = method;
    }else{
        this->ai = NULL;
    }
}

void Entity::kill(){
    this->unspawn();
    if(this == controlledEntity) setControlledEntity(NULL);
}

void Entity::spawn(){
    worldEntities.push_back(this);
}

void Entity::unspawn(){
    auto it = std::find(worldEntities.begin(), worldEntities.end(), this);
    if(it != worldEntities.end()){
        std::swap(*it, worldEntities.back());
        worldEntities.pop_back();
    }
}


void Entity::moveAng(angle ang, double delta){
    this->x += sin(ang) * delta;
    this->y -= cos(ang) * delta;
}
