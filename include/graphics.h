#ifndef graphics_H
#define graphics_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/GLU.h>
#include <stdio.h>

#include "logic.h"

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
int renderText(const char *text, int x, int y, int scale);
void setVSync(char vsync);
GLuint loadTexture(const char *name);

#endif
