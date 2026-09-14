#pragma once

#include "lua.h"
#include <lauxlib.h>
#include <lualib.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static Color lua_checkColor(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "r");
    int r = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "g");
    int g = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "b");
    int b = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "a");
    int a = luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    return (Color){.r = r, .g = g, .b = b, .a = a};
}

static void pushColor(lua_State *L, Color color) {
    lua_createtable(L, 0, 4);

    lua_pushinteger(L, color.r);
    lua_setfield(L, -2, "r");

    lua_pushinteger(L, color.g);
    lua_setfield(L, -2, "g");

    lua_pushinteger(L, color.b);
    lua_setfield(L, -2, "b");

    lua_pushinteger(L, color.a);
    lua_setfield(L, -2, "a");
}

static Vector2 lua_checkVector2(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "x");
    float x = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    float y = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return (Vector2){.x = x, .y = y};
}

#define VECTOR2_METATABLE "Vector2"

static void pushVector2(lua_State *L, Vector2 v) {
    lua_createtable(L, 0, 2);

    lua_pushnumber(L, v.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, v.y);
    lua_setfield(L, -2, "y");

    luaL_getmetatable(L, VECTOR2_METATABLE);
    lua_setmetatable(L, -2);
}

static Vector3 lua_checkVector3(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "x");
    float x = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    float y = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    float z = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return (Vector3){.x = x, .y = y, .z = z};
}

#define VECTOR3_METATABLE "Vector3"

static void pushVector3(lua_State *L, Vector3 v) {
    lua_createtable(L, 0, 3);

    lua_pushnumber(L, v.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, v.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, v.z);
    lua_setfield(L, -2, "z");

    luaL_getmetatable(L, VECTOR3_METATABLE);
    lua_setmetatable(L, -2);
}

static Vector4 lua_checkVector4(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "x");
    float x = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    float y = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "z");
    float z = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "w");
    float w = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return (Vector4){.x = x, .y = y, .z = z, .w = w};
}

#define VECTOR4_METATABLE "Vector4"

static void pushVector4(lua_State *L, Vector4 v) {
    lua_createtable(L, 0, 4);

    lua_pushnumber(L, v.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, v.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, v.z);
    lua_setfield(L, -2, "z");

    lua_pushnumber(L, v.w);
    lua_setfield(L, -2, "w");

    luaL_getmetatable(L, VECTOR4_METATABLE);
    lua_setmetatable(L, -2);
}

// Matrix is a plain table of its 16 raw fields (m0..m15, matching raylib's
// C struct field names exactly) -- same "value type as table" treatment as
// Color/Vector2/Rectangle, since a Matrix owns no GPU/CPU resource.
static Matrix lua_checkMatrix(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    Matrix mat;
    const char *fields[16] = {
        "m0", "m1", "m2",  "m3",  "m4",  "m5",  "m6",  "m7",
        "m8", "m9", "m10", "m11", "m12", "m13", "m14", "m15"};
    float *slots[16] = {
        &mat.m0, &mat.m1, &mat.m2,  &mat.m3,  &mat.m4,  &mat.m5,  &mat.m6,
        &mat.m7, &mat.m8, &mat.m9,  &mat.m10, &mat.m11, &mat.m12, &mat.m13,
        &mat.m14, &mat.m15};

    for (int i = 0; i < 16; i++) {
        lua_getfield(L, index, fields[i]);
        *slots[i] = (float)luaL_checknumber(L, -1);
        lua_pop(L, 1);
    }

    return mat;
}

#define MATRIX_METATABLE "Matrix"

static void pushMatrix(lua_State *L, Matrix mat) {
    lua_createtable(L, 0, 16);

    lua_pushnumber(L, mat.m0);
    lua_setfield(L, -2, "m0");
    lua_pushnumber(L, mat.m1);
    lua_setfield(L, -2, "m1");
    lua_pushnumber(L, mat.m2);
    lua_setfield(L, -2, "m2");
    lua_pushnumber(L, mat.m3);
    lua_setfield(L, -2, "m3");
    lua_pushnumber(L, mat.m4);
    lua_setfield(L, -2, "m4");
    lua_pushnumber(L, mat.m5);
    lua_setfield(L, -2, "m5");
    lua_pushnumber(L, mat.m6);
    lua_setfield(L, -2, "m6");
    lua_pushnumber(L, mat.m7);
    lua_setfield(L, -2, "m7");
    lua_pushnumber(L, mat.m8);
    lua_setfield(L, -2, "m8");
    lua_pushnumber(L, mat.m9);
    lua_setfield(L, -2, "m9");
    lua_pushnumber(L, mat.m10);
    lua_setfield(L, -2, "m10");
    lua_pushnumber(L, mat.m11);
    lua_setfield(L, -2, "m11");
    lua_pushnumber(L, mat.m12);
    lua_setfield(L, -2, "m12");
    lua_pushnumber(L, mat.m13);
    lua_setfield(L, -2, "m13");
    lua_pushnumber(L, mat.m14);
    lua_setfield(L, -2, "m14");
    lua_pushnumber(L, mat.m15);
    lua_setfield(L, -2, "m15");

    luaL_getmetatable(L, MATRIX_METATABLE);
    lua_setmetatable(L, -2);
}

static Rectangle lua_checkRectangle(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "x");
    float x = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "y");
    float y = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "width");
    float width = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "height");
    float height = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return (Rectangle){.x = x, .y = y, .width = width, .height = height};
}

static void pushRectangle(lua_State *L, Rectangle r) {
    lua_createtable(L, 0, 4);

    lua_pushnumber(L, r.x);
    lua_setfield(L, -2, "x");

    lua_pushnumber(L, r.y);
    lua_setfield(L, -2, "y");

    lua_pushnumber(L, r.width);
    lua_setfield(L, -2, "width");

    lua_pushnumber(L, r.height);
    lua_setfield(L, -2, "height");
}

static Texture2D lua_checkTexture2D(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "id");
    unsigned int id = (unsigned int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "width");
    int width = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "height");
    int height = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "mipmaps");
    int mipmaps = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "format");
    int format = (int)luaL_checkinteger(L, -1);
    lua_pop(L, 1);

    return (Texture2D){.id      = id,
                       .width   = width,
                       .height  = height,
                       .mipmaps = mipmaps,
                       .format  = format};
}

static void pushTexture2D(lua_State *L, Texture2D t) {
    lua_createtable(L, 0, 5);

    lua_pushinteger(L, t.id);
    lua_setfield(L, -2, "id");

    lua_pushinteger(L, t.width);
    lua_setfield(L, -2, "width");

    lua_pushinteger(L, t.height);
    lua_setfield(L, -2, "height");

    lua_pushinteger(L, t.mipmaps);
    lua_setfield(L, -2, "mipmaps");

    lua_pushinteger(L, t.format);
    lua_setfield(L, -2, "format");
}

static Camera2D lua_checkCamera2D(lua_State *L, int index) {
    luaL_checktype(L, index, LUA_TTABLE);

    lua_getfield(L, index, "offset");
    Vector2 offset = lua_checkVector2(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "target");
    Vector2 target = lua_checkVector2(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "rotation");
    float rotation = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, index, "zoom");
    float zoom = (float)luaL_checknumber(L, -1);
    lua_pop(L, 1);

    return (Camera2D){
        .offset = offset, .target = target, .rotation = rotation, .zoom = zoom};
}

static void pushCamera2D(lua_State *L, Camera2D c) {
    lua_createtable(L, 0, 4);

    pushVector2(L, c.offset);
    lua_setfield(L, -2, "offset");

    pushVector2(L, c.target);
    lua_setfield(L, -2, "target");

    lua_pushnumber(L, c.rotation);
    lua_setfield(L, -2, "rotation");

    lua_pushnumber(L, c.zoom);
    lua_setfield(L, -2, "zoom");
}

#define SOUND_METATABLE "Sound"
#define MUSIC_METATABLE "Music"

static Sound *lua_checkSound(lua_State *L, int index) {
    return (Sound *)luaL_checkudata(L, index, SOUND_METATABLE);
}

static Music *lua_checkMusic(lua_State *L, int index) {
    return (Music *)luaL_checkudata(L, index, MUSIC_METATABLE);
}

#define FONT_METATABLE "Font"
#define RENDERTEXTURE2D_METATABLE "RenderTexture2D"

static Font *lua_checkFont(lua_State *L, int index) {
    return (Font *)luaL_checkudata(L, index, FONT_METATABLE);
}

#define SHADER_METATABLE "Shader"

static Shader *lua_checkShader(lua_State *L, int index) {
    return (Shader *)luaL_checkudata(L, index, SHADER_METATABLE);
}

static RenderTexture2D *lua_checkRenderTexture2D(lua_State *L, int index) {
    return (RenderTexture2D *)luaL_checkudata(
        L, index, RENDERTEXTURE2D_METATABLE);
}

// RenderTexture2D exposes its color/depth attachments as read-only fields
// (target.texture / target.depth) via this __index metamethod, since the
// userdata itself only stores the raw C struct.
static int lua_RenderTexture2DIndex_mm(lua_State *L) {
    RenderTexture2D *rt  = lua_checkRenderTexture2D(L, 1);
    const char      *key = luaL_checkstring(L, 2);

    if (strcmp(key, "texture") == 0) {
        pushTexture2D(L, rt->texture);
        return 1;
    }
    if (strcmp(key, "depth") == 0) {
        pushTexture2D(L, rt->depth);
        return 1;
    }
    if (strcmp(key, "id") == 0) {
        lua_pushinteger(L, rt->id);
        return 1;
    }
    return 0;
}

static void registerRenderTexture2DMetatable(lua_State *L) {
    luaL_newmetatable(L, RENDERTEXTURE2D_METATABLE);
    lua_pushcfunction(L, lua_RenderTexture2DIndex_mm);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}

#define IMAGE_METATABLE "Image"

static Image *lua_checkImage(lua_State *L, int index) {
    return (Image *)luaL_checkudata(L, index, IMAGE_METATABLE);
}

// Image exposes its dimensions as read-only fields (image.width / .height /
// .mipmaps / .format) via this __index metamethod, since the userdata only
// stores the raw C struct (including the CPU-side pixel data pointer).
static int lua_ImageIndex_mm(lua_State *L) {
    Image      *img = lua_checkImage(L, 1);
    const char *key  = luaL_checkstring(L, 2);

    if (strcmp(key, "width") == 0) {
        lua_pushinteger(L, img->width);
        return 1;
    }
    if (strcmp(key, "height") == 0) {
        lua_pushinteger(L, img->height);
        return 1;
    }
    if (strcmp(key, "mipmaps") == 0) {
        lua_pushinteger(L, img->mipmaps);
        return 1;
    }
    if (strcmp(key, "format") == 0) {
        lua_pushinteger(L, img->format);
        return 1;
    }
    return 0;
}

static void registerImageMetatable(lua_State *L) {
    luaL_newmetatable(L, IMAGE_METATABLE);
    lua_pushcfunction(L, lua_ImageIndex_mm);
    lua_setfield(L, -2, "__index");
    lua_pop(L, 1);
}

// Converts a Lua array table of Vector2 tables ({p1, p2, p3, ...}) into a
// heap-allocated Vector2 array; caller must free() the result. Used by the
// spline drawing functions, which all take a raw Vector2* + count.
static Vector2 *lua_checkVector2Array(lua_State *L, int index, int *count) {
    luaL_checktype(L, index, LUA_TTABLE);
    int      n      = (int)lua_rawlen(L, index);
    Vector2 *points = (Vector2 *)malloc(sizeof(Vector2) * (size_t)n);
    for (int i = 0; i < n; i++) {
        lua_geti(L, index, i + 1);
        points[i] = lua_checkVector2(L, -1);
        lua_pop(L, 1);
    }
    *count = n;
    return points;
}

