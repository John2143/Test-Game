#ifndef font_H
#define font_H

#include <GL/GLU.h>
#include <GL/GL.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ASCII_RANGE 128

struct font {
	char monospace;
	char *width; //actually a number
	unsigned int textures[ASCII_RANGE];
};

int renderChar(struct font *font, const unsigned char c, int x, int y, int scale);
struct font *loadFont(const char* path);
void freeFont(struct font *f);
char getWidth(struct font *font, const unsigned char c);
char getHeight(struct font *font, const unsigned char c);

#endif
