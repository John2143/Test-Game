#include "inventory.h"

struct itemData *itemDatas;
struct itemPropertyData *itemPropertyDatas;
int64_t itemGlobalID = 0;

void initializeItems(){
    itemDatas = malloc(8 * sizeof(*itemDatas));

    itemPropertyDatas = malloc(8 * sizeof(*itemPropertyDatas));
    itemPropertyDatas[0].formatStr = "+%i Primary Attribute";
    itemPropertyDatas[0].valueType = IPVT_INT64;
    itemPropertyDatas[0].valueMin.i = 10;
    itemPropertyDatas[0].valueMax.i = 20;
}

void uninitializeItems(){
    free(itemDatas);
    free(itemPropertyDatas);
}

struct inventory *createInventory(int size){
    struct inventory *i = calloc(1, sizeof(*i) + size * sizeof(pitem));
    i->size = size;
    return i;
}

void freeInventory(struct inventory *i){
    free(i);
}

static double randomDouble(){
    return rand() / (double) RAND_MAX;
}

static void randomProperty(struct itemProperty *ip){
    ip->id = 0;
    struct itemPropertyData ipd = itemPropertyDatas[ip->id];
    switch(ipd.valueType){
    case IPVT_INT64:
        ip->value.i = rand() % (ipd.valueMax.i - ipd.valueMin.i) + ipd.valueMin.i;
    case IPVT_DOUBLE:
        ip->value.d = randomDouble() * (ipd.valueMax.d - ipd.valueMin.d) + ipd.valueMin.d;
    }
}

pitem createRandomItem(uid itemid){
    int numprops = itemDatas[itemid].baseNumProps;
    pitem item = malloc(sizeof(*item) + numprops * sizeof(struct itemProperty));
    item->globalid = itemGlobalID++;
    item->itemid = itemid;
    for(int i = 0; i < numprops; i++){
        randomProperty(&item->itemProperties[i]);
    }
    return item;
}

void freeItem(pitem i){
    free(i);
}

enum invError giveItem(struct inventory *inv, pitem newitem){
    if(!inv) return INVE_NOINV;
    for(int i = 0; i < inv->size; i++){
        pitem item = inv->items[i];
        if(item) continue;
        inv->items[i] = newitem;
        return INVE_SUCCESS;
    }
    return INVE_NOSPACE;
}
