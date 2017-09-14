#ifndef logic_H
#define logic_H

#include "global.h"
#include "input.h" //isKeyDown
#include "entity.hpp"
#include "camera.h" //cameraAng changes
#include "inventory.h"
#include "bullet.h"
#include "tile.h"
#include "map.h"

void gameUpdate();
void initLogic();

void logicUseItem(int slot);
void logicOnKeyPress(uint32_t code);
void setControlledEntity(Entity *e);
extern Entity *controlledEntity;

#endif
