#include "logic.h"

pent controlledEntity = NULL;

void setControlledEntity(pent e){
    controlledEntity = e;
    cameraFollowEntity(e);
}

enum movementDirection{
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

#define KP_ROR isKeyPressed(0x1B)
#define KP_ROL isKeyPressed(0x1D)

static framerate lastShot = 0;
static void controlEntity(framerate framems, framerate appTime){
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
    }
    if(KP_LEFT && !KP_RIGHT){
        switch(direction){
            case MOVED_N: direction = MOVED_NW; break;
            case MOVED_S: direction = MOVED_SW; break;
            default: direction = MOVED_W;
        }
    }

    if(KP_ROR && !KP_ROL){
        cameraAng += framems * 20;
    }
    if(KP_ROL && !KP_ROR){
        cameraAng += framems * 20;
    }

    if(direction != MOVED_NONE){
        moveEntityAng(controlledEntity,
            movementDirectionAngles[direction],
            getEntityMovespeed(controlledEntity) * framems
        );
    }

    if(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT) && appTime - lastShot > 1){
        lastShot = appTime;
        int x, y;
        worldMousePosition(&x, &y);

        angle a = atan2(
            y - controlledEntity->y,
            x - controlledEntity->x
        );

        createBullet(0, appTime, controlledEntity, a);
    }
}

static void tickAI(framerate framems, framerate appTime){
    (void) appTime;
    pent c = worldEntities.first;
    while(c != NULL){
        if(c != controlledEntity && c->ai != NULL){

switch(c->ai->currentMethod){
case AI_NONE: break;
case AI_WANDER: {
    c->facing += (-1 + ((angle) (rand() % 16) * .5)) * framems;
    moveEntityAng(c,
        c->facing,
        getEntityMovespeed(c) * framems
    );
} break;
case AI_CHASE: {
    pent closestPlayer = findClosestEntity(c, 0);
    if(!closestPlayer) break;

    c->facing = atan2(closestPlayer->y - c->y, closestPlayer->x - c->x) + PI/2;

    moveEntityAng(c,
        c->facing,
        getEntityMovespeed(c) * framems
    );
} break;
}

        }
        c = c->next;
    }
}

void gameUpdate(framerate framems, framerate appTime){
    if(controlledEntity != NULL) controlEntity(framems, appTime);
    tickAI(framems, appTime);
    tickBullets(framems, appTime);
}

void initLogic(){
}
