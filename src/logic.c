#include "logic.h"

pent controlledEntity = NULL;

void setControlledEntity(pent e){
    controlledEntity = e;
}

static enum movementDirection{
    MOVED_N, MOVED_S,
    MOVED_E, MOVED_W,
    MOVED_NE, MOVED_SE,
    MOVED_NW, MOVED_SW,
    MOVED_NONE
};

static const double movementDirectionAngles[] = {
    (0*PI)/4, (4*PI)/4,
    (2*PI)/4, (6*PI)/4,
    (1*PI)/4, (3*PI)/4,
    (7*PI)/4, (5*PI)/4,
};

#define KP_UP    isKeyPressed(0x52)
#define KP_DOWN  isKeyPressed(0x51)
#define KP_LEFT  isKeyPressed(0x50)
#define KP_RIGHT isKeyPressed(0x4F)

void gameUpdate(framerate framems){
    if(controlledEntity != NULL){
        enum movementDirection direction = MOVED_NONE;
        if(KP_UP){
            direction = MOVED_N;
        }
        if(KP_DOWN){
            direction = direction == MOVED_N ? MOVED_NONE : MOVED_S;
        }
        if(KP_RIGHT && !KP_LEFT){
            switch(direction){
                case MOVED_N: direction = MOVED_NE; break;
                case MOVED_S: direction = MOVED_SE; break;
                default: direction = MOVED_E;
            }
        }else if(KP_LEFT){
            switch(direction){
                case MOVED_N: direction = MOVED_NW; break;
                case MOVED_S: direction = MOVED_SW; break;
                default: direction = MOVED_W;
            }
        }

        if(isKeyPressed(0xa)){
            controlledEntity->stats.agi = 0;
        }else if(isKeyPressed(0xb)){
            controlledEntity->stats.agi = 100;
        }else if(isKeyPressed(0xd)){
            controlledEntity->stats.agi = 150;
        }

        if(direction != MOVED_NONE){
            moveEntityAng(controlledEntity,
                movementDirectionAngles[direction],
                ((controlledEntity->stats.agi * 2 + 200) * framems)
            );
        }
    }
}

void initLogic(){
}
