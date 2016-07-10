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
void cameraTick(framerate framems, framerate appTime){
    (void) framems, (void) appTime;
    if(cameraFollowing != NULL){
        cameraMoveTo(cameraFollowing->x - cameraXOffset, cameraFollowing->y - cameraYOffset);
    }
}
void screenToWorld(int x, int y, int *relx, int *rely){
    //TODO make this not so hacky
    if(relx) *relx = (cameraX + x);
    if(rely) *rely = (cameraY + y);
}

void worldMousePosition(int *relx, int *rely){
    screenToWorld(mouseX, mouseY, relx, rely);
}