#define pushColorGlobal(L, color)                                              \
    do {                                                                       \
        pushColor((L), (color));                                               \
        lua_setglobal(L, #color);                                              \
    } while (0)

#define pushIntGlobal(L, value)                                                \
    do {                                                                       \
        lua_pushinteger((L), (value));                                         \
        lua_setglobal((L), #value);                                            \
    } while (0)

int lua_ClearBackground(lua_State *L) {
    Color color = lua_checkColor(L, 1);
    ClearBackground(color);
    return 0;
}

int lua_DrawPixel(lua_State *L) {
    int   x     = (int)luaL_checknumber(L, 1);
    int   y     = (int)luaL_checknumber(L, 2);
    Color color = lua_checkColor(L, 3);
    DrawPixel(x, y, color);
    return 0;
}

int lua_DrawLine(lua_State *L) {
    int   x1    = (int)luaL_checknumber(L, 1);
    int   y1    = (int)luaL_checknumber(L, 2);
    int   x2    = (int)luaL_checknumber(L, 3);
    int   y2    = (int)luaL_checknumber(L, 4);
    Color color = lua_checkColor(L, 5);
    DrawLine(x1, y1, x2, y2, color);
    return 0;
}

int lua_DrawCircle(lua_State *L) {
    int   centerX = (int)luaL_checknumber(L, 1);
    int   centerY = (int)luaL_checknumber(L, 2);
    float radius  = luaL_checknumber(L, 3);
    Color color   = lua_checkColor(L, 4);
    DrawCircle(centerX, centerY, radius, color);
    return 0;
}

int lua_DrawCircleLines(lua_State *L) {
    int   centerX = (int)luaL_checknumber(L, 1);
    int   centerY = (int)luaL_checknumber(L, 2);
    float radius  = luaL_checknumber(L, 3);
    Color color   = lua_checkColor(L, 4);
    DrawCircleLines(centerX, centerY, radius, color);
    return 0;
}

int lua_DrawRectangle(lua_State *L) {
    int   x      = (int)luaL_checknumber(L, 1);
    int   y      = (int)luaL_checknumber(L, 2);
    int   width  = (int)luaL_checknumber(L, 3);
    int   height = (int)luaL_checknumber(L, 4);
    Color color  = lua_checkColor(L, 5);
    DrawRectangle(x, y, width, height, color);
    return 0;
}

int lua_DrawRectangleLines(lua_State *L) {
    int   x      = (int)luaL_checknumber(L, 1);
    int   y      = (int)luaL_checknumber(L, 2);
    int   width  = (int)luaL_checknumber(L, 3);
    int   height = (int)luaL_checknumber(L, 4);
    Color color  = lua_checkColor(L, 5);
    DrawRectangleLines(x, y, width, height, color);
    return 0;
}

int lua_GetScreenWidth(lua_State *L) {
    lua_pushinteger(L, GetScreenWidth());
    return 1;
}

int lua_GetScreenHeight(lua_State *L) {
    lua_pushinteger(L, GetScreenHeight());
    return 1;
}

int lua_GetFrameTime(lua_State *L) {
    lua_pushnumber(L, GetFrameTime());
    return 1;
}

int lua_InitWindow(lua_State *L) {
    int         width  = (int)luaL_checknumber(L, 1);
    int         height = (int)luaL_checknumber(L, 2);
    const char *title  = luaL_checkstring(L, 3);
    InitWindow(width, height, title);
    return 0;
}

int lua_InitAudioDevice(lua_State *L) {
    InitAudioDevice();
    return 0;
}

int lua_SetTargetFPS(lua_State *L) {
    int fps = (int)luaL_checknumber(L, 1);
    SetTargetFPS(fps);
    return 0;
}

int lua_WindowShouldClose(lua_State *L) {
    lua_pushboolean(L, WindowShouldClose() ? true : false);
    return 1;
}

int lua_BeginDrawing(lua_State *L) {
    BeginDrawing();
    return 0;
}

int lua_EndDrawing(lua_State *L) {
    EndDrawing();
    return 0;
}

int lua_CloseWindow(lua_State *L) {
    CloseWindow();
    return 0;
}

int lua_CloseAudioDevice(lua_State *L) {
    CloseAudioDevice();
    return 0;
}

int lua_Color(lua_State *L) {
    int r = (int)luaL_checknumber(L, 1);
    int g = (int)luaL_checknumber(L, 2);
    int b = (int)luaL_checknumber(L, 3);
    int a = (int)luaL_optnumber(L, 4, 255);

    pushColor(L,
              (Color){
                  .r = (unsigned char)r,
                  .g = (unsigned char)g,
                  .b = (unsigned char)b,
                  .a = (unsigned char)a,
              });

    return 1;
}

int lua_Vector2(lua_State *L) {
    float x = luaL_checknumber(L, 1);
    float y = luaL_checknumber(L, 2);

    pushVector2(L, (Vector2){.x = x, .y = y});

    return 1;
}

int lua_Rectangle(lua_State *L) {
    float x      = luaL_checknumber(L, 1);
    float y      = luaL_checknumber(L, 2);
    float width  = luaL_checknumber(L, 3);
    float height = luaL_checknumber(L, 4);

    pushRectangle(
        L, (Rectangle){.x = x, .y = y, .width = width, .height = height});

    return 1;
}

int lua_DrawCircleV(lua_State *L) {
    Vector2 center = lua_checkVector2(L, 1);
    float   radius = (float)luaL_checknumber(L, 2);
    Color   color  = lua_checkColor(L, 3);
    DrawCircleV(center, radius, color);
    return 0;
}

int lua_DrawRectangleRec(lua_State *L) {
    Rectangle rec   = lua_checkRectangle(L, 1);
    Color     color = lua_checkColor(L, 2);
    DrawRectangleRec(rec, color);
    return 0;
}

int lua_DrawRectanglePro(lua_State *L) {
    Rectangle rec      = lua_checkRectangle(L, 1);
    Vector2   origin   = lua_checkVector2(L, 2);
    float     rotation = (float)luaL_checknumber(L, 3);
    Color     color    = lua_checkColor(L, 4);
    DrawRectanglePro(rec, origin, rotation, color);
    return 0;
}

int lua_CheckCollisionRecs(lua_State *L) {
    Rectangle rec1 = lua_checkRectangle(L, 1);
    Rectangle rec2 = lua_checkRectangle(L, 2);
    lua_pushboolean(L, CheckCollisionRecs(rec1, rec2));
    return 1;
}

int lua_CheckCollisionCircles(lua_State *L) {
    Vector2 center1 = lua_checkVector2(L, 1);
    float   radius1 = (float)luaL_checknumber(L, 2);
    Vector2 center2 = lua_checkVector2(L, 3);
    float   radius2 = (float)luaL_checknumber(L, 4);
    lua_pushboolean(L,
                    CheckCollisionCircles(center1, radius1, center2, radius2));
    return 1;
}

int lua_CheckCollisionPointRec(lua_State *L) {
    Vector2   point = lua_checkVector2(L, 1);
    Rectangle rec   = lua_checkRectangle(L, 2);
    lua_pushboolean(L, CheckCollisionPointRec(point, rec));
    return 1;
}

int lua_CheckCollisionCircleRec(lua_State *L) {
    Vector2   center = lua_checkVector2(L, 1);
    float     radius = (float)luaL_checknumber(L, 2);
    Rectangle rec    = lua_checkRectangle(L, 3);
    lua_pushboolean(L, CheckCollisionCircleRec(center, radius, rec));
    return 1;
}

int lua_CheckCollisionPointCircle(lua_State *L) {
    Vector2 point  = lua_checkVector2(L, 1);
    Vector2 center = lua_checkVector2(L, 2);
    float   radius = (float)luaL_checknumber(L, 3);
    lua_pushboolean(L, CheckCollisionPointCircle(point, center, radius));
    return 1;
}

int lua_CheckCollisionPointTriangle(lua_State *L) {
    Vector2 point = lua_checkVector2(L, 1);
    Vector2 p1    = lua_checkVector2(L, 2);
    Vector2 p2    = lua_checkVector2(L, 3);
    Vector2 p3    = lua_checkVector2(L, 4);
    lua_pushboolean(L, CheckCollisionPointTriangle(point, p1, p2, p3));
    return 1;
}

int lua_GetCollisionRec(lua_State *L) {
    Rectangle rec1 = lua_checkRectangle(L, 1);
    Rectangle rec2 = lua_checkRectangle(L, 2);
    pushRectangle(L, GetCollisionRec(rec1, rec2));
    return 1;
}

// ---- Extra shapes ----

int lua_DrawLineEx(lua_State *L) {
    Vector2 startPos = lua_checkVector2(L, 1);
    Vector2 endPos   = lua_checkVector2(L, 2);
    float   thick    = (float)luaL_checknumber(L, 3);
    Color   color    = lua_checkColor(L, 4);
    DrawLineEx(startPos, endPos, thick, color);
    return 0;
}

int lua_DrawLineBezier(lua_State *L) {
    Vector2 startPos = lua_checkVector2(L, 1);
    Vector2 endPos   = lua_checkVector2(L, 2);
    float   thick    = (float)luaL_checknumber(L, 3);
    Color   color    = lua_checkColor(L, 4);
    DrawLineBezier(startPos, endPos, thick, color);
    return 0;
}

int lua_DrawCircleSector(lua_State *L) {
    Vector2 center     = lua_checkVector2(L, 1);
    float   radius     = (float)luaL_checknumber(L, 2);
    float   startAngle = (float)luaL_checknumber(L, 3);
    float   endAngle   = (float)luaL_checknumber(L, 4);
    int     segments   = (int)luaL_checkinteger(L, 5);
    Color   color      = lua_checkColor(L, 6);
    DrawCircleSector(center, radius, startAngle, endAngle, segments, color);
    return 0;
}

int lua_DrawRing(lua_State *L) {
    Vector2 center      = lua_checkVector2(L, 1);
    float   innerRadius = (float)luaL_checknumber(L, 2);
    float   outerRadius = (float)luaL_checknumber(L, 3);
    float   startAngle  = (float)luaL_checknumber(L, 4);
    float   endAngle    = (float)luaL_checknumber(L, 5);
    int     segments    = (int)luaL_checkinteger(L, 6);
    Color   color       = lua_checkColor(L, 7);
    DrawRing(center,
             innerRadius,
             outerRadius,
             startAngle,
             endAngle,
             segments,
             color);
    return 0;
}

int lua_DrawEllipse(lua_State *L) {
    int   centerX = (int)luaL_checknumber(L, 1);
    int   centerY = (int)luaL_checknumber(L, 2);
    float radiusH = (float)luaL_checknumber(L, 3);
    float radiusV = (float)luaL_checknumber(L, 4);
    Color color   = lua_checkColor(L, 5);
    DrawEllipse(centerX, centerY, radiusH, radiusV, color);
    return 0;
}

int lua_DrawEllipseLines(lua_State *L) {
    int   centerX = (int)luaL_checknumber(L, 1);
    int   centerY = (int)luaL_checknumber(L, 2);
    float radiusH = (float)luaL_checknumber(L, 3);
    float radiusV = (float)luaL_checknumber(L, 4);
    Color color   = lua_checkColor(L, 5);
    DrawEllipseLines(centerX, centerY, radiusH, radiusV, color);
    return 0;
}

int lua_DrawRectangleRounded(lua_State *L) {
    Rectangle rec       = lua_checkRectangle(L, 1);
    float     roundness = (float)luaL_checknumber(L, 2);
    int       segments  = (int)luaL_checkinteger(L, 3);
    Color     color     = lua_checkColor(L, 4);
    DrawRectangleRounded(rec, roundness, segments, color);
    return 0;
}

int lua_DrawRectangleRoundedLines(lua_State *L) {
    Rectangle rec       = lua_checkRectangle(L, 1);
    float     roundness = (float)luaL_checknumber(L, 2);
    int       segments  = (int)luaL_checkinteger(L, 3);
    Color     color     = lua_checkColor(L, 4);
    DrawRectangleRoundedLines(rec, roundness, segments, color);
    return 0;
}

int lua_DrawRectangleGradientV(lua_State *L) {
    int   posX   = (int)luaL_checknumber(L, 1);
    int   posY   = (int)luaL_checknumber(L, 2);
    int   width  = (int)luaL_checknumber(L, 3);
    int   height = (int)luaL_checknumber(L, 4);
    Color top    = lua_checkColor(L, 5);
    Color bottom = lua_checkColor(L, 6);
    DrawRectangleGradientV(posX, posY, width, height, top, bottom);
    return 0;
}

int lua_DrawRectangleGradientH(lua_State *L) {
    int   posX   = (int)luaL_checknumber(L, 1);
    int   posY   = (int)luaL_checknumber(L, 2);
    int   width  = (int)luaL_checknumber(L, 3);
    int   height = (int)luaL_checknumber(L, 4);
    Color left   = lua_checkColor(L, 5);
    Color right  = lua_checkColor(L, 6);
    DrawRectangleGradientH(posX, posY, width, height, left, right);
    return 0;
}

int lua_DrawTriangle(lua_State *L) {
    Vector2 v1    = lua_checkVector2(L, 1);
    Vector2 v2    = lua_checkVector2(L, 2);
    Vector2 v3    = lua_checkVector2(L, 3);
    Color   color = lua_checkColor(L, 4);
    DrawTriangle(v1, v2, v3, color);
    return 0;
}

int lua_DrawTriangleLines(lua_State *L) {
    Vector2 v1    = lua_checkVector2(L, 1);
    Vector2 v2    = lua_checkVector2(L, 2);
    Vector2 v3    = lua_checkVector2(L, 3);
    Color   color = lua_checkColor(L, 4);
    DrawTriangleLines(v1, v2, v3, color);
    return 0;
}

int lua_DrawPoly(lua_State *L) {
    Vector2 center   = lua_checkVector2(L, 1);
    int     sides    = (int)luaL_checkinteger(L, 2);
    float   radius   = (float)luaL_checknumber(L, 3);
    float   rotation = (float)luaL_checknumber(L, 4);
    Color   color    = lua_checkColor(L, 5);
    DrawPoly(center, sides, radius, rotation, color);
    return 0;
}

int lua_DrawPolyLines(lua_State *L) {
    Vector2 center   = lua_checkVector2(L, 1);
    int     sides    = (int)luaL_checkinteger(L, 2);
    float   radius   = (float)luaL_checknumber(L, 3);
    float   rotation = (float)luaL_checknumber(L, 4);
    Color   color    = lua_checkColor(L, 5);
    DrawPolyLines(center, sides, radius, rotation, color);
    return 0;
}

// ---- Splines (each takes a Lua array table of Vector2 points) ----

int lua_DrawSplineLinear(lua_State *L) {
    int      count;
    Vector2 *points = lua_checkVector2Array(L, 1, &count);
    float    thick  = (float)luaL_checknumber(L, 2);
    Color    color  = lua_checkColor(L, 3);
    DrawSplineLinear(points, count, thick, color);
    free(points);
    return 0;
}

int lua_DrawSplineBasis(lua_State *L) {
    int      count;
    Vector2 *points = lua_checkVector2Array(L, 1, &count);
    float    thick  = (float)luaL_checknumber(L, 2);
    Color    color  = lua_checkColor(L, 3);
    DrawSplineBasis(points, count, thick, color);
    free(points);
    return 0;
}

int lua_DrawSplineCatmullRom(lua_State *L) {
    int      count;
    Vector2 *points = lua_checkVector2Array(L, 1, &count);
    float    thick  = (float)luaL_checknumber(L, 2);
    Color    color  = lua_checkColor(L, 3);
    DrawSplineCatmullRom(points, count, thick, color);
    free(points);
    return 0;
}

int lua_DrawSplineBezierQuadratic(lua_State *L) {
    int      count;
    Vector2 *points = lua_checkVector2Array(L, 1, &count);
    float    thick  = (float)luaL_checknumber(L, 2);
    Color    color  = lua_checkColor(L, 3);
    DrawSplineBezierQuadratic(points, count, thick, color);
    free(points);
    return 0;
}

int lua_DrawSplineBezierCubic(lua_State *L) {
    int      count;
    Vector2 *points = lua_checkVector2Array(L, 1, &count);
    float    thick  = (float)luaL_checknumber(L, 2);
    Color    color  = lua_checkColor(L, 3);
    DrawSplineBezierCubic(points, count, thick, color);
    free(points);
    return 0;
}

int lua_IsKeyDown(lua_State *L) {
    int key = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyDown(key));
    return 1;
}

int lua_IsKeyPressed(lua_State *L) {
    int key = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}

int lua_IsKeyReleased(lua_State *L) {
    int key = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyReleased(key));
    return 1;
}

