#ifndef inventory_H
#define inventory_H

#include "global.h"

enum rarity{
    RARITY_BASIC,
    RARITY_SPECIAL,
    RARITY_RARE,
    RARITY_ULTRARARE,
    RARITY_UNIQUE,
};

enum itemPropertyValueType{
    IPVT_DOUBLE,
    IPVT_INT64,
};

union itemPropertyValue{
    int64_t i;
    double d;
};


struct itemPropertyData{
    const char *formatStr;
    enum itemPropertyValueType valueType;
    union itemPropertyValue valueMin;
    union itemPropertyValue valueMax;
};

struct itemProperty{
    int id;
    union itemPropertyValue value;
};

typedef struct item{
    int64_t globalid;
    uid itemid;

    int numprops;
    struct itemProperty itemProperties[];
} *pitem;

typedef struct entity *pent;
typedef int (*itemUseFunction)(pent ent, pitem it);

struct itemData{
    textureID texture;
    int baseNumProps;
    enum rarity baseRarity;
    itemUseFunction onUse;
};

struct inventory{
    uint8_t size;
    pitem items[];
};

enum invError{
    INVE_NOSPACE = -1,
    INVE_NOINV = -2,
    INVE_BADSLOT = -3,
    INVE_NOITEM = -4,
    INVE_NOFUNC = -5,
};

extern struct itemData *itemDatas;
extern struct itemPropertyData *itemPropertyDatas;

void initializeItems();
void uninitializeItems();

struct inventory *createInventory(int size);
void freeInventory(struct inventory *i);
pitem createRandomItem(uid itemid);
void freeItem(pitem i);
//returns slot # or error
int giveItem(struct inventory *inv, pitem newitem);
int moveItem(struct inventory *inv, int slot, int newslot);

#endif
