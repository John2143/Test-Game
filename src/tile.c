#include "tile.h"

struct tile *tileDatas;

void loadTileTextures(){
    tileDatas = malloc(50 * sizeof(struct tile));

    tileDatas[0].texture = loadTexture(assetFolderPath "missing.png");
    tileDatas[1].texture = loadTexture(assetFolderPath "brick.png");
    tileDatas[2].texture = loadTexture(assetFolderPath "water.png");
    tileDatas[3].texture = loadTexture(assetFolderPath "dirt.png");
}

void unloadTileTextures(){
    free(tileDatas);
}
