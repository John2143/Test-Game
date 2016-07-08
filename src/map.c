#include "map.h"

tileid gameworld[WORLDSIZE][WORLDSIZE];

void brush(int x, int y, int size, tileid id){
    for(int mx = x - size; mx < x + size; mx++){
        for(int my = y - size; my < y + size; my++){
            gameworld[CLAMPWORLD(mx)][CLAMPWORLD(my)] = id;
        }
    }
}

float normalRandomFloat(){
    return ((float) rand()) / ((float) RAND_MAX);
}

float randomFloat(float a, float b){
    float diff = b - a;
    return a + normalRandomFloat() * diff;
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

    /*int rivx = (rand() % WORLDSIZE), rivy = (rand() % WORLDSIZE);*/
    int rivx = 30, rivy = 30;
    float rivdir = normalRandomFloat() * PI2;
    for(int i = 0; i < WORLDSIZE; i++){
        brush(rivx, rivy, 3, 2);
        rivdir += randomFloat(-.5, .5);
        rivx += cos(rivdir);
        rivy += sin(rivdir);
    }
}

void uninitializeWorld(){
}
