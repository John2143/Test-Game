#ifndef global_H
#define global_H

#include <GL/GLU.h>

typedef double framerate;

typedef GLuint textureID;
extern textureID loadTexture(const char *name); //Defined in graphics.c

#define SIN45 0.70710678118
#define PI 3.1415926
#define PI2 (PI * 2)
#define assetFolder "assets"
#define assetFolderPath assetFolder "/"
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef enum {false, true} bool;
typedef unsigned char uchar;

#endif
