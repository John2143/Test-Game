#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#	include <Windows.h>
#	define thread DWORD
#	define newThread(f, arg, id) CreateThread(NULL, 0, f, arg, 0, id)
#	define threadRType DWORD WINAPI
#	define threadExit() return 0;
#	define clockType LARGE_INTEGER
static LARGE_INTEGER clockDivisor;
#else
#	include <pthread.h>
#	define thread pthread
#	define newThread(f, arg, id) pthread_create(id, NULL, f, arg)
#	define threadRType void*
#	define threadExit() pthread_exit(NULL);
#	define clockType int
static LARGE_INTEGER clockDivisor = CLOCKS_PER_SEC;
#endif

#include <SDL2/SDL.h>

#include "input.h"
#include "graphics.h"
#include "logic.h"

/*const float g_sin45 = 0.70710678118f;*/

typedef float framerate;

threadRType test(void *args){
	(void) args;
	printf("WOW\n");
	threadExit();
}

static void getClockTime(clockType *val){
#ifdef WIN32
	QueryPerformanceCounter(val);
#else
	*val = clock();
#endif
}

static double getDiffClock(clockType a, clockType b){
#ifdef WIN32
	return (double) ((b.QuadPart - a.QuadPart) / clockDivisor.QuadPart);
#else
	return (double) (b - a) / clockDivisortoms;
#endif
}

int main(int argc, char** argv) {
	(void) argc;
	(void) argv;
#ifdef WIN32
	QueryPerformanceFrequency(&clockDivisor);
#endif

	printf("Starting\n");
	struct graphics g;
	SDL_Event event;

	const framerate FPS = 144;
	(void) FPS;

	initiateGraphics(&g, "Test window");
	if(!g.window) goto CLEANUP;
	thread t;
	newThread(test, NULL, &t);

	clockType frameStart, frameEnd;
	framerate frameTime = 0;

	getClockTime(&frameStart);
	while(1){
		if(SDL_PollEvent(&event)){
			switch(event.type){
			case SDL_QUIT:
				goto CLEANUP;
			case SDL_KEYDOWN:
			case SDL_KEYUP:
				keyEvent(event.key);
				break;
			}
		}
		renderGraphics(&g);

		/*clock_gettime(CLOCK_MONOTONIC_RAW, &frameEnd);*/

		/*if(framems < MAXFRAMETIE)*/
			/*framems = MAXFRAMETIME;*/

		getClockTime(&frameStart);
		
		frameTime = getDiffClock(frameStart, frameEnd);
		gameUpdate(frameTime);

		frameStart = frameEnd;
	}
CLEANUP:
	destroyGraphics(&g);

	return 0;
}
