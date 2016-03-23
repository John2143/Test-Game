#include "logic.h"

pent controlledEntity = NULL;

void setControlledEntity(pent e){
    controlledEntity = e;
}

void gameUpdate(framerate framems){
    (void) framems;
    if(controlledEntity != NULL){
        if(isKeyPressed(0x52)){ //up
            moveEntity(controlledEntity, 0, -.1);
        }
        if(isKeyPressed(0x50)){ //left
            moveEntity(controlledEntity, -.1, 0);
        }
        if(isKeyPressed(0x51)){ //down
            moveEntity(controlledEntity, 0, .1);
        }
        if(isKeyPressed(0x4f)){ //right
            moveEntity(controlledEntity, .1, 0);
        }
    }
}

void initLogic(){
}
