#include "logic.h"

void gameUpdate(framerate framems){
	pos += framems;
	pos = fmod(pos, 2*PI);
}
void initLogic(){
	pos = 0;
}
