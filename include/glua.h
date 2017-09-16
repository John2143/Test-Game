#ifndef lua_H
#define lua_H

#include "global.h"

#include <cstring>
#include <cstdlib>

extern lua_State *L;

int callLuaGameFunc(const char *name, int nargs, int nret);
void luaStackDump();
int luaStart();
int luaEnd();

#endif
