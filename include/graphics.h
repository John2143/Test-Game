#ifndef graphics_H
#define graphics_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <GL/GLU.h>
#include <stdio.h>

#include "entity.h"
#include "camera.h"
#include "tile.h"
#include "map.h"
#include "input.h"
#include "bullet.h"

#define TILEPIXELS (4 * 8)

typedef GLuint fontID;
struct font{
    fontID chars[128];
    unsigned char kerning[128];
    int bits;
};

extern struct font *globalFont;
struct font *loadFont(const char *name, int bits, int width);

struct graphics{
	SDL_Window *window;
	SDL_GLContext *glcontext;
	int width, height;
	int windowWidth, windowHeight;
};

void initiateGraphics(struct graphics *g, const char* name);
void destroyGraphics(struct graphics *g);
void renderGraphics(struct graphics *g, framerate frameTime, framerate appTime);
void setVSync(bool vsync);

void renderSquareTexture   (textureID textureid, int x, int y, int w, int h);
void renderSquareTextureRot(textureID textureid, int x, int y, int w, int h, angle ang);

textureID loadTextureFromSurface(SDL_Surface *texture);

enum justification{
    JUSTIFY_LEFT, JUSTIFY_RIGHT, JUSTIFY_CENTER
};

int renderChar    (const struct font *f, const unsigned char c, int x, int y, int scale);
int renderText    (const struct font *f, const char *text,      int x, int y, int scale);
int renderTextJust(const struct font *f, const char *text,      int x, int y, int scale, enum justification just);
int textLength    (const struct font *f, const char *text,                    int scale);

#endif
