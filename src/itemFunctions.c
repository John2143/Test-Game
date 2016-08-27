#include "itemFunctions.h"

static int IFShotgun(pent e, pitem it){
    (void) (it);

    angle a = getLocalAimPosition(e);

    createBullet(0, appTime, e, a);
    createBullet(0, appTime, e, a - .15);
    createBullet(0, appTime, e, a + .15);

    return 0;
}

static int IFWand(pent e, pitem it){
    angle a = getLocalAimPosition(e);
    createBullet(0, appTime, e, a);
    return 0;
}

void initializeItemFunctions(){
    itemDatas[0].onUse = IFShotgun;
    itemDatas[1].onUse = IFWand;
}

void uninitializeItemFunctions(){
}
