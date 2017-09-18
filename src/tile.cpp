#include "tile.h"

struct tile *tileDatas;

void loadTileTextures(){
    tileDatas = new tile[50];

    tileDatas[0].texture = loadTexture(assetFolderPath "missing.png");
    tileDatas[1].texture = loadTexture(assetFolderPath "brick.png");
    tileDatas[1].isSolid = 1;
    tileDatas[2].texture = loadTexture(assetFolderPath "water.png");
    tileDatas[3].texture = loadTexture(assetFolderPath "dark.png");
    tileDatas[4].texture = loadTexture(assetFolderPath "dirt.png");
}

void unloadTileTextures(){
    delete[] tileDatas;
}
