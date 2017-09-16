#include "camera.h"

position cameraX = 0, cameraY = 0;
angle cameraAng = 0;
Entity *cameraFollowing = nullptr;
int cameraYOffset = 0, cameraXOffset = 0;
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
    if(cameraFollowing != nullptr){
        cameraMoveTo(cameraFollowing->x - cameraXOffset, cameraFollowing->y - cameraYOffset);
    }
}
void screenToWorld(int x, int y, int &relx, int &rely){
    //TODO make this not so hacky
    relx = (cameraX + x);
    rely = (cameraY + y);
}
void worldMousePosition(int &relx, int &rely){
    screenToWorld(mouseX, mouseY, relx, rely);
}
angle getLocalAimPosition(Entity &e){
    int x, y;
    worldMousePosition(x, y);
    return atan2(
        y - e.y,
        x - e.x
    );
}
