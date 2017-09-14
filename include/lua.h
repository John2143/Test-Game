#ifndef lua_H
#define lua_H

#include <stdlib.h>

#include "logic.h"
#include "entity.h"
#include "map.h"
#include "luaLoader.hpp"

extern lua_State *globalLuaState;

void luaStart();
void luaEnd();

#endif
