#include "map.h"

World *World::currentWorld = nullptr;

World::World(size_t size): size(size){
    this->gameworld = new tileid[size * size];
}

World::~World(){
    delete[] this->gameworld;
}

tileid World::get(size_t x, size_t y){
    return this->gameworld[x + y * size];
}

tile &World::getd(size_t x, size_t y){
    return tileDatas[this->gameworld[x + y * size]];
}

void World::set(size_t x, size_t y, tileid id){
    this->gameworld[x + y * size] = id;
}

void World::makeCurrent(){
    World::currentWorld = this;
}

void World::initializeWorld(){
}
void World::uninitializeWorld(){
}
