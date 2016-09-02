#include <entity.h>

static unsigned int currentEntityID = 0;
pent worldEntities = NULL;

static struct entityData *defaultEntites;

void spawnEntity(pent e){
    e->last = NULL;
    e->next = worldEntities;
    if(worldEntities) worldEntities->last = e;
    worldEntities = e;
}

void unspawnEntity(pent e){
    if(e->last) e->last->next = e->next;
    if(e->next) e->next->last = e->last;
    if(worldEntities == e) worldEntities = e->next;
}

pent newEntityShell(uid parentid, pent e){
    e->globalid = currentEntityID++;
    e->parentid = parentid;

    e->textureID = defaultEntites[parentid].textureID;
    e->stats = defaultEntites[parentid].stats;

    setEntityPos(e, 200, 200);
    setEntitySize(e, 4);

    setEntityHealth(e, getEntityMaxHealth(e));
    setEntityAbility(e, getEntityMaxAbility(e));

    e->inventory = NULL;
    //TODO
    if(parentid == 0){
        e->inventory = createInventory(12);
                   giveItem(e->inventory, createRandomItem(1));
        int slot = giveItem(e->inventory, createRandomItem(0));
        moveItem(e->inventory, slot, 4);
    }

    e->facing = 0;
    grantAI(e, AI_NONE);
    return e;
}

pent newEntity(uid parentid){
    return newEntityShell(parentid, malloc(sizeof(struct entity)));
}

void deleteEntity(pent e){
    if(e->inventory) freeInventory(e->inventory);
    free(e);
}

int entityUseItem(pent e, int slot){
    struct inventory *inv = e->inventory;
    if(!inv) return INVE_NOINV;
    pitem it = inv->items[slot];
    if(!it) return INVE_NOITEM;
    struct itemData idat = itemDatas[it->itemid];
    if(e->abi < idat.abiCost) return INVE_NOABI;
    if(appTime - it->lastUse < idat.cooldown) return INVE_ONCOOLDOWN;
    itemUseFunction iuf = idat.onUse;
    if(!iuf) return INVE_NOFUNC;
    int ret = iuf(e, it);
    if(ret != INVE_DONTCOOLDOWN){
        e->abi -= idat.abiCost;
        it->lastUse = appTime;
    }
    return 0;
}

void moveEntity(pent e, position x, position y){
    e->x += x;
    e->y += y;
}

void setEntityPos(pent e, position x, position y){
    e->x = x;
    e->y = y;
}

void moveEntityAng(pent e, angle ang, double del){
    e->x += sin(ang) * del;
    e->y -= cos(ang) * del;
}

void killEntity(pent e){
    unspawnEntity(e);
    if(e == controlledEntity) setControlledEntity(NULL);
}

void setEntityHealth(pent e, int hp){
    e->hp = hp;
    if(hp <= 0){
        killEntity(e);
        //deleteEntity(e); ?????
    }
}

void changeEntityHealth(pent e, int hp){
    setEntityHealth(e, e->hp + hp);
}

void hurtEntity(pent e, int hp){
    changeEntityHealth(e, -hp);
}

void setEntityAbility(pent e, int abi){
    e->abi = abi;
}

void loadEntities(){
    defaultEntites = malloc(50 * sizeof(*defaultEntites));

    defaultEntites[0].name = "Player";
    defaultEntites[0].textureID = loadTexture(assetFolderPath "player.png");
    defaultEntites[0].stats = (struct stats) {
        .vit = 10, .def = 5, .agi = 500, .abi = 5
    };

    defaultEntites[1].name = "testname";
    defaultEntites[1].textureID = loadTexture(assetFolderPath "meme.png");
    defaultEntites[1].stats = (struct stats) {
        .vit = 0, .def = 0, .agi = 0, .abi = 0
    };

}

void unloadEntities(){
    free(defaultEntites);
}

int getEntityMovespeed(pent e){
    return e->stats.agi * 2 + 200;
}

stattype getEntityMaxHealth(pent e){
    return e->stats.vit * 10 + 10;
}

stattype getEntityMaxAbility(pent e){
    return e->stats.abi * 5;
}

//Amount per second = max / (seconds to fully regen)
stattype getEntityRegenHealth(pent e){
    return getEntityMaxHealth(e) / 60;
}

stattype getEntityRegenAbility(pent e){
    return getEntityMaxAbility(e) / 15;
}

pent findClosestEntity(pent to, uid type){
    pent c = worldEntities;
    while(c != NULL){
        if(c != to && type == c->parentid) return c;
        c = c->next;
    }
    return NULL;
}

const char *getName(pent e){
    return e->name == NULL ? defaultEntites[e->parentid].name : e->name;
}

void setEntitySize(pent e, int scale){
    e->w = scale * 8;
    e->h = scale * 8;
}

void embiggenEntity(pent e){
    e->w += 8;
    e->h += 8;
}

void grantAI(pent e, enum AI method){
    if(method != AI_NONE){
        if(e->ai != NULL) free(e->ai);
        e->ai = malloc(sizeof(*e->ai));
        e->ai->currentMethod = method;
    }else{
        e->ai = NULL;
    }
}
