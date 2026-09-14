#include "lua.h"
#include <stdio.h>
#define OMNI_IMPLEMENTATION
#include "omni.h"

#include "raylib_bindings.h"

#include <lauxlib.h>
#include <lualib.h>
#include <raylib.h>

#include <libgen.h>
#include <limits.h>
#include <unistd.h>

lua_State *L;

// Resolves the running executable's own directory and chdir()s into it, so
// that every relative path used from here on (script.lua, asset files
// loaded from Lua, require()'d modules) resolves consistently against the
// game's own folder -- regardless of what directory the binary was actually
// launched from.
static void chdirToExecutableDir(const char *argv0) {
    char resolved[PATH_MAX];
    if (realpath(argv0, resolved) == NULL) {
        fprintf(stderr,
                "Warning: could not resolve executable path (%s), using "
                "current directory\n",
                argv0);
        return;
    }

    // dirname() may modify `resolved` and/or return a pointer into it or
    // into static storage -- safe here since we use it immediately and
    // don't touch `resolved` again afterward.
    char *dir = dirname(resolved);
    if (chdir(dir) != 0) {
        fprintf(stderr, "Warning: could not chdir to %s\n", dir);
        return;
    }

    // The executable lived in build/, one level below the project root
    // where game/, assets/, etc. live -- step up once more
    if (chdir("..") != 0) {
        fprintf(stderr, "Warning: could not chdir to project root\n");
    }
}

// Message handler passed to lua_pcall: turns a bare error message into a
// full Lua stack trace (function names + line numbers up the call chain),
// instead of just the final error string.
static int messageHandler(lua_State *L) {
    const char *msg = lua_tostring(L, 1);
    if (msg == NULL) {
        if (luaL_callmeta(L, 1, "__tostring") &&
            lua_type(L, -1) == LUA_TSTRING) {
            return 1;
        }
        msg = lua_pushfstring(
            L, "(error object is a %s value)", luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);
    return 1;
}

int main(int argc, char *argv[]) {
    chdirToExecutableDir(argv[0]);

    L = luaL_newstate();
    if (L == NULL) {
        fprintf(stderr, "Failed to initialize Lua\n");
        exit(1);
    }
    luaL_openlibs(L);

    // Make sure require("module") looks next to script.lua even if the
    // platform's default package.path doesn't already include "./?.lua".
    luaL_dostring(
        L, "package.path = './game/?.lua;./game/?/init.lua;' .. package.path");

    pushLuaFunctions(L);

    lua_pushcfunction(L, messageHandler);
    int messageHandlerIndex = lua_gettop(L);

    if (luaL_loadfile(L, "game/script.lua") != LUA_OK) {
        fprintf(stderr,
                "Failed to load game/script.lua: %s\n",
                lua_tostring(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    if (lua_pcall(L, 0, 0, messageHandlerIndex) != LUA_OK) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        return 1;
    }

    return 0;
}
