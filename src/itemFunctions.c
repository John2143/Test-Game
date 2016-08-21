#include "itemFunctions.h"

int IFShotgun(pent e, pitem it){
    (void) (it);
    int x, y;
    worldMousePosition(&x, &y);

    angle a = atan2(
        y - e->y,
        x - e->x
    );

    createBullet(0, appTime, e, a);
    createBullet(0, appTime, e, a - .15);
    createBullet(0, appTime, e, a + .15);
    return 0;
}

void initializeItemFunctions(){
    itemDatas[0].onUse = IFShotgun;
}

void uninitializeItemFunctions(){
}
