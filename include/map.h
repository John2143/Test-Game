#ifndef map_H
#define map_H

#include "global.h"
#include "tile.h"

class World{
public:
    static World *currentWorld;

    size_t size;
    tileid *gameworld;

    World(size_t size);
    ~World();

    tileid get(size_t x, size_t y);
    tile &getd(size_t x, size_t y);
    void set(size_t x, size_t y, tileid id);
    void makeCurrent();

    static void initializeWorld();
    static void uninitializeWorld();
};

#endif
