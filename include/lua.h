#ifndef lua_H
#define lua_H

#include <stdlib.h>

#include "entity.hpp"
#include "luaLoader.hpp"

extern lua_State *globalLuaState;

void luaStart();
void luaEnd();

#endif
