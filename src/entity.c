#include <entity.h>

static int currentEntityID = 0;
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
                worldEntities.first = NULL;
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

    e->textureID = defaultEntites[parentid].textureID;
    e->stats = defaultEntites[parentid].stats;

    e->x = 5;
    e->y = 5;
    e->hp = 0;
    e->facing = 0;


    /*typedef struct entity{*/
        /*int parentid;*/
        /*int globalid;*/

        /*GLuint textureID;*/
        /*//size_t textureOffset;*/

        /*double x, y;*/
        /*double facing;*/
        /*int hp;*/
        /*struct stats stats; //Customs stats: modified at spawn time*/

        /*struct entity *next;*/
    /*} *pent;*/
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
    e->x += cos(ang) * del;
    e->y += sin(ang) * del;
}

void killEntity(pent e){
    e->hp = 0;
    unspawnEntity(e);
}

void loadEntities(){
    defaultEntites = malloc(1 * sizeof(*defaultEntites));

    defaultEntites[0].name = "testname";
    defaultEntites[0].textureID = loadTexture(assetFolderPath "meme.png");
    defaultEntites[0].stats = (struct stats) {
        .hp = 100, .def = 0, .agi = 10
    };
}

void unloadEntities(){
    free(defaultEntites);
}
