#ifndef graphics_H
#define graphics_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/GLU.h>
#include <stdio.h>

#include "entity.h" //knowing how to draw entities

struct graphics{
	SDL_Window *window;
	SDL_GLContext *glcontext;
	int height;
	int width;
};

void initiateGraphics(struct graphics *g, const char* name);
void destroyGraphics(struct graphics *g);
void renderGraphics(struct graphics *g);
int renderText(const char *text, int x, int y, int scale);
void setVSync(bool vsync);
GLuint loadTexture(const char *name);

#endif
