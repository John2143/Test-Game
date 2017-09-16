/*
 * John Schmidt - john@john2143.com
 * Based off luna.h by Lenny Palozzi - lenny.palozzi@home.com
 *
 * modified to work in 5.3
 */

#ifndef luaLoader_H
#define luaLoader_H

extern "C" {
    #include <lua.h>
    #include <lauxlib.h>
    #include <lualib.h>
}

template <class T> class Luna{
public:
    struct RegType{
        const char *name;
        int (T::*member_function)(lua_State *L);
    };

    static int nameFunc(lua_State *L){
        lua_pop(L, 1);
        lua_pushstring(L, T::className);
        return 1;
    }

    static void Register(lua_State *L) {
        //Register the keyword as a global function that returns the userdata
        lua_pushcfunction(L, &Luna<T>::newT);
        lua_setglobal(L, T::className);

        luaL_newmetatable(L, T::className);

        //prototype methods
        lua_pushstring(L, "__index");
        lua_newtable(L);
        for(int i = 0; T::Register[i].name != NULL; i++){
            lua_pushstring(L, T::Register[i].name);
            lua_pushnumber(L, i);
            lua_pushcclosure(L, &Luna<T>::proxyFunc, 1);
            lua_settable(L, -3);
        }
        lua_settable(L, -3);

        //gc method
        lua_pushstring(L, "__gc");
        lua_pushcfunction(L, &Luna<T>::gc);
        lua_settable(L, -3);

        lua_pushstring(L, "__tostring");
        lua_pushcclosure(L, &Luna<T>::nameFunc, 0);
        lua_settable(L, -3);

        printf("Registered lua class '%s'\n", T::className);

        lua_pop(L, 1);
    }

private:
    static int proxyFunc(lua_State *L) {
        //Get method id from closure and the object
        int i = static_cast<int>(lua_tonumber(L, lua_upvalueindex(1)));

        //Get the object but keep it on the stack (to mimic 'return self')
        T *obj = getObjFromStack(L, 1);

        //Run proxied member function
        return (obj->*(T::Register[i].member_function))(L);
    }

    static int newT(lua_State* L) {
        T *obj = new T(L);

        //Create the userdata pointer that lua will actually get and assign it
        T **ptr = (T **) lua_newuserdata(L, sizeof(T*));
        *ptr = obj;
        luaL_setmetatable(L, T::className);

        //Return 1 new object
        return 1;
    }

    //Call deconstructors for proxy (which will call the other object deconstructor)
    static int gc(lua_State *L) {
        delete getObjFromStack(L, -1);
        return 0;
    }

    static T *getObjFromStack(lua_State *L, int i){
        return *static_cast<T**>(lua_touserdata(L, i));
    }

protected:
    Luna();
};

template <class T> class LunaClass{
protected:
    T *proxy;
};


#endif
