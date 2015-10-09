#ifndef graphics_H
#define graphics_H

#include <SDL2/SDL.h>
#include <GL/GLU.h>
#include <stdio.h>

#include "logic.h"
#include "font.h"

struct graphics{
	SDL_Window *window;
	SDL_GLContext *glcontext;
	int height;
	int width;
};

extern framerate fps;

void initiateGraphics(struct graphics *g, const char* name);
void destroyGraphics(struct graphics *g);
void renderGraphics(struct graphics *g);
int renderText(const char *text, struct font *f, int x, int y, int scale);
void setVSync(char vsync);

#endif
