#ifndef map_H
#define map_H

#include "global.h"
#include "tile.h"
#include <cstring>

#define WORLDSIZE 2

extern tileid gameworld[WORLDSIZE][WORLDSIZE];

void initializeWorld();
void uninitializeWorld();

#endif
