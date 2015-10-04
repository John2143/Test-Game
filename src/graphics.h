#ifndef graphics_H
#define graphics_H

#include <SDL2/SDL.h>
#include <GL/GLU.h>
#include <stdio.h>

struct graphics{
	SDL_Window *window;
	SDL_GLContext *glcontext;
};

void initiateGraphics(struct graphics *g, const char* name);
void destroyGraphics(struct graphics *g);
void renderGraphics(struct graphics *g);

#endif
