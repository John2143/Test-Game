#include "inventory.h"

int64_t globalID = 0;

Item::itemData *Item::itemDatas;
void Item::initializeItems(){
    Item::itemDatas = new Item::itemData[8];
    Item::itemDatas[0].texture = loadTexture(assetFolderPath "brick.png");
    Item::itemDatas[0].cooldown = 1.0;
    Item::itemDatas[0].baseDamage = 5;
    Item::itemDatas[0].abiCost = 10;

    Item::itemDatas[1].texture = loadTexture(assetFolderPath "water.png");
    Item::itemDatas[1].cooldown = .3;
    Item::itemDatas[1].baseDamage = 5;
    Item::itemDatas[1].abiCost = 0;
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
