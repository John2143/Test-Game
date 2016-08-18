#ifndef map_H
#define map_H

#include "global.h"
#include "tile.h"

#define WORLDSIZE 100
#define CLAMPWORLD(val) (MIN(MAX(val, 0), WORLDSIZE))

extern tileid gameworld[WORLDSIZE][WORLDSIZE];

void initializeWorld();
void uninitializeWorld();

#endif
