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
    uid id;
    union itemPropertyValue value;
};

typedef struct item{
    int64_t globalid;
    uid itemid;
    int numprops;
    framerate lastUse;
    struct itemProperty itemProperties[];
} *pitem;

class Entity;
typedef int (*itemUseFunction)(Entity &ent, pitem it);

struct itemData{
    textureID texture;
    int baseNumProps;
    framerate cooldown;
    enum rarity baseRarity;
    int baseDamage;
    itemUseFunction onUse;
    int abiCost;
};

struct inventory{
    uint8_t size;
    pitem items[];
};

enum invError{
    INVE_NONE = 0,
    INVE_NOSPACE = -1,
    INVE_NOINV = -2,
    INVE_BADSLOT = -3,
    INVE_NOITEM = -4,
    INVE_NOFUNC = -5,
    INVE_DONTCOOLDOWN = -6,
    INVE_ONCOOLDOWN = -7,
    INVE_NOABI = -8,
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
struct itemProperty *getItemProperty(pitem it, uid propid);

#endif
