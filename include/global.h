#ifndef global_H
#define global_H

#include "stl.h"

typedef double framerate;

typedef double position;
typedef float angle;
typedef uint32_t uid;

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

#endif
