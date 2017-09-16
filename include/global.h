#ifndef global_H
#define global_H

#include "stl.h"

std::string operator "" _s(const char *t, size_t s){
    return std::string(t, s);
}

typedef double framerate;
typedef double stattype;
extern framerate appTime, frameTime;

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
#define STRUCTARRAYLEN(arr) (sizeof(arr)/sizeof(*arr))

#define GSPR_NONE (1 << 32 - 1)
size_t gsprintf(char *buf, size_t buflength, const char *fmt, ...);
#define gprintf(buf, ...) gsprintf(buf, GSPR_NONE, ...)

extern float normalRandomFloat();
extern float randomFloat(float a, float b);

#endif
