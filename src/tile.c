#include "tile.h"

struct tile *globalTileArray;

void loadTileTextures(){
    globalTileArray = malloc(50 * sizeof(struct tile));

    globalTileArray[0].texture = loadTexture(assetFolderPath "missing.png");
    globalTileArray[1].texture = loadTexture(assetFolderPath "brick.png");
    globalTileArray[2].texture = loadTexture(assetFolderPath "water.png");
    globalTileArray[3].texture = loadTexture(assetFolderPath "dirt.png");
}

void unloadTileTextures(){
    free(globalTileArray);
}
