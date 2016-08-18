#ifndef camera_H
#define camera_H

#include "global.h"
#include "entity.h" //pent
#include "input.h"

extern position cameraX, cameraY;
extern angle cameraAng;
extern pent cameraFollowing;
extern int cameraYOffset, cameraXOffset;
extern angle cameraZoom;

void cameraFollowEntity(pent e);
void cameraMoveTo(position x, position y);
void setCameraOffset(int x, int y);
void setCameraZoom(angle zoom);

void screenToWorld(int x, int y, int *relx, int *rely);
void worldMousePosition(int *relx, int *rely);

void cameraTick(framerate framems, framerate appTime);

#endif
