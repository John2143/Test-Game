static unsigned long clockDivisor;

#define WIN32 _WIN32
#ifdef WIN32
#    include <Windows.h>
#    define thread DWORD
#    define newThread(f, arg, id) CreateThread(NULL, 0, f, arg, 0, id)
#    define threadRType DWORD WINAPI
#    define threadExit() return 0;
#    define clockType LARGE_INTEGER
#else
#    include <pthread.h>
#    define thread pthread
#    define newThread(f, arg, id) pthread_create(id, NULL, f, arg)
#    define threadRType void*
#    define threadExit() pthread_exit(NULL);
#    define clockType int
#endif

#include <time.h>

#include "global.h"
#include "entity.h"
#include "input.h"
#include "graphics.h"
#include "logic.h"
#include "glua.h"
#include "map.h"
#include "tile.h"
#include "bullet.h"
#include "inventory.h"

framerate frameTime = 0, appTime = 0;

static void getClockTime(clockType *val){
#ifdef WIN32
    QueryPerformanceCounter(val);
#else
    *val = clock();
#endif
}

std::string operator "" _s(const char *t, size_t s){
    return std::string(t, s);
}

static framerate getDiffClock(clockType a, clockType b){
#ifdef WIN32
    long diff = b.QuadPart - a.QuadPart;
    return diff / (double) clockDivisor;
#else
    return (b - a) / (double) clockDivisor;
#endif
}

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;
    srand(time(NULL));
#ifdef WIN32
    {
        LARGE_INTEGER divisor;
        QueryPerformanceFrequency(&divisor);
        clockDivisor = divisor.QuadPart;
    }
#else
    clockDivisor = CLOCKS_PER_SEC;
#endif
    printf("Starting\nClock divisor: %lu\n", clockDivisor);

    graphics g;
    g.height = 900;
    g.width = 1200;

    /*thread t;*/
    /*newThread(test, NULL, &t);*/

    clockType frameStart, frameEnd, gameStart;
    getClockTime(&frameStart);

    if(Lua::start()){
        printf("Lua could not initialize so quitting now\n");
        goto LUA_FAIL;
    }

    initiateGraphics(&g, "Test window");
    if(!g.window) goto CLEANUP;

    Lua::callGameFunc("preInit", 0, 0);

    initLogic();
    Entity::loadData();
    loadTileTextures();
    initializeWorld();
    Bullet::initializeBullets();
    Item::initializeItems();
    Lua::callGameFunc("postInit", 0, 0);

    fflush(stdout);

    getClockTime(&gameStart);

    while(1){
        cameraTick();
        renderGraphics(&g);

        oldMouseState = mouseState;
        mouseState = SDL_GetMouseState(&mouseX, &mouseY);
        if(oldMouseState != mouseState){
            mouseEvent();
        }
        for(SDL_Event event; SDL_PollEvent(&event);){
            switch(event.type){
            case SDL_QUIT:
                goto CLEANUP;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if(event.key.keysym.scancode == 0x14) goto CLEANUP; //TODO
                keyEvent(event.key);
            break;
            }
        }

        getClockTime(&frameEnd);
        frameTime = getDiffClock(frameStart, frameEnd);
        appTime = getDiffClock(gameStart, frameEnd);
        frameStart = frameEnd;
        gameUpdate();
    }
CLEANUP:

    Lua::callGameFunc("preExit", 0, 0);
    Item::uninitializeItems();
    Bullet::uninitializeBullets();
    uninitializeWorld();
    unloadTileTextures();
    Entity::unloadData();
    destroyGraphics(&g);
    Lua::callGameFunc("postExit", 0, 0);

LUA_FAIL:
    Lua::end();

    return 0;
}

float normalRandomFloat(){
    return ((float) rand()) / ((float) RAND_MAX);
}

float randomFloat(float a, float b){
    float diff = b - a;
    return a + normalRandomFloat() * diff;
}

#define safePutChar(buf, c) \
    *buf++ = c; if(++printsize == buflength) goto terminateBuf;

size_t gsprintf(char *buf, size_t buflength, const char *fmt, ...){
    va_list vargs;
    size_t printsize = 1; //Include null
    va_start(vargs, fmt);
    for(; *fmt != '\0'; fmt++){
        if(*fmt != '%'){
            safePutChar(buf, *fmt);
            continue;
        }
        switch(*fmt){
        }
    }
terminateBuf:
    *buf = '\0';
    va_end(vargs);
    return printsize;
}
