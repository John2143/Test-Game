#ifndef tile_H
#define tile_H

#include "global.h"

extern struct tile *tileDatas;

typedef unsigned char tileid;
struct tile{
    textureID texture;
};

void loadTileTextures();
void unloadTileTextures();

#endif
