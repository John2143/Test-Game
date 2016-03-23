#include <stdio.h>
#include <stdlib.h>

static unsigned long clockDivisor;

#ifdef WIN32
#	include <Windows.h>
#	define thread DWORD
#	define newThread(f, arg, id) CreateThread(NULL, 0, f, arg, 0, id)
#	define threadRType DWORD WINAPI
#	define threadExit() return 0;
#	define clockType LARGE_INTEGER
#else
#	include <pthread.h>
#	include <time.h>
#	define thread pthread
#	define newThread(f, arg, id) pthread_create(id, NULL, f, arg)
#	define threadRType void*
#	define threadExit() pthread_exit(NULL);
#	define clockType int
#endif

#include <SDL2/SDL.h>

#include "input.h"
#include "graphics.h"
#include "logic.h"
#include "global.h"
#include "entity.h"

/*static char isVSync = 0;*/
/*static void toggleVSync(uint32_t code, char pressed){*/
	/*(void) code;*/
	/*if(pressed){*/
		/*setVSync(isVSync = !isVSync);*/
		/*printf("\rVSync: %i\n", isVSync);*/
	/*}*/
/*}*/

static void getClockTime(clockType *val){
#ifdef WIN32
	QueryPerformanceCounter(val);
#else
	*val = clock();
#endif
}

static framerate getDiffClock(clockType a, clockType b){
#ifdef WIN32
	long diff = b.QuadPart - a.QuadPart;
	return diff / (double) clockDivisor;
#else
	return (b - a) / (double) clockDivisor;
#endif
}

framerate fps;

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;
#ifdef WIN32
	{
		LARGE_INTEGER divisor;
		QueryPerformanceFrequency(&divisor);
		clockDivisor = divisor.QuadPart;
	}
#else
	clockDivisor = CLOCKS_PER_SEC;
#endif
	printf("Starting\nClock divisor: %lu\n", clockDivisor);
	struct graphics g;
	SDL_Event event;

	initiateGraphics(&g, "Test window");
	if(!g.window) goto CLEANUP;
	/*thread t;*/
	/*newThread(test, NULL, &t);*/

	clockType frameStart, frameEnd;
	framerate frameTime = 0;

	getClockTime(&frameStart);
	initLogic();
	while(1){
		if(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_QUIT:
				goto CLEANUP;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
                if(event.key.keysym.scancode == 0x14) goto CLEANUP; //TODO
				keyEvent(event.key);
				break;
			}
		}
		renderGraphics(&g);

		getClockTime(&frameEnd);

		frameTime = getDiffClock(frameStart, frameEnd);
		fps = 1 / frameTime;
		frameStart = frameEnd;

		gameUpdate(frameTime);
	}
CLEANUP:
	destroyGraphics(&g);

	return 0;
}
