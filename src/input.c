#include "input.h"

static uint32_t keysdown[MAXKEYSDOWN];

static short replaceFirst(uint32_t a, uint32_t b){
	short i;
	for(i = 0; i < MAXKEYSDOWN; i++){
		if(keysdown[i] == a){
			keysdown[i] = b;
			return i;
		}
	}
	return -1;
}

#ifdef DEBUG
static void printAll(){
	int i;
	for(i = 0; i < MAXKEYSDOWN; i++){
		if(keysdown[i]){
			printf("%.3u", keysdown[i]);
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
		(void) replaceFirst(0, code);
	}else{
		(void) replaceFirst(code, 0);
	}
#ifdef DEBUG
	printAll();
#endif
	if(onKeyPress)
		onKeyPress(code, state);
}

int isKeyPressed(uint32_t code){
	int i;
	for(i = 0; i < MAXKEYSDOWN; i++){
		if(keysdown[i] == code){
			return code;
		}
	}
	return 0;
}