#include "itemFunctions.h"

static bool IFShotgun(Entity &e, Item &it){
    angle a = getLocalAimPosition(e);
    createItemBullet(0, appTime, e, &it, a);
    createItemBullet(0, appTime, e, &it, a - .15);
    createItemBullet(0, appTime, e, &it, a + .15);
    return true;
}

static bool IFWand(Entity &e, Item &it){
    angle a = getLocalAimPosition(e);
    createItemBullet(0, appTime, e, &it, a);
    return true;
}

void initializeItemFunctions(){
    Item::itemDatas[0].onUse = IFShotgun;
    Item::itemDatas[1].onUse = IFWand;
}

void uninitializeItemFunctions(){
}
