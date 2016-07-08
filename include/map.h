#ifndef map_H
#define map_H

#include <string.h>
#include <math.h>

#include "tile.h"
#include "global.h"

#define WORLDSIZE 100
#define CLAMPWORLD(val) (MIN(MAX(val, 0), WORLDSIZE))

extern tileid gameworld[WORLDSIZE][WORLDSIZE];

void initializeWorld();
void uninitializeWorld();

#endif
