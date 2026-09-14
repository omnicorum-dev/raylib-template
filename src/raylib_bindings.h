#pragma once

#include "lua.h"
#include <lauxlib.h>
#include <lualib.h>
#include <raylib.h>
#include <raymath.h>
#include <stdbool.h>

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

    return (Texture2D){
        .id = id, .width = width, .height = height, .mipmaps = mipmaps, .format = format};
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
    lua_setfield(L, -2, "__index");

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

int lua_Camera2D(lua_State *L) {
    Vector2 offset   = lua_checkVector2(L, 1);
    Vector2 target   = lua_checkVector2(L, 2);
    float   rotation = (float)luaL_optnumber(L, 3, 0);
    float   zoom     = (float)luaL_optnumber(L, 4, 1);

    pushCamera2D(L, (Camera2D){
                         .offset = offset,
                         .target = target,
                         .rotation = rotation,
                         .zoom = zoom,
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

#define pushLuaFunction(state, func, name)                                     \
    do {                                                                       \
        lua_pushcfunction((state), (func));                                    \
        lua_setfield((state), -2, (name));                                     \
    } while (0)

void pushLuaFunctions(lua_State *L) {
    luaL_newmetatable(L, SOUND_METATABLE);
    lua_pop(L, 1);
    luaL_newmetatable(L, MUSIC_METATABLE);
    lua_pop(L, 1);
    registerVector2Metatable(L);

    lua_newtable(L); // rl

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
    pushLuaFunction(L, lua_DrawText, "DrawText");
    pushLuaFunction(L, lua_MeasureText, "MeasureText");
    pushLuaFunction(L, lua_DrawFPS, "DrawFPS");
    pushLuaFunction(L, lua_LoadSound, "LoadSound");
    pushLuaFunction(L, lua_UnloadSound, "UnloadSound");
    pushLuaFunction(L, lua_PlaySound, "PlaySound");
    pushLuaFunction(L, lua_StopSound, "StopSound");
    pushLuaFunction(L, lua_IsSoundPlaying, "IsSoundPlaying");
    pushLuaFunction(L, lua_SetSoundVolume, "SetSoundVolume");
    pushLuaFunction(L, lua_LoadMusicStream, "LoadMusicStream");
    pushLuaFunction(L, lua_UnloadMusicStream, "UnloadMusicStream");
    pushLuaFunction(L, lua_PlayMusicStream, "PlayMusicStream");
    pushLuaFunction(L, lua_UpdateMusicStream, "UpdateMusicStream");
    pushLuaFunction(L, lua_StopMusicStream, "StopMusicStream");
    pushLuaFunction(L, lua_PauseMusicStream, "PauseMusicStream");
    pushLuaFunction(L, lua_ResumeMusicStream, "ResumeMusicStream");
    pushLuaFunction(L, lua_IsMusicStreamPlaying, "IsMusicStreamPlaying");
    pushLuaFunction(L, lua_SetMusicVolume, "SetMusicVolume");
    pushLuaFunction(L, lua_Lerp, "Lerp");
    pushLuaFunction(L, lua_Clamp, "Clamp");
    pushLuaFunction(L, lua_LoadTexture, "LoadTexture");
    pushLuaFunction(L, lua_UnloadTexture, "UnloadTexture");
    pushLuaFunction(L, lua_DrawTexture, "DrawTexture");
    pushLuaFunction(L, lua_DrawTextureV, "DrawTextureV");
    pushLuaFunction(L, lua_DrawTextureEx, "DrawTextureEx");
    pushLuaFunction(L, lua_DrawTextureRec, "DrawTextureRec");
    pushLuaFunction(L, lua_DrawTexturePro, "DrawTexturePro");
    pushLuaFunction(L, lua_Camera2D, "Camera2D");
    pushLuaFunction(L, lua_BeginMode2D, "BeginMode2D");
    pushLuaFunction(L, lua_EndMode2D, "EndMode2D");
    pushLuaFunction(L, lua_GetScreenToWorld2D, "GetScreenToWorld2D");
    pushLuaFunction(L, lua_GetWorldToScreen2D, "GetWorldToScreen2D");
    pushLuaFunction(L, lua_GetTime, "GetTime");

    // classes
    pushLuaFunction(L, lua_Color, "Color");
    pushLuaFunction(L, lua_Vector2, "Vector2");
    pushLuaFunction(L, lua_Rectangle, "Rectangle");
    pushLuaFunction(L, lua_DrawCircleV, "DrawCircleV");
    pushLuaFunction(L, lua_DrawRectangleRec, "DrawRectangleRec");
    pushLuaFunction(L, lua_CheckCollisionRecs, "CheckCollisionRecs");
    pushLuaFunction(L, lua_CheckCollisionCircles, "CheckCollisionCircles");
    pushLuaFunction(L, lua_CheckCollisionPointRec, "CheckCollisionPointRec");
    pushLuaFunction(L, lua_CheckCollisionCircleRec, "CheckCollisionCircleRec");

    lua_setglobal(L, "rl");

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
}
