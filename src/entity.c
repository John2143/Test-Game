#include <entity.h>

static unsigned int currentEntityID = 0;
struct worldLinkedList worldEntities = {
    .first = NULL,
    .last = NULL
};

static struct entityData *defaultEntites;

void spawnEntity(pent e){
    if(worldEntities.first == NULL){
        worldEntities.first = e;
    }else{
        worldEntities.last->next = e;
    }
    worldEntities.last = e;
    e->next = NULL;
}

void unspawnEntity(pent e){
    pent currEnt = worldEntities.first, lastEnt = NULL;
    while(currEnt != NULL){
        if(currEnt == e){
            //handles edge cases of this being the penultimante entity or this
            //  being the first enemy
            if(lastEnt){
                lastEnt->next = e->next;
            }else{
                worldEntities.first = e->next;
            }
            return; //entity removed successfully
        }
        lastEnt = currEnt;
        currEnt = currEnt->next;
    }
    return; //entity not found
}

pent newEntity(int parentid){
    pent e = malloc(sizeof(*e));
    e->globalid = currentEntityID++;
    e->parentid = parentid;

    e->textureID = defaultEntites[parentid].textureID;
    e->stats = defaultEntites[parentid].stats;

    setEntityPos(e, 200, 200);
    setEntitySize(e, 4);

    e->hp = e->stats.hp;
    e->facing = 0;
    e->ai = NULL;

    return e;
}

void deleteEntity(pent e){
    free(e);
}

void moveEntity(pent e, double x, double y){
    e->x += x;
    e->y += y;
}

void setEntityPos(pent e, double x, double y){
    e->x = x;
    e->y = y;
}

void moveEntityAng(pent e, double ang, double del){
    e->x += sin(ang) * del;
    e->y -= cos(ang) * del;
}

void killEntity(pent e){
    e->hp = 0;
    unspawnEntity(e);
}

void loadEntities(){
    defaultEntites = malloc(2 * sizeof(*defaultEntites));

    defaultEntites[0].name = "Player";
    defaultEntites[0].textureID = loadTexture(assetFolderPath "meme.png");
    defaultEntites[0].stats = (struct stats) {
        .hp = 100, .def = 0, .agi = 50
    };

    defaultEntites[1].name = "testname";
    defaultEntites[1].textureID = loadTexture(assetFolderPath "meme.png");
    defaultEntites[1].stats = (struct stats) {
        .hp = 100, .def = 0, .agi = 0
    };
}

void unloadEntities(){
    free(defaultEntites);
}

int getEntityMovespeed(pent e){
    return e->stats.agi * 2 + 200;
}

pent findClosestEntity(pent to, int type){
    pent c = worldEntities.first;
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
    if(e->ai != NULL) free(e->ai);
    if(method != AI_NONE){
        e->ai = malloc(sizeof(*e->ai));
        e->ai->currentMethod = method;
    }
}
