#include "camera.h"

double cameraAng, cameraX, cameraY;
pent cameraFollowing;
int cameraYOffset, cameraXOffset;
double cameraZoom;

void cameraFollowEntity(pent e){
    cameraFollowing = e;
}
void cameraMoveTo(double x, double y){
    cameraX = x;
    cameraY = y;
}
void setCameraOffset(int x, int y){
    cameraXOffset = x;
    cameraYOffset = y;
}
void setCameraZoom(double zoom){
    (void) zoom;
}
void cameraTick(framerate framems){
    (void) framems;
    if(cameraFollowing != NULL){
        cameraMoveTo(cameraFollowing->x - cameraXOffset - cameraFollowing->w/2, cameraFollowing->y - cameraYOffset - cameraFollowing->h/2);
    }
}
