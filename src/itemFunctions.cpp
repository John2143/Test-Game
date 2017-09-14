#include "itemFunctions.h"

static int IFShotgun(pent e, pitem it){
    angle a = getLocalAimPosition(e);
    createItemBullet(0, appTime, e, it, a);
    createItemBullet(0, appTime, e, it, a - .15);
    createItemBullet(0, appTime, e, it, a + .15);
    return 0;
}

static int IFWand(pent e, pitem it){
    angle a = getLocalAimPosition(e);
    createItemBullet(0, appTime, e, it, a);
    return 0;
}

void initializeItemFunctions(){
    itemDatas[0].onUse = IFShotgun;
    itemDatas[1].onUse = IFWand;
}

void uninitializeItemFunctions(){
}
