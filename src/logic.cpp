#include "logic.h"

Entity *controlledEntity = nullptr;

void setControlledEntity(Entity *e){
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

#define KP_UP    (isKeyPressed(0x52) || isKeyPressed(0x1A))
#define KP_DOWN  (isKeyPressed(0x51) || isKeyPressed(0x16))
#define KP_LEFT  (isKeyPressed(0x50) || isKeyPressed(0x04))
#define KP_RIGHT (isKeyPressed(0x4F) || isKeyPressed(0x07))

#define KP_ROR   isKeyPressed(0x1B)
#define KP_ROL   isKeyPressed(0x1D)

static void controlEntity(){
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
        cameraAng += frameTime * 20;
    }
    if(KP_ROL && !KP_ROR){
        cameraAng += frameTime * 20;
    }

    if(direction != MOVED_NONE){
        controlledEntity->moveAng(
            movementDirectionAngles[direction],
            controlledEntity->getMovespeed() * frameTime
        );
    }

    if(mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)){
        logicUseItem(0);
    }
    if(mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)){
        logicUseItem(1);
    }

}

void logicUseItem(int slot){
    if(controlledEntity != NULL) controlledEntity->useItem(slot);
}

void tickWallCheck(Entity &e){
    int x = e.x;
    int y = e.y;
    int w2 = e.w/2;
    int h2 = e.h/2;
    int tileX = x/TILESIZE;
    int tileY = y/TILESIZE;
    if(tileX < 1 || tileY < 1 ||
       tileY >= WORLDSIZE - 1 || tileX >= WORLDSIZE - 1)
        return;
    if(tileDatas[gameworld[tileX - 1][tileY]].isSolid){
        if(x - tileX*TILESIZE < w2) e.x = w2 + tileX*TILESIZE;
    }
    if(tileDatas[gameworld[tileX + 1][tileY]].isSolid){
        //HACKY: TILESIZE = PLAYER SIZE:::: CHANGE IN RELEASE
        if(x - tileX*TILESIZE > w2) e.x = w2 + tileX*TILESIZE;
    }
    if(tileDatas[gameworld[tileX][tileY - 1]].isSolid){
        if(y - tileY*TILESIZE < h2) e.y = h2 + tileY*TILESIZE;
    }
    if(tileDatas[gameworld[tileX][tileY + 1]].isSolid){
        //HACKY: TILESIZE = PLAYER SIZE:::: CHANGE IN RELEASE
        if(y - tileY*TILESIZE > h2) e.y = h2 + tileY*TILESIZE;
    }
}

void logicOnKeyPress(uint32_t code){
    const int keyboard1 = 0x1e;
    bool shiftPressed = isKeyPressed(0xe1);
    int keyVal = code - keyboard1;
    const int numberKeyOffset = 3;
    const int numNumberKeys = 5;
    if(keyVal < numNumberKeys && keyVal >= 0){
        if(shiftPressed){
            logicUseItem(keyVal + numNumberKeys + numberKeyOffset);
        }else{
            logicUseItem(keyVal + numberKeyOffset);
        }
    }else if(code == 0x2c){
        logicUseItem(2);
    }
}

static void tickAI(Entity &c){
    //TODO implement in lua
    if(&c != controlledEntity && c.ai != NULL){

switch(c.ai->currentMethod){
case Entity::AI::AI_NONE: break;
case Entity::AI::AI_WANDER: {
    c.facing += (-1 + ((angle) (rand() % 16) * .5)) * frameTime;
    c.moveAng(
        c.facing,
        c.getMovespeed() * frameTime
    );
} break;
case Entity::AI::AI_CHASE: break;
}

    }
}

static void tickEntity(Entity &c){
    stattype maxAbi = c.getMaxAbility();
    stattype maxHP  = c.getMaxHealth();
    if(c.abi < maxAbi) c.abi = MIN(maxAbi, c.abi + frameTime * c.getRegenAbility());
    if(c.hp < maxHP)  c.hp   = MIN(maxHP,  c.hp  + frameTime * c.getRegenHealth());
}

void gameUpdate(){
    if(controlledEntity != NULL) controlEntity();
    tickBullets();

    for(Entity *c : worldEntities){
        tickEntity(*c);
        tickAI(*c);
        tickWallCheck(*c);
    }
}

void initLogic(){
}
