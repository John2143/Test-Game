#include "inventory.h"

int64_t globalID = 0;

Item::itemData *Item::itemDatas;
static size_t numItems;
void Item::initializeItems(){
    lua_getfield(L, -1, "data");
    lua_getfield(L, -1, "items");
    lua_len(L, -1);

    numItems = lua_tonumber(L, -1);
    lua_pop(L, 1);

    itemDatas = new Item::itemData[numItems];

    printf("Made %i items\n", numItems);

    for(size_t i = 0; i < numItems; i++){
        lua_geti(L, -1, i + 1);
        if(!lua_istable(L, -1)) throw "Incorrect table format in item datadata "_s;

#define defineField(name, logic) \
        lua_getfield(L, -1, #name); \
        itemDatas[i]. name = logic; \
        lua_pop(L, 1);

        defineField(texture, (textureID) lua_tonumber(L, -1));
        defineField(cooldown, lua_tonumber(L, -1));
        defineField(baseDamage, lua_tonumber(L, -1));
        defineField(abiCost, lua_tonumber(L, -1));
        lua_getfield(L, -1, "onUse");
        itemDatas[i].onUse = luaL_ref(L, LUA_REGISTRYINDEX);

        lua_pop(L, 1);
    }

    lua_pop(L, 2);
}

void Item::uninitializeItems(){
    delete[] Item::itemDatas;
}

Inventory::Inventory(int size): size(size){
    this->items = new Item *[size];
    for(int i = 0; i < size; i++){
        items[i] = nullptr;
    }
}

Inventory::~Inventory(){
    for(int i = 0; i < size; i++){
        if(items[i]) delete items[i];
    }
    delete this->items;
}

Item::Item(uid itemid){
    this->globalid = globalID++;
    this->itemid = itemid;
    this->lastUse = 0;
}

int Item::getDamage(){
    return this->getBaseData().baseDamage;
}

Item::itemData &Item::getBaseData(){
    return Item::itemDatas[this->itemid];
}

int Inventory::giveItem(Item *newitem){
    for(int i = 0; i < this->size; i++){
        Item *item = this->items[i];
        if(item) continue;

        this->items[i] = newitem;
        return i;
    }
    return INVE_NOSPACE;
}

int Inventory::moveItem(int slot, int newslot){
    if(slot >= this->size || slot < 0) return INVE_BADSLOT;
    if(newslot >= this->size || newslot < 0) return INVE_BADSLOT;

    Item *tmp = this->items[slot];
    this->items[slot] = this->items[newslot];
    this->items[newslot] = tmp;
    return 0;
}
