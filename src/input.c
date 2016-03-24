#include "input.h"

uint32_t keysdown[MAXKEYSDOWN];

static int replaceFirst(uint32_t a, uint32_t b){
	for(int i = 0; i < MAXKEYSDOWN; i++){
		if(keysdown[i] == a){
			keysdown[i] = b;
			return i;
		}
	}
	return -1;
}

#ifdef DEBUG
static void printAll(){
	for(int i = 0; i < MAXKEYSDOWN; i++){
		if(keysdown[i]){
			printf("%.3x", keysdown[i]);
		}else{
			printf("...");
		}
	}
	printf("\n");
}
#endif

void keyEvent(SDL_KeyboardEvent key){
	if(key.repeat > 0) return;
	uint32_t code = key.keysym.scancode;
	int state = key.state == SDL_PRESSED;

	if(state){
		replaceFirst(0, code);
	}else{
		replaceFirst(code, 0);
	}
#ifdef DEBUG
    printAll();
#endif
}

int isKeyPressed(uint32_t code){
	for(int i = 0; i < MAXKEYSDOWN; i++){
		if(keysdown[i] == code){
			return code;
		}
	}
	return 0;
}
