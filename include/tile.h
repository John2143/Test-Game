#ifndef tile_H
#define tile_H

#include "global.h"

#define TILESIZE 32

extern struct tile *tileDatas;

typedef unsigned char tileid;
struct tile{
    textureID texture;
    bool isSolid;
};

void loadTileTextures();
void unloadTileTextures();

#endif
