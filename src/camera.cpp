#include "camera.h"

position cameraX, cameraY;
angle cameraAng;
Entity *cameraFollowing = nullptr;
int cameraYOffset, cameraXOffset;
angle cameraZoom;

void cameraFollowEntity(Entity *e){
    cameraFollowing = e;
}
void cameraMoveTo(position x, position y){
    cameraX = x;
    cameraY = y;
}
void setCameraOffset(int x, int y){
    cameraXOffset = x;
    cameraYOffset = y;
}
void setCameraZoom(angle zoom){
    (void) zoom;
}
void cameraTick(){
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
angle getLocalAimPosition(Entity &e){
    int x, y;
    worldMousePosition(&x, &y);
    return atan2(
        y - e.y,
        x - e.x
    );
}
