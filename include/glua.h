#ifndef lua_H
#define lua_H

#include "global.h"

#include <cstring>
#include <cstdlib>
extern lua_State *L;

namespace Lua{
    int callGameFunc(const char *name, int nargs, int nret);
    void stackDump();
    int start();
    int end();
}

#endif
