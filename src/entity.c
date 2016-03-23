#include <entity.h>

struct worldLinkedList worldEntities = {
    .first = NULL,
    .last = NULL
};
/*struct entityData defaultEntites[] = {*/
    /*{.name = "None", .stats = {.def = 0, .hp = 100, .agi = 0}, .textureID = 1},*/
/*};*/

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

pent newEntity(enum entityID id){
    pent e = malloc(sizeof(*e));
    e->id = id;
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
