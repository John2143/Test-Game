#ifndef tile_H
#define tile_H

//for textureID
#include <global.h>
#include <stdlib.h>

#include "global.h"

extern struct tile *globalTileArray;

typedef unsigned short tileid;
struct tile{
    textureID texture;
};

void loadTileTextures();
void unloadTileTextures();

#endif
