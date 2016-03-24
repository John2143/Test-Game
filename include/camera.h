#ifndef camera_H
#define camera_H

#include "entity.h" //pent

extern double cameraAng, cameraX, cameraY;
extern pent cameraFollowing;
extern int cameraYOffset, cameraXOffset;
extern double cameraZoom;

void cameraFollowEntity(pent e);
void cameraMoveTo(double x, double y);
void setCameraOffset(int x, int y);
void setCameraZoom(double zoom);

void cameraTick(framerate framems);

#endif
