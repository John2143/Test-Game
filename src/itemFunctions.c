#include "itemFunctions.h"

int IFShotgun(pent e, pitem it){
    (void) (it);

    angle a = getLocalAimPosition(e);

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
