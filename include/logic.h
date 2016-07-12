#ifndef logic_H
#define logic_H

#include "global.h" //framerate
#include "math.h" //sin cos
#include "input.h" //isKeyDown
#include "entity.h" //pent and entity movement
#include "camera.h" //cameraAng changes
#include "bullet.h"

void gameUpdate(framerate framems, framerate appTime);
void initLogic();

void setControlledEntity(pent e);
extern pent controlledEntity;

#endif
