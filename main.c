#include "lua.h"
#define OMNI_IMPLEMENTATION
#include "omni.h"

#include "raylib_bindings.h"

#include <lauxlib.h>
#include <lualib.h>
#include <raylib.h>

lua_State *L;

int main() {
    L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "Failed to initialize Lua\n");
        exit(1);
    }
    luaL_openlibs(L);

    pushLuaFunctions(L);

    if (luaL_dofile(L, "script.lua") != LUA_OK) {
        fprintf(stderr, "Failed to load script.lua: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    return 0;
}