int lua_IsKeyUp(lua_State *L) {
    int key = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyUp(key));
    return 1;
}

int lua_IsMouseButtonDown(lua_State *L) {
    int button = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsMouseButtonDown(button));
    return 1;
}

int lua_IsMouseButtonPressed(lua_State *L) {
    int button = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsMouseButtonPressed(button));
    return 1;
}

int lua_IsMouseButtonReleased(lua_State *L) {
    int button = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsMouseButtonReleased(button));
    return 1;
}

int lua_GetMousePosition(lua_State *L) {
    pushVector2(L, GetMousePosition());
    return 1;
}

int lua_GetMouseWheelMove(lua_State *L) {
    lua_pushnumber(L, GetMouseWheelMove());
    return 1;
}

// ---- Cursor & mouse control ----

int lua_ShowCursor(lua_State *L) {
    ShowCursor();
    return 0;
}

int lua_HideCursor(lua_State *L) {
    HideCursor();
    return 0;
}

int lua_IsCursorHidden(lua_State *L) {
    lua_pushboolean(L, IsCursorHidden());
    return 1;
}

int lua_EnableCursor(lua_State *L) {
    EnableCursor();
    return 0;
}

int lua_DisableCursor(lua_State *L) {
    DisableCursor();
    return 0;
}

int lua_IsCursorOnScreen(lua_State *L) {
    lua_pushboolean(L, IsCursorOnScreen());
    return 1;
}

int lua_GetMouseDelta(lua_State *L) {
    pushVector2(L, GetMouseDelta());
    return 1;
}

int lua_SetMousePosition(lua_State *L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);
    SetMousePosition(x, y);
    return 0;
}

int lua_SetMouseOffset(lua_State *L) {
    int offsetX = (int)luaL_checkinteger(L, 1);
    int offsetY = (int)luaL_checkinteger(L, 2);
    SetMouseOffset(offsetX, offsetY);
    return 0;
}

int lua_SetMouseScale(lua_State *L) {
    float scaleX = (float)luaL_checknumber(L, 1);
    float scaleY = (float)luaL_checknumber(L, 2);
    SetMouseScale(scaleX, scaleY);
    return 0;
}

int lua_SetMouseCursor(lua_State *L) {
    int cursor = (int)luaL_checkinteger(L, 1);
    SetMouseCursor(cursor);
    return 0;
}

// ---- Keyboard extras & clipboard ----

int lua_GetKeyPressed(lua_State *L) {
    lua_pushinteger(L, GetKeyPressed());
    return 1;
}

int lua_GetCharPressed(lua_State *L) {
    lua_pushinteger(L, GetCharPressed());
    return 1;
}

int lua_SetExitKey(lua_State *L) {
    int key = (int)luaL_checkinteger(L, 1);
    SetExitKey(key);
    return 0;
}

int lua_GetClipboardText(lua_State *L) {
    const char *text = GetClipboardText();
    if (text == NULL) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, text);
    }
    return 1;
}

int lua_SetClipboardText(lua_State *L) {
    const char *text = luaL_checkstring(L, 1);
    SetClipboardText(text);
    return 0;
}

// ---- Gamepad input ----

int lua_IsGamepadAvailable(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsGamepadAvailable(gamepad));
    return 1;
}

int lua_GetGamepadName(lua_State *L) {
    int         gamepad = (int)luaL_checkinteger(L, 1);
    const char *name    = GetGamepadName(gamepad);
    if (name == NULL) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, name);
    }
    return 1;
}

int lua_IsGamepadButtonDown(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    int button  = (int)luaL_checkinteger(L, 2);
    lua_pushboolean(L, IsGamepadButtonDown(gamepad, button));
    return 1;
}

int lua_IsGamepadButtonPressed(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    int button  = (int)luaL_checkinteger(L, 2);
    lua_pushboolean(L, IsGamepadButtonPressed(gamepad, button));
    return 1;
}

int lua_IsGamepadButtonReleased(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    int button  = (int)luaL_checkinteger(L, 2);
    lua_pushboolean(L, IsGamepadButtonReleased(gamepad, button));
    return 1;
}

int lua_IsGamepadButtonUp(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    int button  = (int)luaL_checkinteger(L, 2);
    lua_pushboolean(L, IsGamepadButtonUp(gamepad, button));
    return 1;
}

int lua_GetGamepadAxisCount(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, GetGamepadAxisCount(gamepad));
    return 1;
}

int lua_GetGamepadAxisMovement(lua_State *L) {
    int gamepad = (int)luaL_checkinteger(L, 1);
    int axis    = (int)luaL_checkinteger(L, 2);
    lua_pushnumber(L, GetGamepadAxisMovement(gamepad, axis));
    return 1;
}

int lua_DrawText(lua_State *L) {
    const char *text     = luaL_checkstring(L, 1);
    int         posX     = (int)luaL_checknumber(L, 2);
    int         posY     = (int)luaL_checknumber(L, 3);
    int         fontSize = (int)luaL_checknumber(L, 4);
    Color       color    = lua_checkColor(L, 5);
    DrawText(text, posX, posY, fontSize, color);
    return 0;
}

int lua_MeasureText(lua_State *L) {
    const char *text     = luaL_checkstring(L, 1);
    int         fontSize = (int)luaL_checknumber(L, 2);
    lua_pushinteger(L, MeasureText(text, fontSize));
    return 1;
}

int lua_DrawFPS(lua_State *L) {
    int posX = (int)luaL_checknumber(L, 1);
    int posY = (int)luaL_checknumber(L, 2);
    DrawFPS(posX, posY);
    return 0;
}

