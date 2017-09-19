#ifndef graphics_H
#define graphics_H

#include "global.h"

#include "entity.h"
#include "camera.h"
#include "tile.h"
#include "map.h"
#include "input.h"
#include "bullet.h"
#include "inventory.h"
#include "glua.h"

#define TILEPIXELS TILESIZE
#define KERNINGSIZE 128

typedef GLuint fontID;
class Font{
public:
    fontID chars[128];
    unsigned char kerning[128];
    int bits;

    Font(const char *name, int bits, int width);
    ~Font();
};

class Graphics{
public:
    enum justification{
        JUSTIFY_LEFT = 0, JUSTIFY_RIGHT = 1, JUSTIFY_CENTER = 2
    };

    SDL_Window *window;
    SDL_GLContext glcontext;
    int width, height;
    int windowWidth, windowHeight;

    Graphics(const char *name, int width, int height);
    ~Graphics();

    void renderStart();
    void renderEnd();

    static textureID loadTextureFromSurface(SDL_Surface *texture);

    static void createLua();

    int renderTextJust(const char *text,      int x, int y, int scale, justification just);

private:
    void renderSquareTexture   (textureID textureid, int x, int y, int w, int h);
    void renderSquareTextureRot(textureID textureid, int x, int y, int w, int h, angle ang);

    int renderChar    (Font *f, const unsigned char c, int x, int y, int scale);
    int renderText    (Font *f, const char *text,      int x, int y, int scale);
    int renderTextJust(Font *f, const char *text,      int x, int y, int scale, justification just);
    int textLength    (Font *f, const char *text,                    int scale);


    void renderMap();
    void renderWorld2D();
    void renderStatusBar();
    void renderInterface();
};

#endif
