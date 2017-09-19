#ifndef graphics_L_H
#define graphics_L_H

#include "graphics.h"
#include "luaLoader.h"

class Graphics_L: public LunaClass<Graphics>{
public:
    static const char className[];
    static typename Luna<Graphics_L>::RegType Register[];

    Graphics_L(lua_State *L){
        const char *name = lua_tostring(L, -3);
        int width = lua_tonumber(L, -2);
        int height = lua_tonumber(L, -1);
        proxy = new Graphics(name, width, height);
        lua_pop(L, 3);
    }
    ~Graphics_L(){
        delete proxy;
    }

    int renderTextJust(lua_State *L){
        const char *text = lua_tostring(L, -5);
        int x = lua_tonumber(L, -4);
        int y = lua_tonumber(L, -3);
        int scale = lua_tonumber(L, -2);
        int just = lua_tonumber(L, -1);
        proxy->renderTextJust(text, x, y, scale, (Graphics::justification) just);
        lua_pop(L, 5);
        return 1;
    }
    int renderEnd(lua_State *L){
        (void) L;
        proxy->renderEnd();
        return 1;
    }
    int renderStart(lua_State *L){
        (void) L;
        proxy->renderStart();
        return 1;
    }
    int color(lua_State *L){
        double r = lua_tonumber(L, -3);
        double g = lua_tonumber(L, -2);
        double b = lua_tonumber(L, -1);
        lua_pop(L, 3);
        glColor3f(r, g, b);
        return 1;
    }
};

const char Graphics_L::className[] = "Graphics";
Luna<Graphics_L>::RegType Graphics_L::Register[] = {
    {"renderTextJust", &Graphics_L::renderTextJust},
    {"renderStart", &Graphics_L::renderStart},
    {"renderEnd", &Graphics_L::renderEnd},
    {"color", &Graphics_L::color},
    {NULL, NULL}
};

#endif
