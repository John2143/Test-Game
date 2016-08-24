#include "map.h"

tileid gameworld[WORLDSIZE][WORLDSIZE];

void brush(int x, int y, int size, tileid id){
    for(int mx = x - size; mx < x + size; mx++){
        for(int my = y - size; my < y + size; my++){
            gameworld[CLAMPWORLD(mx)][CLAMPWORLD(my)] = id;
        }
    }
}

void initializeWorld(){
    /*memset(&gameworld, 0, sizeof(tileid) * WORLDSIZE * WORLDSIZE);*/
    for(int x = 0; x < WORLDSIZE; x++){
        for(int y = 0; y < WORLDSIZE; y++){
            gameworld[x][y] = 3;
        }
    }

    for(int i = 0; i < WORLDSIZE * WORLDSIZE / 3; i++){
        gameworld[rand() % WORLDSIZE][rand() % WORLDSIZE] = 1;
    }
}

void uninitializeWorld(){
}
