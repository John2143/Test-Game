#ifndef logic_H
#define logic_H

#include "global.h" //framerate
#include "math.h" //sin cos
#include "input.h" //isKeyDown
#include "entity.h" //pent and entity movement

void gameUpdate(framerate framems);
void initLogic();

void setControlledEntity(pent e);
extern pent controlledEntity;

#endif
