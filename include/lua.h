#ifndef lua_H
#define lua_H

#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>

#include <stdlib.h>

#include "logic.h"
#include "entity.h"

extern lua_State *globalLuaState;

void luaStart();
void luaEnd();

#endif