int lua_LoadSound(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    Sound       sound    = LoadSound(fileName);

    Sound *ud = (Sound *)lua_newuserdata(L, sizeof(Sound));
    *ud       = sound;
    luaL_getmetatable(L, SOUND_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_UnloadSound(lua_State *L) {
    Sound *sound = lua_checkSound(L, 1);
    UnloadSound(*sound);
    return 0;
}

int lua_PlaySound(lua_State *L) {
    Sound *sound = lua_checkSound(L, 1);
    PlaySound(*sound);
    return 0;
}

int lua_StopSound(lua_State *L) {
    Sound *sound = lua_checkSound(L, 1);
    StopSound(*sound);
    return 0;
}

int lua_IsSoundPlaying(lua_State *L) {
    Sound *sound = lua_checkSound(L, 1);
    lua_pushboolean(L, IsSoundPlaying(*sound));
    return 1;
}

int lua_SetSoundVolume(lua_State *L) {
    Sound *sound  = lua_checkSound(L, 1);
    float  volume = (float)luaL_checknumber(L, 2);
    SetSoundVolume(*sound, volume);
    return 0;
}

int lua_SetSoundPitch(lua_State *L) {
    Sound *sound = lua_checkSound(L, 1);
    float  pitch = (float)luaL_checknumber(L, 2);
    SetSoundPitch(*sound, pitch);
    return 0;
}

int lua_SetSoundPan(lua_State *L) {
    Sound *sound = lua_checkSound(L, 1);
    float  pan   = (float)luaL_checknumber(L, 2);
    SetSoundPan(*sound, pan);
    return 0;
}

int lua_LoadMusicStream(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    Music       music    = LoadMusicStream(fileName);

    Music *ud = (Music *)lua_newuserdata(L, sizeof(Music));
    *ud       = music;
    luaL_getmetatable(L, MUSIC_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_UnloadMusicStream(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    UnloadMusicStream(*music);
    return 0;
}

int lua_PlayMusicStream(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    PlayMusicStream(*music);
    return 0;
}

int lua_UpdateMusicStream(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    UpdateMusicStream(*music);
    return 0;
}

int lua_StopMusicStream(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    StopMusicStream(*music);
    return 0;
}

int lua_PauseMusicStream(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    PauseMusicStream(*music);
    return 0;
}

int lua_ResumeMusicStream(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    ResumeMusicStream(*music);
    return 0;
}

int lua_IsMusicStreamPlaying(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    lua_pushboolean(L, IsMusicStreamPlaying(*music));
    return 1;
}

int lua_SetMusicVolume(lua_State *L) {
    Music *music  = lua_checkMusic(L, 1);
    float  volume = (float)luaL_checknumber(L, 2);
    SetMusicVolume(*music, volume);
    return 0;
}

int lua_SetMusicPitch(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    float  pitch = (float)luaL_checknumber(L, 2);
    SetMusicPitch(*music, pitch);
    return 0;
}

int lua_SetMusicPan(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    float  pan   = (float)luaL_checknumber(L, 2);
    SetMusicPan(*music, pan);
    return 0;
}

int lua_GetMusicTimeLength(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    lua_pushnumber(L, GetMusicTimeLength(*music));
    return 1;
}

int lua_GetMusicTimePlayed(lua_State *L) {
    Music *music = lua_checkMusic(L, 1);
    lua_pushnumber(L, GetMusicTimePlayed(*music));
    return 1;
}

int lua_SeekMusicStream(lua_State *L) {
    Music *music    = lua_checkMusic(L, 1);
    float  position = (float)luaL_checknumber(L, 2);
    SeekMusicStream(*music, position);
    return 0;
}

// ---- Fonts & text ----

int lua_LoadFont(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    Font        font     = LoadFont(fileName);

    Font *ud = (Font *)lua_newuserdata(L, sizeof(Font));
    *ud      = font;
    luaL_getmetatable(L, FONT_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_LoadFontEx(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    int         fontSize = (int)luaL_checkinteger(L, 2);
    Font        font     = LoadFontEx(fileName, fontSize, NULL, 0);

    Font *ud = (Font *)lua_newuserdata(L, sizeof(Font));
    *ud      = font;
    luaL_getmetatable(L, FONT_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_UnloadFont(lua_State *L) {
    Font *font = lua_checkFont(L, 1);
    UnloadFont(*font);
    return 0;
}

int lua_GetFontDefault(lua_State *L) {
    Font font = GetFontDefault();

    Font *ud = (Font *)lua_newuserdata(L, sizeof(Font));
    *ud      = font;
    luaL_getmetatable(L, FONT_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_DrawTextEx(lua_State *L) {
    Font       *font     = lua_checkFont(L, 1);
    const char *text     = luaL_checkstring(L, 2);
    Vector2     position = lua_checkVector2(L, 3);
    float       fontSize = (float)luaL_checknumber(L, 4);
    float       spacing  = (float)luaL_checknumber(L, 5);
    Color       tint     = lua_checkColor(L, 6);
    DrawTextEx(*font, text, position, fontSize, spacing, tint);
    return 0;
}

int lua_DrawTextPro(lua_State *L) {
    Font       *font     = lua_checkFont(L, 1);
    const char *text     = luaL_checkstring(L, 2);
    Vector2     position = lua_checkVector2(L, 3);
    Vector2     origin   = lua_checkVector2(L, 4);
    float       rotation = (float)luaL_checknumber(L, 5);
    float       fontSize = (float)luaL_checknumber(L, 6);
    float       spacing  = (float)luaL_checknumber(L, 7);
    Color       tint     = lua_checkColor(L, 8);
    DrawTextPro(
        *font, text, position, origin, rotation, fontSize, spacing, tint);
    return 0;
}

int lua_MeasureTextEx(lua_State *L) {
    Font       *font     = lua_checkFont(L, 1);
    const char *text     = luaL_checkstring(L, 2);
    float       fontSize = (float)luaL_checknumber(L, 3);
    float       spacing  = (float)luaL_checknumber(L, 4);
    pushVector2(L, MeasureTextEx(*font, text, fontSize, spacing));
    return 1;
}

// ---- Vector2 operator metamethods ----

int lua_Vector2Add_mm(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    pushVector2(L, Vector2Add(a, b));
    return 1;
}

int lua_Vector2Sub_mm(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    pushVector2(L, Vector2Subtract(a, b));
    return 1;
}

int lua_Vector2Mul_mm(lua_State *L) {
    if (lua_type(L, 1) == LUA_TNUMBER) {
        float   scalar = (float)lua_tonumber(L, 1);
        Vector2 v      = lua_checkVector2(L, 2);
        pushVector2(L, Vector2Scale(v, scalar));
    } else if (lua_type(L, 2) == LUA_TNUMBER) {
        Vector2 v      = lua_checkVector2(L, 1);
        float   scalar = (float)lua_tonumber(L, 2);
        pushVector2(L, Vector2Scale(v, scalar));
    } else {
        Vector2 a = lua_checkVector2(L, 1);
        Vector2 b = lua_checkVector2(L, 2);
        pushVector2(L, Vector2Multiply(a, b));
    }
    return 1;
}

int lua_Vector2Unm_mm(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    pushVector2(L, Vector2Negate(v));
    return 1;
}

int lua_Vector2Eq_mm(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    lua_pushboolean(L, Vector2Equals(a, b));
    return 1;
}

int lua_Vector2ToString_mm(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    lua_pushfstring(L, "Vector2(%f, %f)", v.x, v.y);
    return 1;
}

// ---- Vector2 methods (called as v:method(...)) ----

int lua_Vector2Length_m(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    lua_pushnumber(L, Vector2Length(v));
    return 1;
}

int lua_Vector2Normalize_m(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    pushVector2(L, Vector2Normalize(v));
    return 1;
}

int lua_Vector2Dot_m(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    lua_pushnumber(L, Vector2DotProduct(a, b));
    return 1;
}

int lua_Vector2Distance_m(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    lua_pushnumber(L, Vector2Distance(a, b));
    return 1;
}

int lua_Vector2Lerp_m(lua_State *L) {
    Vector2 a      = lua_checkVector2(L, 1);
    Vector2 b      = lua_checkVector2(L, 2);
    float   amount = (float)luaL_checknumber(L, 3);
    pushVector2(L, Vector2Lerp(a, b, amount));
    return 1;
}

int lua_Vector2Rotate_m(lua_State *L) {
    Vector2 v     = lua_checkVector2(L, 1);
    float   angle = (float)luaL_checknumber(L, 2);
    pushVector2(L, Vector2Rotate(v, angle));
    return 1;
}

int lua_Vector2Angle_m(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    lua_pushnumber(L, Vector2Angle(a, b));
    return 1;
}

int lua_Vector2Reflect_m(lua_State *L) {
    Vector2 v      = lua_checkVector2(L, 1);
    Vector2 normal = lua_checkVector2(L, 2);
    pushVector2(L, Vector2Reflect(v, normal));
    return 1;
}

int lua_Vector2MoveTowards_m(lua_State *L) {
    Vector2 v           = lua_checkVector2(L, 1);
    Vector2 target      = lua_checkVector2(L, 2);
    float   maxDistance = (float)luaL_checknumber(L, 3);
    pushVector2(L, Vector2MoveTowards(v, target, maxDistance));
    return 1;
}

int lua_Vector2Clamp_m(lua_State *L) {
    Vector2 v   = lua_checkVector2(L, 1);
    Vector2 min = lua_checkVector2(L, 2);
    Vector2 max = lua_checkVector2(L, 3);
    pushVector2(L, Vector2Clamp(v, min, max));
    return 1;
}

int lua_Vector2Invert_m(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    pushVector2(L, Vector2Invert(v));
    return 1;
}

int lua_Vector2LengthSqr_m(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    lua_pushnumber(L, Vector2LengthSqr(v));
    return 1;
}

int lua_Vector2DistanceSqr_m(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    lua_pushnumber(L, Vector2DistanceSqr(a, b));
    return 1;
}

int lua_Vector2LineAngle_m(lua_State *L) {
    Vector2 start = lua_checkVector2(L, 1);
    Vector2 end   = lua_checkVector2(L, 2);
    lua_pushnumber(L, Vector2LineAngle(start, end));
    return 1;
}

int lua_Vector2CrossProduct_m(lua_State *L) {
    Vector2 a = lua_checkVector2(L, 1);
    Vector2 b = lua_checkVector2(L, 2);
    lua_pushnumber(L, Vector2CrossProduct(a, b));
    return 1;
}

int lua_Vector2Refract_m(lua_State *L) {
    Vector2 v = lua_checkVector2(L, 1);
    Vector2 n = lua_checkVector2(L, 2);
    float   r = (float)luaL_checknumber(L, 3);
    pushVector2(L, Vector2Refract(v, n, r));
    return 1;
}

static void registerVector2Metatable(lua_State *L) {
    luaL_newmetatable(L, VECTOR2_METATABLE);

    lua_pushcfunction(L, lua_Vector2Add_mm);
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, lua_Vector2Sub_mm);
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, lua_Vector2Mul_mm);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, lua_Vector2Unm_mm);
    lua_setfield(L, -2, "__unm");

    lua_pushcfunction(L, lua_Vector2Eq_mm);
    lua_setfield(L, -2, "__eq");

    lua_pushcfunction(L, lua_Vector2ToString_mm);
    lua_setfield(L, -2, "__tostring");

    lua_newtable(L); // methods table, becomes __index
    lua_pushcfunction(L, lua_Vector2Length_m);
    lua_setfield(L, -2, "length");
    lua_pushcfunction(L, lua_Vector2Normalize_m);
    lua_setfield(L, -2, "normalize");
    lua_pushcfunction(L, lua_Vector2Dot_m);
    lua_setfield(L, -2, "dot");
    lua_pushcfunction(L, lua_Vector2Distance_m);
    lua_setfield(L, -2, "distance");
    lua_pushcfunction(L, lua_Vector2Lerp_m);
    lua_setfield(L, -2, "lerp");
    lua_pushcfunction(L, lua_Vector2Rotate_m);
    lua_setfield(L, -2, "rotate");
    lua_pushcfunction(L, lua_Vector2Angle_m);
    lua_setfield(L, -2, "angle");
    lua_pushcfunction(L, lua_Vector2Reflect_m);
    lua_setfield(L, -2, "reflect");
    lua_pushcfunction(L, lua_Vector2MoveTowards_m);
    lua_setfield(L, -2, "moveTowards");
    lua_pushcfunction(L, lua_Vector2Clamp_m);
    lua_setfield(L, -2, "clamp");
    lua_pushcfunction(L, lua_Vector2Invert_m);
    lua_setfield(L, -2, "invert");
    lua_pushcfunction(L, lua_Vector2LengthSqr_m);
    lua_setfield(L, -2, "lengthSqr");
    lua_pushcfunction(L, lua_Vector2DistanceSqr_m);
    lua_setfield(L, -2, "distanceSqr");
    lua_pushcfunction(L, lua_Vector2LineAngle_m);
    lua_setfield(L, -2, "lineAngle");
    lua_pushcfunction(L, lua_Vector2CrossProduct_m);
    lua_setfield(L, -2, "cross");
    lua_pushcfunction(L, lua_Vector2Refract_m);
    lua_setfield(L, -2, "refract");
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1); // pop the metatable; it stays registered in the Lua registry
}

// ---- Vector3 constructor ----

int lua_Vector3(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    pushVector3(L, (Vector3){.x = x, .y = y, .z = z});
    return 1;
}

// ---- Vector3 operator metamethods ----

int lua_Vector3Add_mm(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    pushVector3(L, Vector3Add(a, b));
    return 1;
}

int lua_Vector3Sub_mm(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    pushVector3(L, Vector3Subtract(a, b));
    return 1;
}

int lua_Vector3Mul_mm(lua_State *L) {
    if (lua_type(L, 1) == LUA_TNUMBER) {
        float   scalar = (float)lua_tonumber(L, 1);
        Vector3 v      = lua_checkVector3(L, 2);
        pushVector3(L, Vector3Scale(v, scalar));
    } else if (lua_type(L, 2) == LUA_TNUMBER) {
        Vector3 v      = lua_checkVector3(L, 1);
        float   scalar = (float)lua_tonumber(L, 2);
        pushVector3(L, Vector3Scale(v, scalar));
    } else {
        Vector3 a = lua_checkVector3(L, 1);
        Vector3 b = lua_checkVector3(L, 2);
        pushVector3(L, Vector3Multiply(a, b));
    }
    return 1;
}

int lua_Vector3Unm_mm(lua_State *L) {
    Vector3 v = lua_checkVector3(L, 1);
    pushVector3(L, Vector3Negate(v));
    return 1;
}

int lua_Vector3Eq_mm(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    lua_pushboolean(L, Vector3Equals(a, b));
    return 1;
}

int lua_Vector3ToString_mm(lua_State *L) {
    Vector3 v = lua_checkVector3(L, 1);
    lua_pushfstring(L, "Vector3(%f, %f, %f)", v.x, v.y, v.z);
    return 1;
}

// ---- Vector3 methods (called as v:method(...)) ----

int lua_Vector3Length_m(lua_State *L) {
    Vector3 v = lua_checkVector3(L, 1);
    lua_pushnumber(L, Vector3Length(v));
    return 1;
}

int lua_Vector3LengthSqr_m(lua_State *L) {
    Vector3 v = lua_checkVector3(L, 1);
    lua_pushnumber(L, Vector3LengthSqr(v));
    return 1;
}

int lua_Vector3Normalize_m(lua_State *L) {
    Vector3 v = lua_checkVector3(L, 1);
    pushVector3(L, Vector3Normalize(v));
    return 1;
}

int lua_Vector3Dot_m(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    lua_pushnumber(L, Vector3DotProduct(a, b));
    return 1;
}

int lua_Vector3Cross_m(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    pushVector3(L, Vector3CrossProduct(a, b));
    return 1;
}

int lua_Vector3Distance_m(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    lua_pushnumber(L, Vector3Distance(a, b));
    return 1;
}

int lua_Vector3DistanceSqr_m(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    lua_pushnumber(L, Vector3DistanceSqr(a, b));
    return 1;
}

int lua_Vector3Lerp_m(lua_State *L) {
    Vector3 a      = lua_checkVector3(L, 1);
    Vector3 b      = lua_checkVector3(L, 2);
    float   amount = (float)luaL_checknumber(L, 3);
    pushVector3(L, Vector3Lerp(a, b, amount));
    return 1;
}

int lua_Vector3Angle_m(lua_State *L) {
    Vector3 a = lua_checkVector3(L, 1);
    Vector3 b = lua_checkVector3(L, 2);
    lua_pushnumber(L, Vector3Angle(a, b));
    return 1;
}

int lua_Vector3Reflect_m(lua_State *L) {
    Vector3 v      = lua_checkVector3(L, 1);
    Vector3 normal = lua_checkVector3(L, 2);
    pushVector3(L, Vector3Reflect(v, normal));
    return 1;
}

int lua_Vector3MoveTowards_m(lua_State *L) {
    Vector3 v           = lua_checkVector3(L, 1);
    Vector3 target      = lua_checkVector3(L, 2);
    float   maxDistance = (float)luaL_checknumber(L, 3);
    pushVector3(L, Vector3MoveTowards(v, target, maxDistance));
    return 1;
}

int lua_Vector3Clamp_m(lua_State *L) {
    Vector3 v   = lua_checkVector3(L, 1);
    Vector3 min = lua_checkVector3(L, 2);
    Vector3 max = lua_checkVector3(L, 3);
    pushVector3(L, Vector3Clamp(v, min, max));
    return 1;
}

int lua_Vector3Invert_m(lua_State *L) {
    Vector3 v = lua_checkVector3(L, 1);
    pushVector3(L, Vector3Invert(v));
    return 1;
}

int lua_Vector3RotateByAxisAngle_m(lua_State *L) {
    Vector3 v     = lua_checkVector3(L, 1);
    Vector3 axis  = lua_checkVector3(L, 2);
    float   angle = (float)luaL_checknumber(L, 3);
    pushVector3(L, Vector3RotateByAxisAngle(v, axis, angle));
    return 1;
}

int lua_Vector3Transform_m(lua_State *L) {
    Vector3 v   = lua_checkVector3(L, 1);
    Matrix  mat = lua_checkMatrix(L, 2);
    pushVector3(L, Vector3Transform(v, mat));
    return 1;
}

static void registerVector3Metatable(lua_State *L) {
    luaL_newmetatable(L, VECTOR3_METATABLE);

    lua_pushcfunction(L, lua_Vector3Add_mm);
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, lua_Vector3Sub_mm);
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, lua_Vector3Mul_mm);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, lua_Vector3Unm_mm);
    lua_setfield(L, -2, "__unm");

    lua_pushcfunction(L, lua_Vector3Eq_mm);
    lua_setfield(L, -2, "__eq");

    lua_pushcfunction(L, lua_Vector3ToString_mm);
    lua_setfield(L, -2, "__tostring");

    lua_newtable(L); // methods table, becomes __index
    lua_pushcfunction(L, lua_Vector3Length_m);
    lua_setfield(L, -2, "length");
    lua_pushcfunction(L, lua_Vector3LengthSqr_m);
    lua_setfield(L, -2, "lengthSqr");
    lua_pushcfunction(L, lua_Vector3Normalize_m);
    lua_setfield(L, -2, "normalize");
    lua_pushcfunction(L, lua_Vector3Dot_m);
    lua_setfield(L, -2, "dot");
    lua_pushcfunction(L, lua_Vector3Cross_m);
    lua_setfield(L, -2, "cross");
    lua_pushcfunction(L, lua_Vector3Distance_m);
    lua_setfield(L, -2, "distance");
    lua_pushcfunction(L, lua_Vector3DistanceSqr_m);
    lua_setfield(L, -2, "distanceSqr");
    lua_pushcfunction(L, lua_Vector3Lerp_m);
    lua_setfield(L, -2, "lerp");
    lua_pushcfunction(L, lua_Vector3Angle_m);
    lua_setfield(L, -2, "angle");
    lua_pushcfunction(L, lua_Vector3Reflect_m);
    lua_setfield(L, -2, "reflect");
    lua_pushcfunction(L, lua_Vector3MoveTowards_m);
    lua_setfield(L, -2, "moveTowards");
    lua_pushcfunction(L, lua_Vector3Clamp_m);
    lua_setfield(L, -2, "clamp");
    lua_pushcfunction(L, lua_Vector3Invert_m);
    lua_setfield(L, -2, "invert");
    lua_pushcfunction(L, lua_Vector3RotateByAxisAngle_m);
    lua_setfield(L, -2, "rotateByAxisAngle");
    lua_pushcfunction(L, lua_Vector3Transform_m);
    lua_setfield(L, -2, "transform");
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1); // pop the metatable; it stays registered in the Lua registry
}

// ---- Vector4 constructor ----

int lua_Vector4(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    float w = (float)luaL_checknumber(L, 4);
    pushVector4(L, (Vector4){.x = x, .y = y, .z = z, .w = w});
    return 1;
}

// ---- Vector4 operator metamethods ----

int lua_Vector4Add_mm(lua_State *L) {
    Vector4 a = lua_checkVector4(L, 1);
    Vector4 b = lua_checkVector4(L, 2);
    pushVector4(L, Vector4Add(a, b));
    return 1;
}

int lua_Vector4Sub_mm(lua_State *L) {
    Vector4 a = lua_checkVector4(L, 1);
    Vector4 b = lua_checkVector4(L, 2);
    pushVector4(L, Vector4Subtract(a, b));
    return 1;
}

int lua_Vector4Mul_mm(lua_State *L) {
    if (lua_type(L, 1) == LUA_TNUMBER) {
        float   scalar = (float)lua_tonumber(L, 1);
        Vector4 v      = lua_checkVector4(L, 2);
        pushVector4(L, Vector4Scale(v, scalar));
    } else if (lua_type(L, 2) == LUA_TNUMBER) {
        Vector4 v      = lua_checkVector4(L, 1);
        float   scalar = (float)lua_tonumber(L, 2);
        pushVector4(L, Vector4Scale(v, scalar));
    } else {
        Vector4 a = lua_checkVector4(L, 1);
        Vector4 b = lua_checkVector4(L, 2);
        pushVector4(L, Vector4Multiply(a, b));
    }
    return 1;
}

int lua_Vector4Unm_mm(lua_State *L) {
    Vector4 v = lua_checkVector4(L, 1);
    pushVector4(L, Vector4Negate(v));
    return 1;
}

int lua_Vector4Eq_mm(lua_State *L) {
    Vector4 a = lua_checkVector4(L, 1);
    Vector4 b = lua_checkVector4(L, 2);
    lua_pushboolean(L, Vector4Equals(a, b));
    return 1;
}

int lua_Vector4ToString_mm(lua_State *L) {
    Vector4 v = lua_checkVector4(L, 1);
    lua_pushfstring(L, "Vector4(%f, %f, %f, %f)", v.x, v.y, v.z, v.w);
    return 1;
}

// ---- Vector4 methods (called as v:method(...)) ----

int lua_Vector4Length_m(lua_State *L) {
    Vector4 v = lua_checkVector4(L, 1);
    lua_pushnumber(L, Vector4Length(v));
    return 1;
}

int lua_Vector4LengthSqr_m(lua_State *L) {
    Vector4 v = lua_checkVector4(L, 1);
    lua_pushnumber(L, Vector4LengthSqr(v));
    return 1;
}

int lua_Vector4Normalize_m(lua_State *L) {
    Vector4 v = lua_checkVector4(L, 1);
    pushVector4(L, Vector4Normalize(v));
    return 1;
}

int lua_Vector4Dot_m(lua_State *L) {
    Vector4 a = lua_checkVector4(L, 1);
    Vector4 b = lua_checkVector4(L, 2);
    lua_pushnumber(L, Vector4DotProduct(a, b));
    return 1;
}

int lua_Vector4Distance_m(lua_State *L) {
    Vector4 a = lua_checkVector4(L, 1);
    Vector4 b = lua_checkVector4(L, 2);
    lua_pushnumber(L, Vector4Distance(a, b));
    return 1;
}

int lua_Vector4DistanceSqr_m(lua_State *L) {
    Vector4 a = lua_checkVector4(L, 1);
    Vector4 b = lua_checkVector4(L, 2);
    lua_pushnumber(L, Vector4DistanceSqr(a, b));
    return 1;
}

int lua_Vector4Lerp_m(lua_State *L) {
    Vector4 a      = lua_checkVector4(L, 1);
    Vector4 b      = lua_checkVector4(L, 2);
    float   amount = (float)luaL_checknumber(L, 3);
    pushVector4(L, Vector4Lerp(a, b, amount));
    return 1;
}

int lua_Vector4MoveTowards_m(lua_State *L) {
    Vector4 v           = lua_checkVector4(L, 1);
    Vector4 target      = lua_checkVector4(L, 2);
    float   maxDistance = (float)luaL_checknumber(L, 3);
    pushVector4(L, Vector4MoveTowards(v, target, maxDistance));
    return 1;
}

int lua_Vector4Invert_m(lua_State *L) {
    Vector4 v = lua_checkVector4(L, 1);
    pushVector4(L, Vector4Invert(v));
    return 1;
}

static void registerVector4Metatable(lua_State *L) {
    luaL_newmetatable(L, VECTOR4_METATABLE);

    lua_pushcfunction(L, lua_Vector4Add_mm);
    lua_setfield(L, -2, "__add");

    lua_pushcfunction(L, lua_Vector4Sub_mm);
    lua_setfield(L, -2, "__sub");

    lua_pushcfunction(L, lua_Vector4Mul_mm);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, lua_Vector4Unm_mm);
    lua_setfield(L, -2, "__unm");

    lua_pushcfunction(L, lua_Vector4Eq_mm);
    lua_setfield(L, -2, "__eq");

    lua_pushcfunction(L, lua_Vector4ToString_mm);
    lua_setfield(L, -2, "__tostring");

    lua_newtable(L); // methods table, becomes __index
    lua_pushcfunction(L, lua_Vector4Length_m);
    lua_setfield(L, -2, "length");
    lua_pushcfunction(L, lua_Vector4LengthSqr_m);
    lua_setfield(L, -2, "lengthSqr");
    lua_pushcfunction(L, lua_Vector4Normalize_m);
    lua_setfield(L, -2, "normalize");
    lua_pushcfunction(L, lua_Vector4Dot_m);
    lua_setfield(L, -2, "dot");
    lua_pushcfunction(L, lua_Vector4Distance_m);
    lua_setfield(L, -2, "distance");
    lua_pushcfunction(L, lua_Vector4DistanceSqr_m);
    lua_setfield(L, -2, "distanceSqr");
    lua_pushcfunction(L, lua_Vector4Lerp_m);
    lua_setfield(L, -2, "lerp");
    lua_pushcfunction(L, lua_Vector4MoveTowards_m);
    lua_setfield(L, -2, "moveTowards");
    lua_pushcfunction(L, lua_Vector4Invert_m);
    lua_setfield(L, -2, "invert");
    lua_setfield(L, -2, "__index");

    lua_pop(L, 1); // pop the metatable; it stays registered in the Lua registry
}

// ---- Matrix ----
//
// Exposed as transform-building functions (MatrixIdentity, MatrixTranslate,
// ...) rather than methods -- this mirrors raylib's own Matrix* naming and
// keeps scope to what's needed to build a transform for a shader uniform,
// not a full linear-algebra toolkit.

int lua_MatrixIdentity(lua_State *L) {
    pushMatrix(L, MatrixIdentity());
    return 1;
}

int lua_MatrixTranslate(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    pushMatrix(L, MatrixTranslate(x, y, z));
    return 1;
}

int lua_MatrixScale(lua_State *L) {
    float x = (float)luaL_checknumber(L, 1);
    float y = (float)luaL_checknumber(L, 2);
    float z = (float)luaL_checknumber(L, 3);
    pushMatrix(L, MatrixScale(x, y, z));
    return 1;
}

int lua_MatrixRotateX(lua_State *L) {
    float angle = (float)luaL_checknumber(L, 1);
    pushMatrix(L, MatrixRotateX(angle));
    return 1;
}

int lua_MatrixRotateY(lua_State *L) {
    float angle = (float)luaL_checknumber(L, 1);
    pushMatrix(L, MatrixRotateY(angle));
    return 1;
}

int lua_MatrixRotateZ(lua_State *L) {
    float angle = (float)luaL_checknumber(L, 1);
    pushMatrix(L, MatrixRotateZ(angle));
    return 1;
}

int lua_MatrixRotate(lua_State *L) {
    Vector3 axis  = lua_checkVector3(L, 1);
    float   angle = (float)luaL_checknumber(L, 2);
    pushMatrix(L, MatrixRotate(axis, angle));
    return 1;
}

int lua_MatrixRotateXYZ(lua_State *L) {
    Vector3 angle = lua_checkVector3(L, 1);
    pushMatrix(L, MatrixRotateXYZ(angle));
    return 1;
}

int lua_MatrixMultiply(lua_State *L) {
    Matrix left  = lua_checkMatrix(L, 1);
    Matrix right = lua_checkMatrix(L, 2);
    pushMatrix(L, MatrixMultiply(left, right));
    return 1;
}

int lua_MatrixTranspose(lua_State *L) {
    Matrix mat = lua_checkMatrix(L, 1);
    pushMatrix(L, MatrixTranspose(mat));
    return 1;
}

int lua_MatrixMul_mm(lua_State *L) {
    Matrix left  = lua_checkMatrix(L, 1);
    Matrix right = lua_checkMatrix(L, 2);
    pushMatrix(L, MatrixMultiply(left, right));
    return 1;
}

int lua_MatrixToString_mm(lua_State *L) {
    lua_checkMatrix(L, 1); // validate shape
    lua_pushstring(L, "Matrix");
    return 1;
}

static void registerMatrixMetatable(lua_State *L) {
    luaL_newmetatable(L, MATRIX_METATABLE);

    lua_pushcfunction(L, lua_MatrixMul_mm);
    lua_setfield(L, -2, "__mul");

    lua_pushcfunction(L, lua_MatrixToString_mm);
    lua_setfield(L, -2, "__tostring");

    lua_pop(L, 1); // pop the metatable; it stays registered in the Lua registry
}

int lua_Lerp(lua_State *L) {
    float start  = (float)luaL_checknumber(L, 1);
    float stop   = (float)luaL_checknumber(L, 2);
    float amount = (float)luaL_checknumber(L, 3);
    lua_pushnumber(L, Lerp(start, stop, amount));
    return 1;
}

int lua_Clamp(lua_State *L) {
    float value = (float)luaL_checknumber(L, 1);
    float min   = (float)luaL_checknumber(L, 2);
    float max   = (float)luaL_checknumber(L, 3);
    lua_pushnumber(L, Clamp(value, min, max));
    return 1;
}

int lua_LoadTexture(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    Texture2D   texture  = LoadTexture(fileName);
    pushTexture2D(L, texture);
    return 1;
}

int lua_UnloadTexture(lua_State *L) {
    Texture2D texture = lua_checkTexture2D(L, 1);
    UnloadTexture(texture);
    return 0;
}

// ---- CPU-side images ----

int lua_LoadImage(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    Image       image    = LoadImage(fileName);

    Image *ud = (Image *)lua_newuserdata(L, sizeof(Image));
    *ud       = image;
    luaL_getmetatable(L, IMAGE_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_UnloadImage(lua_State *L) {
    Image *image = lua_checkImage(L, 1);
    UnloadImage(*image);
    return 0;
}

int lua_ExportImage(lua_State *L) {
    Image      *image    = lua_checkImage(L, 1);
    const char *fileName = luaL_checkstring(L, 2);
    lua_pushboolean(L, ExportImage(*image, fileName));
    return 1;
}

int lua_ImageResize(lua_State *L) {
    Image *image     = lua_checkImage(L, 1);
    int    newWidth  = (int)luaL_checkinteger(L, 2);
    int    newHeight = (int)luaL_checkinteger(L, 3);
    ImageResize(image, newWidth, newHeight);
    return 0;
}

int lua_ImageCrop(lua_State *L) {
    Image    *image = lua_checkImage(L, 1);
    Rectangle crop  = lua_checkRectangle(L, 2);
    ImageCrop(image, crop);
    return 0;
}

int lua_ImageFlipVertical(lua_State *L) {
    Image *image = lua_checkImage(L, 1);
    ImageFlipVertical(image);
    return 0;
}

int lua_ImageFlipHorizontal(lua_State *L) {
    Image *image = lua_checkImage(L, 1);
    ImageFlipHorizontal(image);
    return 0;
}

int lua_ImageRotate(lua_State *L) {
    Image *image   = lua_checkImage(L, 1);
    int    degrees = (int)luaL_checkinteger(L, 2);
    ImageRotate(image, degrees);
    return 0;
}

int lua_GetImageColor(lua_State *L) {
    Image *image = lua_checkImage(L, 1);
    int    x     = (int)luaL_checkinteger(L, 2);
    int    y     = (int)luaL_checkinteger(L, 3);
    pushColor(L, GetImageColor(*image, x, y));
    return 1;
}

int lua_ImageDrawPixel(lua_State *L) {
    Image *image = lua_checkImage(L, 1);
    int    posX  = (int)luaL_checkinteger(L, 2);
    int    posY  = (int)luaL_checkinteger(L, 3);
    Color  color = lua_checkColor(L, 4);
    ImageDrawPixel(image, posX, posY, color);
    return 0;
}

int lua_ImageDrawCircle(lua_State *L) {
    Image *image   = lua_checkImage(L, 1);
    int    centerX = (int)luaL_checkinteger(L, 2);
    int    centerY = (int)luaL_checkinteger(L, 3);
    int    radius  = (int)luaL_checkinteger(L, 4);
    Color  color   = lua_checkColor(L, 5);
    ImageDrawCircle(image, centerX, centerY, radius, color);
    return 0;
}

int lua_ImageDrawRectangle(lua_State *L) {
    Image *image  = lua_checkImage(L, 1);
    int    posX   = (int)luaL_checkinteger(L, 2);
    int    posY   = (int)luaL_checkinteger(L, 3);
    int    width  = (int)luaL_checkinteger(L, 4);
    int    height = (int)luaL_checkinteger(L, 5);
    Color  color  = lua_checkColor(L, 6);
    ImageDrawRectangle(image, posX, posY, width, height, color);
    return 0;
}

int lua_LoadTextureFromImage(lua_State *L) {
    Image    *image   = lua_checkImage(L, 1);
    Texture2D texture = LoadTextureFromImage(*image);
    pushTexture2D(L, texture);
    return 1;
}

int lua_DrawTexture(lua_State *L) {
    Texture2D texture = lua_checkTexture2D(L, 1);
    int       posX    = (int)luaL_checknumber(L, 2);
    int       posY    = (int)luaL_checknumber(L, 3);
    Color     tint    = lua_checkColor(L, 4);
    DrawTexture(texture, posX, posY, tint);
    return 0;
}

int lua_DrawTextureV(lua_State *L) {
    Texture2D texture  = lua_checkTexture2D(L, 1);
    Vector2   position = lua_checkVector2(L, 2);
    Color     tint     = lua_checkColor(L, 3);
    DrawTextureV(texture, position, tint);
    return 0;
}

int lua_DrawTextureEx(lua_State *L) {
    Texture2D texture  = lua_checkTexture2D(L, 1);
    Vector2   position = lua_checkVector2(L, 2);
    float     rotation = (float)luaL_checknumber(L, 3);
    float     scale    = (float)luaL_checknumber(L, 4);
    Color     tint     = lua_checkColor(L, 5);
    DrawTextureEx(texture, position, rotation, scale, tint);
    return 0;
}

int lua_DrawTextureRec(lua_State *L) {
    Texture2D texture  = lua_checkTexture2D(L, 1);
    Rectangle source   = lua_checkRectangle(L, 2);
    Vector2   position = lua_checkVector2(L, 3);
    Color     tint     = lua_checkColor(L, 4);
    DrawTextureRec(texture, source, position, tint);
    return 0;
}

int lua_DrawTexturePro(lua_State *L) {
    Texture2D texture  = lua_checkTexture2D(L, 1);
    Rectangle source   = lua_checkRectangle(L, 2);
    Rectangle dest     = lua_checkRectangle(L, 3);
    Vector2   origin   = lua_checkVector2(L, 4);
    float     rotation = (float)luaL_checknumber(L, 5);
    Color     tint     = lua_checkColor(L, 6);
    DrawTexturePro(texture, source, dest, origin, rotation, tint);
    return 0;
}

// ---- Render textures ----

int lua_LoadRenderTexture(lua_State *L) {
    int             width  = (int)luaL_checkinteger(L, 1);
    int             height = (int)luaL_checkinteger(L, 2);
    RenderTexture2D target = LoadRenderTexture(width, height);

    RenderTexture2D *ud =
        (RenderTexture2D *)lua_newuserdata(L, sizeof(RenderTexture2D));
    *ud = target;
    luaL_getmetatable(L, RENDERTEXTURE2D_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_UnloadRenderTexture(lua_State *L) {
    RenderTexture2D *target = lua_checkRenderTexture2D(L, 1);
    UnloadRenderTexture(*target);
    return 0;
}

int lua_BeginTextureMode(lua_State *L) {
    RenderTexture2D *target = lua_checkRenderTexture2D(L, 1);
    BeginTextureMode(*target);
    return 0;
}

int lua_EndTextureMode(lua_State *L) {
    EndTextureMode();
    return 0;
}

// ---- Shaders ----
//
// Uniform setters are split into typed helpers (Float/Vec2/Int/Texture)
// rather than one generic SetShaderValue(shader, loc, value, type) -- this
// project's shaders only need float/vec2/int/sampler2D uniforms, so there's
// no need to push raylib's SHADER_UNIFORM_* type constants into Lua or
// marshal an arbitrary value based on one.

int lua_LoadShader(lua_State *L) {
    const char *vsFileName =
        lua_isnil(L, 1) ? NULL : luaL_checkstring(L, 1);
    const char *fsFileName =
        lua_isnil(L, 2) ? NULL : luaL_checkstring(L, 2);
    Shader shader = LoadShader(vsFileName, fsFileName);

    Shader *ud = (Shader *)lua_newuserdata(L, sizeof(Shader));
    *ud        = shader;
    luaL_getmetatable(L, SHADER_METATABLE);
    lua_setmetatable(L, -2);
    return 1;
}

int lua_UnloadShader(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    UnloadShader(*shader);
    return 0;
}

int lua_IsShaderValid(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    lua_pushboolean(L, IsShaderValid(*shader));
    return 1;
}

int lua_GetShaderLocation(lua_State *L) {
    Shader     *shader      = lua_checkShader(L, 1);
    const char *uniformName = luaL_checkstring(L, 2);
    lua_pushinteger(L, GetShaderLocation(*shader, uniformName));
    return 1;
}

int lua_GetShaderLocationAttrib(lua_State *L) {
    Shader     *shader     = lua_checkShader(L, 1);
    const char *attribName = luaL_checkstring(L, 2);
    lua_pushinteger(L, GetShaderLocationAttrib(*shader, attribName));
    return 1;
}

int lua_SetShaderValueFloat(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    int     loc    = (int)luaL_checkinteger(L, 2);
    float   value  = (float)luaL_checknumber(L, 3);
    SetShaderValue(*shader, loc, &value, SHADER_UNIFORM_FLOAT);
    return 0;
}

int lua_SetShaderValueVec2(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    int     loc    = (int)luaL_checkinteger(L, 2);
    float   value[2];
    value[0] = (float)luaL_checknumber(L, 3);
    value[1] = (float)luaL_checknumber(L, 4);
    SetShaderValue(*shader, loc, value, SHADER_UNIFORM_VEC2);
    return 0;
}

int lua_SetShaderValueVec3(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    int     loc    = (int)luaL_checkinteger(L, 2);
    Vector3 vec    = lua_checkVector3(L, 3);
    float   value[3];
    value[0] = vec.x;
    value[1] = vec.y;
    value[2] = vec.z;
    SetShaderValue(*shader, loc, value, SHADER_UNIFORM_VEC3);
    return 0;
}

int lua_SetShaderValueVec4(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    int     loc    = (int)luaL_checkinteger(L, 2);
    Vector4 vec    = lua_checkVector4(L, 3);
    float   value[4];
    value[0] = vec.x;
    value[1] = vec.y;
    value[2] = vec.z;
    value[3] = vec.w;
    SetShaderValue(*shader, loc, value, SHADER_UNIFORM_VEC4);
    return 0;
}

int lua_SetShaderValueInt(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    int     loc    = (int)luaL_checkinteger(L, 2);
    int     value  = (int)luaL_checkinteger(L, 3);
    SetShaderValue(*shader, loc, &value, SHADER_UNIFORM_INT);
    return 0;
}

int lua_SetShaderValueMatrix(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    int     loc    = (int)luaL_checkinteger(L, 2);
    Matrix  mat    = lua_checkMatrix(L, 3);
    SetShaderValueMatrix(*shader, loc, mat);
    return 0;
}

int lua_SetShaderValueTexture(lua_State *L) {
    Shader   *shader  = lua_checkShader(L, 1);
    int       loc     = (int)luaL_checkinteger(L, 2);
    Texture2D texture = lua_checkTexture2D(L, 3);
    SetShaderValueTexture(*shader, loc, texture);
    return 0;
}

int lua_BeginShaderMode(lua_State *L) {
    Shader *shader = lua_checkShader(L, 1);
    BeginShaderMode(*shader);
    return 0;
}

int lua_EndShaderMode(lua_State *L) {
    EndShaderMode();
    return 0;
}

int lua_Camera2D(lua_State *L) {
    Vector2 offset   = lua_checkVector2(L, 1);
    Vector2 target   = lua_checkVector2(L, 2);
    float   rotation = (float)luaL_optnumber(L, 3, 0);
    float   zoom     = (float)luaL_optnumber(L, 4, 1);

    pushCamera2D(L,
                 (Camera2D){
                     .offset   = offset,
                     .target   = target,
                     .rotation = rotation,
                     .zoom     = zoom,
                 });
    return 1;
}

int lua_BeginMode2D(lua_State *L) {
    Camera2D camera = lua_checkCamera2D(L, 1);
    BeginMode2D(camera);
    return 0;
}

int lua_EndMode2D(lua_State *L) {
    EndMode2D();
    return 0;
}

int lua_GetScreenToWorld2D(lua_State *L) {
    Vector2  position = lua_checkVector2(L, 1);
    Camera2D camera   = lua_checkCamera2D(L, 2);
    pushVector2(L, GetScreenToWorld2D(position, camera));
    return 1;
}

int lua_GetWorldToScreen2D(lua_State *L) {
    Vector2  position = lua_checkVector2(L, 1);
    Camera2D camera   = lua_checkCamera2D(L, 2);
    pushVector2(L, GetWorldToScreen2D(position, camera));
    return 1;
}

int lua_GetTime(lua_State *L) {
    lua_pushnumber(L, GetTime());
    return 1;
}

// ---- Window & misc utility ----

int lua_SetWindowTitle(lua_State *L) {
    const char *title = luaL_checkstring(L, 1);
    SetWindowTitle(title);
    return 0;
}

int lua_SetWindowSize(lua_State *L) {
    int width  = (int)luaL_checkinteger(L, 1);
    int height = (int)luaL_checkinteger(L, 2);
    SetWindowSize(width, height);
    return 0;
}

int lua_ToggleFullscreen(lua_State *L) {
    ToggleFullscreen();
    return 0;
}

int lua_IsWindowFocused(lua_State *L) {
    lua_pushboolean(L, IsWindowFocused());
    return 1;
}

int lua_IsWindowResized(lua_State *L) {
    lua_pushboolean(L, IsWindowResized());
    return 1;
}

int lua_IsWindowMinimized(lua_State *L) {
    lua_pushboolean(L, IsWindowMinimized());
    return 1;
}

int lua_IsWindowMaximized(lua_State *L) {
    lua_pushboolean(L, IsWindowMaximized());
    return 1;
}

int lua_GetCurrentMonitor(lua_State *L) {
    lua_pushinteger(L, GetCurrentMonitor());
    return 1;
}

int lua_GetMonitorWidth(lua_State *L) {
    int monitor = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, GetMonitorWidth(monitor));
    return 1;
}

int lua_GetMonitorHeight(lua_State *L) {
    int monitor = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, GetMonitorHeight(monitor));
    return 1;
}

int lua_SetWindowPosition(lua_State *L) {
    int x = (int)luaL_checkinteger(L, 1);
    int y = (int)luaL_checkinteger(L, 2);
    SetWindowPosition(x, y);
    return 0;
}

int lua_GetWindowPosition(lua_State *L) {
    pushVector2(L, GetWindowPosition());
    return 1;
}

int lua_SetWindowMinSize(lua_State *L) {
    int width  = (int)luaL_checkinteger(L, 1);
    int height = (int)luaL_checkinteger(L, 2);
    SetWindowMinSize(width, height);
    return 0;
}

int lua_SetWindowMaxSize(lua_State *L) {
    int width  = (int)luaL_checkinteger(L, 1);
    int height = (int)luaL_checkinteger(L, 2);
    SetWindowMaxSize(width, height);
    return 0;
}

int lua_SetWindowOpacity(lua_State *L) {
    float opacity = (float)luaL_checknumber(L, 1);
    SetWindowOpacity(opacity);
    return 0;
}

int lua_GetWindowScaleDPI(lua_State *L) {
    pushVector2(L, GetWindowScaleDPI());
    return 1;
}

int lua_GetMonitorName(lua_State *L) {
    int         monitor = (int)luaL_checkinteger(L, 1);
    const char *name    = GetMonitorName(monitor);
    if (name == NULL) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, name);
    }
    return 1;
}

int lua_GetMonitorCount(lua_State *L) {
    lua_pushinteger(L, GetMonitorCount());
    return 1;
}

int lua_GetMonitorPosition(lua_State *L) {
    int monitor = (int)luaL_checkinteger(L, 1);
    pushVector2(L, GetMonitorPosition(monitor));
    return 1;
}

int lua_GetMonitorRefreshRate(lua_State *L) {
    int monitor = (int)luaL_checkinteger(L, 1);
    lua_pushinteger(L, GetMonitorRefreshRate(monitor));
    return 1;
}

int lua_TakeScreenshot(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    TakeScreenshot(fileName);
    return 0;
}

int lua_GetFPS(lua_State *L) {
    lua_pushinteger(L, GetFPS());
    return 1;
}

int lua_SetRandomSeed(lua_State *L) {
    unsigned int seed = (unsigned int)luaL_checkinteger(L, 1);
    SetRandomSeed(seed);
    return 0;
}

int lua_GetRandomValue(lua_State *L) {
    int min = (int)luaL_checkinteger(L, 1);
    int max = (int)luaL_checkinteger(L, 2);
    lua_pushinteger(L, GetRandomValue(min, max));
    return 1;
}

// ---- Filesystem ----

int lua_FileExists(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    lua_pushboolean(L, FileExists(fileName));
    return 1;
}

int lua_DirectoryExists(lua_State *L) {
    const char *dirPath = luaL_checkstring(L, 1);
    lua_pushboolean(L, DirectoryExists(dirPath));
    return 1;
}

// Returned as a normal Lua string (copied and immediately freed via
// UnloadFileText) rather than exposing LoadFileText/UnloadFileText
// separately -- there's no C-owned buffer for Lua code to have to manage.
int lua_LoadFileText(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    char       *text     = LoadFileText(fileName);
    if (text == NULL) {
        lua_pushnil(L);
    } else {
        lua_pushstring(L, text);
        UnloadFileText(text);
    }
    return 1;
}

int lua_SaveFileText(lua_State *L) {
    const char *fileName = luaL_checkstring(L, 1);
    const char *text     = luaL_checkstring(L, 2);
    lua_pushboolean(L, SaveFileText(fileName, text));
    return 1;
}

// Registers a raylib binding as a bare global (InitWindow, DrawText, ...)
// instead of a field on an "rl" namespace table -- there is no "rl" table.
#define pushLuaFunction(state, func, name)                                     \
    do {                                                                       \
        lua_pushcfunction((state), (func));                                    \
        lua_setglobal((state), (name));                                        \
    } while (0)

void pushLuaFunctions(lua_State *L) {
    luaL_newmetatable(L, SOUND_METATABLE);
    lua_pop(L, 1);
    luaL_newmetatable(L, MUSIC_METATABLE);
    lua_pop(L, 1);
    luaL_newmetatable(L, FONT_METATABLE);
    lua_pop(L, 1);
    luaL_newmetatable(L, SHADER_METATABLE);
    lua_pop(L, 1);
    registerRenderTexture2DMetatable(L);
    registerImageMetatable(L);
    registerVector2Metatable(L);
    registerVector3Metatable(L);
    registerVector4Metatable(L);
    registerMatrixMetatable(L);

    // functions
    pushLuaFunction(L, lua_InitWindow, "InitWindow");
    pushLuaFunction(L, lua_InitAudioDevice, "InitAudioDevice");
    pushLuaFunction(L, lua_SetTargetFPS, "SetTargetFPS");
    pushLuaFunction(L, lua_WindowShouldClose, "WindowShouldClose");
    pushLuaFunction(L, lua_BeginDrawing, "BeginDrawing");
    pushLuaFunction(L, lua_EndDrawing, "EndDrawing");
    pushLuaFunction(L, lua_CloseWindow, "CloseWindow");
    pushLuaFunction(L, lua_CloseAudioDevice, "CloseAudioDevice");
    pushLuaFunction(L, lua_ClearBackground, "ClearBackground");
    pushLuaFunction(L, lua_DrawPixel, "DrawPixel");
    pushLuaFunction(L, lua_DrawLine, "DrawLine");
    pushLuaFunction(L, lua_DrawCircle, "DrawCircle");
    pushLuaFunction(L, lua_DrawCircleLines, "DrawCircleLines");
    pushLuaFunction(L, lua_DrawRectangle, "DrawRectangle");
    pushLuaFunction(L, lua_DrawRectangleLines, "DrawRectangleLines");
    pushLuaFunction(L, lua_GetScreenWidth, "GetScreenWidth");
    pushLuaFunction(L, lua_GetScreenHeight, "GetScreenHeight");
    pushLuaFunction(L, lua_GetFrameTime, "GetFrameTime");
    pushLuaFunction(L, lua_IsKeyDown, "IsKeyDown");
    pushLuaFunction(L, lua_IsKeyPressed, "IsKeyPressed");
    pushLuaFunction(L, lua_IsKeyReleased, "IsKeyReleased");
    pushLuaFunction(L, lua_IsKeyUp, "IsKeyUp");
    pushLuaFunction(L, lua_IsMouseButtonDown, "IsMouseButtonDown");
    pushLuaFunction(L, lua_IsMouseButtonPressed, "IsMouseButtonPressed");
    pushLuaFunction(L, lua_IsMouseButtonReleased, "IsMouseButtonReleased");
    pushLuaFunction(L, lua_GetMousePosition, "GetMousePosition");
    pushLuaFunction(L, lua_GetMouseWheelMove, "GetMouseWheelMove");
    pushLuaFunction(L, lua_ShowCursor, "ShowCursor");
    pushLuaFunction(L, lua_HideCursor, "HideCursor");
    pushLuaFunction(L, lua_IsCursorHidden, "IsCursorHidden");
    pushLuaFunction(L, lua_EnableCursor, "EnableCursor");
    pushLuaFunction(L, lua_DisableCursor, "DisableCursor");
    pushLuaFunction(L, lua_IsCursorOnScreen, "IsCursorOnScreen");
    pushLuaFunction(L, lua_GetMouseDelta, "GetMouseDelta");
    pushLuaFunction(L, lua_SetMousePosition, "SetMousePosition");
    pushLuaFunction(L, lua_SetMouseOffset, "SetMouseOffset");
    pushLuaFunction(L, lua_SetMouseScale, "SetMouseScale");
    pushLuaFunction(L, lua_SetMouseCursor, "SetMouseCursor");
    pushLuaFunction(L, lua_GetKeyPressed, "GetKeyPressed");
    pushLuaFunction(L, lua_GetCharPressed, "GetCharPressed");
    pushLuaFunction(L, lua_SetExitKey, "SetExitKey");
    pushLuaFunction(L, lua_GetClipboardText, "GetClipboardText");
    pushLuaFunction(L, lua_SetClipboardText, "SetClipboardText");
    pushLuaFunction(L, lua_IsGamepadAvailable, "IsGamepadAvailable");
    pushLuaFunction(L, lua_GetGamepadName, "GetGamepadName");
    pushLuaFunction(L, lua_IsGamepadButtonDown, "IsGamepadButtonDown");
    pushLuaFunction(L, lua_IsGamepadButtonPressed, "IsGamepadButtonPressed");
    pushLuaFunction(L, lua_IsGamepadButtonReleased, "IsGamepadButtonReleased");
    pushLuaFunction(L, lua_IsGamepadButtonUp, "IsGamepadButtonUp");
    pushLuaFunction(L, lua_GetGamepadAxisCount, "GetGamepadAxisCount");
    pushLuaFunction(L, lua_GetGamepadAxisMovement, "GetGamepadAxisMovement");
    pushLuaFunction(L, lua_DrawText, "DrawText");
    pushLuaFunction(L, lua_MeasureText, "MeasureText");
    pushLuaFunction(L, lua_DrawFPS, "DrawFPS");
    pushLuaFunction(L, lua_LoadSound, "LoadSound");
    pushLuaFunction(L, lua_UnloadSound, "UnloadSound");
    pushLuaFunction(L, lua_PlaySound, "PlaySound");
    pushLuaFunction(L, lua_StopSound, "StopSound");
    pushLuaFunction(L, lua_IsSoundPlaying, "IsSoundPlaying");
    pushLuaFunction(L, lua_SetSoundVolume, "SetSoundVolume");
    pushLuaFunction(L, lua_SetSoundPitch, "SetSoundPitch");
    pushLuaFunction(L, lua_SetSoundPan, "SetSoundPan");
    pushLuaFunction(L, lua_LoadMusicStream, "LoadMusicStream");
    pushLuaFunction(L, lua_UnloadMusicStream, "UnloadMusicStream");
    pushLuaFunction(L, lua_PlayMusicStream, "PlayMusicStream");
    pushLuaFunction(L, lua_UpdateMusicStream, "UpdateMusicStream");
    pushLuaFunction(L, lua_StopMusicStream, "StopMusicStream");
    pushLuaFunction(L, lua_PauseMusicStream, "PauseMusicStream");
    pushLuaFunction(L, lua_ResumeMusicStream, "ResumeMusicStream");
    pushLuaFunction(L, lua_IsMusicStreamPlaying, "IsMusicStreamPlaying");
    pushLuaFunction(L, lua_SetMusicVolume, "SetMusicVolume");
    pushLuaFunction(L, lua_SetMusicPitch, "SetMusicPitch");
    pushLuaFunction(L, lua_SetMusicPan, "SetMusicPan");
    pushLuaFunction(L, lua_GetMusicTimeLength, "GetMusicTimeLength");
    pushLuaFunction(L, lua_GetMusicTimePlayed, "GetMusicTimePlayed");
    pushLuaFunction(L, lua_SeekMusicStream, "SeekMusicStream");
    pushLuaFunction(L, lua_Lerp, "Lerp");
    pushLuaFunction(L, lua_Clamp, "Clamp");
    pushLuaFunction(L, lua_LoadTexture, "LoadTexture");
    pushLuaFunction(L, lua_UnloadTexture, "UnloadTexture");
    pushLuaFunction(L, lua_LoadImage, "LoadImage");
    pushLuaFunction(L, lua_UnloadImage, "UnloadImage");
    pushLuaFunction(L, lua_ExportImage, "ExportImage");
    pushLuaFunction(L, lua_ImageResize, "ImageResize");
    pushLuaFunction(L, lua_ImageCrop, "ImageCrop");
    pushLuaFunction(L, lua_ImageFlipVertical, "ImageFlipVertical");
    pushLuaFunction(L, lua_ImageFlipHorizontal, "ImageFlipHorizontal");
    pushLuaFunction(L, lua_ImageRotate, "ImageRotate");
    pushLuaFunction(L, lua_GetImageColor, "GetImageColor");
    pushLuaFunction(L, lua_ImageDrawPixel, "ImageDrawPixel");
    pushLuaFunction(L, lua_ImageDrawCircle, "ImageDrawCircle");
    pushLuaFunction(L, lua_ImageDrawRectangle, "ImageDrawRectangle");
    pushLuaFunction(L, lua_LoadTextureFromImage, "LoadTextureFromImage");
    pushLuaFunction(L, lua_DrawTexture, "DrawTexture");
    pushLuaFunction(L, lua_DrawTextureV, "DrawTextureV");
    pushLuaFunction(L, lua_DrawTextureEx, "DrawTextureEx");
    pushLuaFunction(L, lua_DrawTextureRec, "DrawTextureRec");
    pushLuaFunction(L, lua_DrawTexturePro, "DrawTexturePro");
    pushLuaFunction(L, lua_LoadRenderTexture, "LoadRenderTexture");
    pushLuaFunction(L, lua_UnloadRenderTexture, "UnloadRenderTexture");
    pushLuaFunction(L, lua_BeginTextureMode, "BeginTextureMode");
    pushLuaFunction(L, lua_EndTextureMode, "EndTextureMode");
    pushLuaFunction(L, lua_LoadShader, "LoadShader");
    pushLuaFunction(L, lua_UnloadShader, "UnloadShader");
    pushLuaFunction(L, lua_IsShaderValid, "IsShaderValid");
    pushLuaFunction(L, lua_GetShaderLocation, "GetShaderLocation");
    pushLuaFunction(
        L, lua_GetShaderLocationAttrib, "GetShaderLocationAttrib");
    pushLuaFunction(L, lua_SetShaderValueFloat, "SetShaderValueFloat");
    pushLuaFunction(L, lua_SetShaderValueVec2, "SetShaderValueVec2");
    pushLuaFunction(L, lua_SetShaderValueVec3, "SetShaderValueVec3");
    pushLuaFunction(L, lua_SetShaderValueVec4, "SetShaderValueVec4");
    pushLuaFunction(L, lua_SetShaderValueInt, "SetShaderValueInt");
    pushLuaFunction(L, lua_SetShaderValueTexture, "SetShaderValueTexture");
    pushLuaFunction(L, lua_SetShaderValueMatrix, "SetShaderValueMatrix");
    pushLuaFunction(L, lua_BeginShaderMode, "BeginShaderMode");
    pushLuaFunction(L, lua_EndShaderMode, "EndShaderMode");
    pushLuaFunction(L, lua_Camera2D, "Camera2D");
    pushLuaFunction(L, lua_BeginMode2D, "BeginMode2D");
    pushLuaFunction(L, lua_EndMode2D, "EndMode2D");
    pushLuaFunction(L, lua_GetScreenToWorld2D, "GetScreenToWorld2D");
    pushLuaFunction(L, lua_GetWorldToScreen2D, "GetWorldToScreen2D");
    pushLuaFunction(L, lua_GetTime, "GetTime");
    pushLuaFunction(L, lua_LoadFont, "LoadFont");
    pushLuaFunction(L, lua_LoadFontEx, "LoadFontEx");
    pushLuaFunction(L, lua_UnloadFont, "UnloadFont");
    pushLuaFunction(L, lua_GetFontDefault, "GetFontDefault");
    pushLuaFunction(L, lua_DrawTextEx, "DrawTextEx");
    pushLuaFunction(L, lua_DrawTextPro, "DrawTextPro");
    pushLuaFunction(L, lua_MeasureTextEx, "MeasureTextEx");
    pushLuaFunction(L, lua_SetWindowTitle, "SetWindowTitle");
    pushLuaFunction(L, lua_SetWindowSize, "SetWindowSize");
    pushLuaFunction(L, lua_ToggleFullscreen, "ToggleFullscreen");
    pushLuaFunction(L, lua_IsWindowFocused, "IsWindowFocused");
    pushLuaFunction(L, lua_IsWindowResized, "IsWindowResized");
    pushLuaFunction(L, lua_IsWindowMinimized, "IsWindowMinimized");
    pushLuaFunction(L, lua_IsWindowMaximized, "IsWindowMaximized");
    pushLuaFunction(L, lua_GetCurrentMonitor, "GetCurrentMonitor");
    pushLuaFunction(L, lua_GetMonitorWidth, "GetMonitorWidth");
    pushLuaFunction(L, lua_GetMonitorHeight, "GetMonitorHeight");
    pushLuaFunction(L, lua_SetWindowPosition, "SetWindowPosition");
    pushLuaFunction(L, lua_GetWindowPosition, "GetWindowPosition");
    pushLuaFunction(L, lua_SetWindowMinSize, "SetWindowMinSize");
    pushLuaFunction(L, lua_SetWindowMaxSize, "SetWindowMaxSize");
    pushLuaFunction(L, lua_SetWindowOpacity, "SetWindowOpacity");
    pushLuaFunction(L, lua_GetWindowScaleDPI, "GetWindowScaleDPI");
    pushLuaFunction(L, lua_GetMonitorName, "GetMonitorName");
    pushLuaFunction(L, lua_GetMonitorCount, "GetMonitorCount");
    pushLuaFunction(L, lua_GetMonitorPosition, "GetMonitorPosition");
    pushLuaFunction(L, lua_GetMonitorRefreshRate, "GetMonitorRefreshRate");
    pushLuaFunction(L, lua_TakeScreenshot, "TakeScreenshot");
    pushLuaFunction(L, lua_GetFPS, "GetFPS");
    pushLuaFunction(L, lua_SetRandomSeed, "SetRandomSeed");
    pushLuaFunction(L, lua_GetRandomValue, "GetRandomValue");
    pushLuaFunction(L, lua_FileExists, "FileExists");
    pushLuaFunction(L, lua_DirectoryExists, "DirectoryExists");
    pushLuaFunction(L, lua_LoadFileText, "LoadFileText");
    pushLuaFunction(L, lua_SaveFileText, "SaveFileText");

    // classes
    pushLuaFunction(L, lua_Color, "Color");
    pushLuaFunction(L, lua_Vector2, "Vector2");
    pushLuaFunction(L, lua_Vector3, "Vector3");
    pushLuaFunction(L, lua_Vector4, "Vector4");
    pushLuaFunction(L, lua_Rectangle, "Rectangle");
    pushLuaFunction(L, lua_MatrixIdentity, "MatrixIdentity");
    pushLuaFunction(L, lua_MatrixTranslate, "MatrixTranslate");
    pushLuaFunction(L, lua_MatrixScale, "MatrixScale");
    pushLuaFunction(L, lua_MatrixRotateX, "MatrixRotateX");
    pushLuaFunction(L, lua_MatrixRotateY, "MatrixRotateY");
    pushLuaFunction(L, lua_MatrixRotateZ, "MatrixRotateZ");
    pushLuaFunction(L, lua_MatrixRotate, "MatrixRotate");
    pushLuaFunction(L, lua_MatrixRotateXYZ, "MatrixRotateXYZ");
    pushLuaFunction(L, lua_MatrixMultiply, "MatrixMultiply");
    pushLuaFunction(L, lua_MatrixTranspose, "MatrixTranspose");
    pushLuaFunction(L, lua_DrawCircleV, "DrawCircleV");
    pushLuaFunction(L, lua_DrawRectangleRec, "DrawRectangleRec");
    pushLuaFunction(L, lua_DrawRectanglePro, "DrawRectanglePro");
    pushLuaFunction(L, lua_CheckCollisionRecs, "CheckCollisionRecs");
    pushLuaFunction(L, lua_CheckCollisionCircles, "CheckCollisionCircles");
    pushLuaFunction(L, lua_CheckCollisionPointRec, "CheckCollisionPointRec");
    pushLuaFunction(L, lua_CheckCollisionCircleRec, "CheckCollisionCircleRec");
    pushLuaFunction(
        L, lua_CheckCollisionPointCircle, "CheckCollisionPointCircle");
    pushLuaFunction(
        L, lua_CheckCollisionPointTriangle, "CheckCollisionPointTriangle");
    pushLuaFunction(L, lua_GetCollisionRec, "GetCollisionRec");
    pushLuaFunction(L, lua_DrawLineEx, "DrawLineEx");
    pushLuaFunction(L, lua_DrawLineBezier, "DrawLineBezier");
    pushLuaFunction(L, lua_DrawCircleSector, "DrawCircleSector");
    pushLuaFunction(L, lua_DrawRing, "DrawRing");
    pushLuaFunction(L, lua_DrawEllipse, "DrawEllipse");
    pushLuaFunction(L, lua_DrawEllipseLines, "DrawEllipseLines");
    pushLuaFunction(L, lua_DrawRectangleRounded, "DrawRectangleRounded");
    pushLuaFunction(
        L, lua_DrawRectangleRoundedLines, "DrawRectangleRoundedLines");
    pushLuaFunction(L, lua_DrawRectangleGradientV, "DrawRectangleGradientV");
    pushLuaFunction(L, lua_DrawRectangleGradientH, "DrawRectangleGradientH");
    pushLuaFunction(L, lua_DrawTriangle, "DrawTriangle");
    pushLuaFunction(L, lua_DrawTriangleLines, "DrawTriangleLines");
    pushLuaFunction(L, lua_DrawPoly, "DrawPoly");
    pushLuaFunction(L, lua_DrawPolyLines, "DrawPolyLines");
    pushLuaFunction(L, lua_DrawSplineLinear, "DrawSplineLinear");
    pushLuaFunction(L, lua_DrawSplineBasis, "DrawSplineBasis");
    pushLuaFunction(L, lua_DrawSplineCatmullRom, "DrawSplineCatmullRom");
    pushLuaFunction(
        L, lua_DrawSplineBezierQuadratic, "DrawSplineBezierQuadratic");
    pushLuaFunction(L, lua_DrawSplineBezierCubic, "DrawSplineBezierCubic");

    // Color constants
    pushColorGlobal(L, LIGHTGRAY);
    pushColorGlobal(L, GRAY);
    pushColorGlobal(L, DARKGRAY);
    pushColorGlobal(L, YELLOW);
    pushColorGlobal(L, GOLD);
    pushColorGlobal(L, ORANGE);
    pushColorGlobal(L, PINK);
    pushColorGlobal(L, RED);
    pushColorGlobal(L, MAROON);
    pushColorGlobal(L, GREEN);
    pushColorGlobal(L, LIME);
    pushColorGlobal(L, DARKGREEN);
    pushColorGlobal(L, SKYBLUE);
    pushColorGlobal(L, BLUE);
    pushColorGlobal(L, DARKBLUE);
    pushColorGlobal(L, PURPLE);
    pushColorGlobal(L, VIOLET);
    pushColorGlobal(L, DARKPURPLE);
    pushColorGlobal(L, BEIGE);
    pushColorGlobal(L, BROWN);
    pushColorGlobal(L, DARKBROWN);
    pushColorGlobal(L, WHITE);
    pushColorGlobal(L, BLACK);
    pushColorGlobal(L, BLANK);
    pushColorGlobal(L, MAGENTA);
    pushColorGlobal(L, RAYWHITE);

    // Key constants
    pushIntGlobal(L, KEY_SPACE);
    pushIntGlobal(L, KEY_ESCAPE);
    pushIntGlobal(L, KEY_ENTER);
    pushIntGlobal(L, KEY_TAB);
    pushIntGlobal(L, KEY_BACKSPACE);
    pushIntGlobal(L, KEY_RIGHT);
    pushIntGlobal(L, KEY_LEFT);
    pushIntGlobal(L, KEY_DOWN);
    pushIntGlobal(L, KEY_UP);
    pushIntGlobal(L, KEY_LEFT_SHIFT);
    pushIntGlobal(L, KEY_LEFT_CONTROL);
    pushIntGlobal(L, KEY_LEFT_ALT);
    pushIntGlobal(L, KEY_RIGHT_SHIFT);
    pushIntGlobal(L, KEY_RIGHT_CONTROL);
    pushIntGlobal(L, KEY_RIGHT_ALT);
    pushIntGlobal(L, KEY_ZERO);
    pushIntGlobal(L, KEY_ONE);
    pushIntGlobal(L, KEY_TWO);
    pushIntGlobal(L, KEY_THREE);
    pushIntGlobal(L, KEY_FOUR);
    pushIntGlobal(L, KEY_FIVE);
    pushIntGlobal(L, KEY_SIX);
    pushIntGlobal(L, KEY_SEVEN);
    pushIntGlobal(L, KEY_EIGHT);
    pushIntGlobal(L, KEY_NINE);
    pushIntGlobal(L, KEY_A);
    pushIntGlobal(L, KEY_B);
    pushIntGlobal(L, KEY_C);
    pushIntGlobal(L, KEY_D);
    pushIntGlobal(L, KEY_E);
    pushIntGlobal(L, KEY_F);
    pushIntGlobal(L, KEY_G);
    pushIntGlobal(L, KEY_H);
    pushIntGlobal(L, KEY_I);
    pushIntGlobal(L, KEY_J);
    pushIntGlobal(L, KEY_K);
    pushIntGlobal(L, KEY_L);
    pushIntGlobal(L, KEY_M);
    pushIntGlobal(L, KEY_N);
    pushIntGlobal(L, KEY_O);
    pushIntGlobal(L, KEY_P);
    pushIntGlobal(L, KEY_Q);
    pushIntGlobal(L, KEY_R);
    pushIntGlobal(L, KEY_S);
    pushIntGlobal(L, KEY_T);
    pushIntGlobal(L, KEY_U);
    pushIntGlobal(L, KEY_V);
    pushIntGlobal(L, KEY_W);
    pushIntGlobal(L, KEY_X);
    pushIntGlobal(L, KEY_Y);
    pushIntGlobal(L, KEY_Z);
    pushIntGlobal(L, KEY_F1);
    pushIntGlobal(L, KEY_F2);
    pushIntGlobal(L, KEY_F3);
    pushIntGlobal(L, KEY_F4);
    pushIntGlobal(L, KEY_F5);
    pushIntGlobal(L, KEY_F6);
    pushIntGlobal(L, KEY_F7);
    pushIntGlobal(L, KEY_F8);
    pushIntGlobal(L, KEY_F9);
    pushIntGlobal(L, KEY_F10);
    pushIntGlobal(L, KEY_F11);
    pushIntGlobal(L, KEY_F12);

    // Mouse button constants
    pushIntGlobal(L, MOUSE_BUTTON_LEFT);
    pushIntGlobal(L, MOUSE_BUTTON_RIGHT);
    pushIntGlobal(L, MOUSE_BUTTON_MIDDLE);
    pushIntGlobal(L, MOUSE_BUTTON_SIDE);
    pushIntGlobal(L, MOUSE_BUTTON_EXTRA);

    // Gamepad button constants
    pushIntGlobal(L, GAMEPAD_BUTTON_UNKNOWN);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_FACE_UP);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_FACE_UP);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
    pushIntGlobal(L, GAMEPAD_BUTTON_MIDDLE_LEFT);
    pushIntGlobal(L, GAMEPAD_BUTTON_MIDDLE);
    pushIntGlobal(L, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    pushIntGlobal(L, GAMEPAD_BUTTON_LEFT_THUMB);
    pushIntGlobal(L, GAMEPAD_BUTTON_RIGHT_THUMB);

    // Gamepad axis constants
    pushIntGlobal(L, GAMEPAD_AXIS_LEFT_X);
    pushIntGlobal(L, GAMEPAD_AXIS_LEFT_Y);
    pushIntGlobal(L, GAMEPAD_AXIS_RIGHT_X);
    pushIntGlobal(L, GAMEPAD_AXIS_RIGHT_Y);
    pushIntGlobal(L, GAMEPAD_AXIS_LEFT_TRIGGER);
    pushIntGlobal(L, GAMEPAD_AXIS_RIGHT_TRIGGER);

    // Mouse cursor constants
    pushIntGlobal(L, MOUSE_CURSOR_DEFAULT);
    pushIntGlobal(L, MOUSE_CURSOR_ARROW);
    pushIntGlobal(L, MOUSE_CURSOR_IBEAM);
    pushIntGlobal(L, MOUSE_CURSOR_CROSSHAIR);
    pushIntGlobal(L, MOUSE_CURSOR_POINTING_HAND);
    pushIntGlobal(L, MOUSE_CURSOR_RESIZE_EW);
    pushIntGlobal(L, MOUSE_CURSOR_RESIZE_NS);
    pushIntGlobal(L, MOUSE_CURSOR_RESIZE_NWSE);
    pushIntGlobal(L, MOUSE_CURSOR_RESIZE_NESW);
    pushIntGlobal(L, MOUSE_CURSOR_RESIZE_ALL);
    pushIntGlobal(L, MOUSE_CURSOR_NOT_ALLOWED);
}
