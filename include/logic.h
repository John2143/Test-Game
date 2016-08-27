#ifndef logic_H
#define logic_H

#include "global.h"
#include "input.h" //isKeyDown
#include "entity.h" //pent and entity movement
#include "camera.h" //cameraAng changes
#include "inventory.h"
#include "bullet.h"

void gameUpdate();
void initLogic();

void logicUseItem(int slot);
void logicOnKeyPress(uint32_t code);
void setControlledEntity(pent e);
extern pent controlledEntity;

#endif
