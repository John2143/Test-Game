#ifndef inventory_H
#define inventory_H

#include "global.h"

class Entity;

class Item{
public:
    typedef bool (itemUseFunction)(Entity &ent, Item &it);

    //Todo distinction between usable and nonusable items
    static struct itemData{
        textureID texture;
        framerate cooldown;
        int baseDamage;
        itemUseFunction *onUse;
        int abiCost;
    } *itemDatas;

    int64_t globalid;
    uid itemid;

    framerate lastUse;

    static void initializeItems();
    static void uninitializeItems();

    Item(uid id);

    int getDamage();
    itemData &getBaseData();
    //return if you can use it
};

class Inventory{
public:
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

    uint8_t size;
    Item **items;

    Inventory(int size);
    ~Inventory();

    //returns slot # or error
    int giveItem(Item *newitem);
    int moveItem(int slot, int newslot);
};

#endif
