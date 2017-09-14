#include "map.h"

tileid gameworld[WORLDSIZE][WORLDSIZE];

void initializeWorld(){
    memset(&gameworld, 0, sizeof(tileid) * WORLDSIZE * WORLDSIZE);
}

void uninitializeWorld(){
}
