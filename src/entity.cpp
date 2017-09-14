#include "entity.hpp"

const char Entity_L::className[] = "Entity";
Luna<Entity_L>::RegType Entity_L::Register[] = {
    {"move", &Entity_L::move},
    {"setPos", &Entity_L::setPos},
    {"setHealth", &Entity_L::setHealth},
    {"changeHealth", &Entity_L::changeHealth},
    {"setAbility", &Entity_L::setAbility},
    {"getMovespeed", &Entity_L::getMovespeed},
    {"getMaxHealth", &Entity_L::getMaxHealth},
    {"getMaxAbility", &Entity_L::getMaxAbility},
    {"getRegenHealth", &Entity_L::getRegenHealth},
    {"getRegenAbility", &Entity_L::getRegenAbility},
    {"getName", &Entity_L::getName},
    {"grantAI", &Entity_L::grantAI},
    {"kill", &Entity_L::kill},
    {"spawn", &Entity_L::spawn},
    {"unspawn", &Entity_L::unspawn},
    {"setControlled", &Entity_L::setControlled},
    {NULL, NULL}
};

std::vector<Entity> worldEntities;

static Entity::entityData *defaultEntites;
unsigned int Entity::currentEntityID = 0;

Entity::Entity(uid pid): parentid(pid) {
    this->globalid = Entity::currentEntityID++;

    this->tid = defaultEntites[parentid].tid;
    this->stats = defaultEntites[parentid].stats;

    this->setSize(4);

    this->setHealth(this->getMaxHealth());
    this->setAbility(this->getMaxAbility());
    this->name = nullptr;

    this->inv = nullptr;
    //if(parentid == 0){
        //e->inv = createInventory(12);
                   //giveItem(e->inv, createRandomItem(1));
        //int slot = giveItem(e->inv, createRandomItem(0));
        //moveItem(e->inv, slot, 4);
    //}

    this->facing = 0;
    this->grantAI(AI_NONE);
}

Entity::~Entity(){
    if(this->inv) freeInventory(this->inv);
    if(this->ai) delete this->ai;
}

invError Entity::useItem(int slot){
    if(!this->inv) return INVE_NOINV;
    pitem it = this->inv->items[slot];
    if(!it) return INVE_NOITEM;
    struct itemData idat = itemDatas[it->itemid];
    if(this->abi < idat.abiCost) return INVE_NOABI;
    if(appTime - it->lastUse < idat.cooldown) return INVE_ONCOOLDOWN;
    itemUseFunction iuf = idat.onUse;
    if(!iuf) return INVE_NOFUNC;
    //int ret = iuf(this, it);
    int ret = 0;
    if(ret != INVE_DONTCOOLDOWN){
        this->abi -= idat.abiCost;
        it->lastUse = appTime;
    }
    return INVE_NONE;
}

void Entity::move(position x, position y){
    this->x += x;
    this->y += y;
}

void Entity::loadData(){
    defaultEntites = new entityData[50];

    defaultEntites[0].name = "Player";
    defaultEntites[0].tid = loadTexture(assetFolderPath "player.png");
    defaultEntites[0].stats.agi = 100;

    defaultEntites[1].name = "testname";
    defaultEntites[1].tid = loadTexture(assetFolderPath "meme.png");
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

const char *Entity::getName(){
    return this->name == NULL ? defaultEntites[this->parentid].name : this->name;
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
    worldEntities.push_back(*this);
}

void Entity::unspawn(){
}


void Entity::moveAng(angle ang, double delta){
    this->x += sin(ang) * delta;
    this->y -= cos(ang) * delta;
}
