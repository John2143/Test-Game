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

struct itemData{
    textureID texture;
    int baseNumProps;
    enum rarity baseRarity;
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

struct inventory{
    uint8_t size;
    pitem items[];
};

enum invError{
    INVE_SUCCESS,
    INVE_NOSPACE,
    INVE_NOINV,
};

extern struct itemData *itemDatas;
extern struct itemPropertyData *itemPropertyDatas;

void initializeItems();
void uninitializeItems();

enum invError giveItem(struct inventory *inv, pitem i);

#endif
