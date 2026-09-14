---@meta

---@class rl
rl = {}

---@class Color
---@field r number
---@field g number
---@field b number
---@field a number

---@class Vector2
---@field x number
---@field y number
---@operator add(Vector2): Vector2
---@operator sub(Vector2): Vector2
---@operator mul(number): Vector2
---@operator unm(): Vector2
---@field length fun(self: Vector2): number
---@field normalize fun(self: Vector2): Vector2
---@field dot fun(self: Vector2, other: Vector2): number
---@field distance fun(self: Vector2, other: Vector2): number
---@field lerp fun(self: Vector2, other: Vector2, amount: number): Vector2
---@field rotate fun(self: Vector2, angle: number): Vector2
---@field angle fun(self: Vector2, other: Vector2): number
---@field reflect fun(self: Vector2, normal: Vector2): Vector2
---@field moveTowards fun(self: Vector2, target: Vector2, maxDistance: number): Vector2
---@field clamp fun(self: Vector2, min: Vector2, max: Vector2): Vector2
---@field invert fun(self: Vector2): Vector2

---@class Rectangle
---@field x number
---@field y number
---@field width number
---@field height number

---@class Texture2D
---@field id integer
---@field width integer
---@field height integer
---@field mipmaps integer
---@field format integer

---@class Camera2D
---@field offset Vector2
---@field target Vector2
---@field rotation number
---@field zoom number

---@class Sound

---@class Music

---@class Font

---@class RenderTexture2D
---@field id integer
---@field texture Texture2D
---@field depth Texture2D

---@param r number
---@param g number
---@param b number
---@param a? number
---@return Color
function rl.Color(r, g, b, a) end

---@param x number
---@param y number
---@return Vector2
function rl.Vector2(x, y) end

---@param x number
---@param y number
---@param width number
---@param height number
---@return Rectangle
function rl.Rectangle(x, y, width, height) end

---@type Color
LIGHTGRAY = nil
---@type Color
GRAY = nil
---@type Color
DARKGRAY = nil
---@type Color
YELLOW = nil
---@type Color
GOLD = nil
---@type Color
ORANGE = nil
---@type Color
PINK = nil
---@type Color
RED = nil
---@type Color
MAROON = nil
---@type Color
GREEN = nil
---@type Color
LIME = nil
---@type Color
DARKGREEN = nil
---@type Color
SKYBLUE = nil
---@type Color
BLUE = nil
---@type Color
DARKBLUE = nil
---@type Color
PURPLE = nil
---@type Color
VIOLET = nil
---@type Color
DARKPURPLE = nil
---@type Color
BEIGE = nil
---@type Color
BROWN = nil
---@type Color
DARKBROWN = nil
---@type Color
WHITE = nil
---@type Color
BLACK = nil
---@type Color
BLANK = nil
---@type Color
MAGENTA = nil
---@type Color
RAYWHITE = nil

---@type integer
KEY_SPACE = nil
---@type integer
KEY_ESCAPE = nil
---@type integer
KEY_ENTER = nil
---@type integer
KEY_TAB = nil
---@type integer
KEY_BACKSPACE = nil
---@type integer
KEY_RIGHT = nil
---@type integer
KEY_LEFT = nil
---@type integer
KEY_DOWN = nil
---@type integer
KEY_UP = nil
---@type integer
KEY_LEFT_SHIFT = nil
---@type integer
KEY_LEFT_CONTROL = nil
---@type integer
KEY_LEFT_ALT = nil
---@type integer
KEY_RIGHT_SHIFT = nil
---@type integer
KEY_RIGHT_CONTROL = nil
---@type integer
KEY_RIGHT_ALT = nil
---@type integer
KEY_ZERO = nil
---@type integer
KEY_ONE = nil
---@type integer
KEY_TWO = nil
---@type integer
KEY_THREE = nil
---@type integer
KEY_FOUR = nil
---@type integer
KEY_FIVE = nil
---@type integer
KEY_SIX = nil
---@type integer
KEY_SEVEN = nil
---@type integer
KEY_EIGHT = nil
---@type integer
KEY_NINE = nil
---@type integer
KEY_A = nil
---@type integer
KEY_B = nil
---@type integer
KEY_C = nil
---@type integer
KEY_D = nil
---@type integer
KEY_E = nil
---@type integer
KEY_F = nil
---@type integer
KEY_G = nil
---@type integer
KEY_H = nil
---@type integer
KEY_I = nil
---@type integer
KEY_J = nil
---@type integer
KEY_K = nil
---@type integer
KEY_L = nil
---@type integer
KEY_M = nil
---@type integer
KEY_N = nil
---@type integer
KEY_O = nil
---@type integer
KEY_P = nil
---@type integer
KEY_Q = nil
---@type integer
KEY_R = nil
---@type integer
KEY_S = nil
---@type integer
KEY_T = nil
---@type integer
KEY_U = nil
---@type integer
KEY_V = nil
---@type integer
KEY_W = nil
---@type integer
KEY_X = nil
---@type integer
KEY_Y = nil
---@type integer
KEY_Z = nil
---@type integer
KEY_F1 = nil
---@type integer
KEY_F2 = nil
---@type integer
KEY_F3 = nil
---@type integer
KEY_F4 = nil
---@type integer
KEY_F5 = nil
---@type integer
KEY_F6 = nil
---@type integer
KEY_F7 = nil
---@type integer
KEY_F8 = nil
---@type integer
KEY_F9 = nil
---@type integer
KEY_F10 = nil
---@type integer
KEY_F11 = nil
---@type integer
KEY_F12 = nil

---@type integer
MOUSE_BUTTON_LEFT = nil
---@type integer
MOUSE_BUTTON_RIGHT = nil
---@type integer
MOUSE_BUTTON_MIDDLE = nil
---@type integer
MOUSE_BUTTON_SIDE = nil
---@type integer
MOUSE_BUTTON_EXTRA = nil

---@type integer
GAMEPAD_BUTTON_UNKNOWN = nil
---@type integer
GAMEPAD_BUTTON_LEFT_FACE_UP = nil
---@type integer
GAMEPAD_BUTTON_LEFT_FACE_RIGHT = nil
---@type integer
GAMEPAD_BUTTON_LEFT_FACE_DOWN = nil
---@type integer
GAMEPAD_BUTTON_LEFT_FACE_LEFT = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_FACE_UP = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_FACE_RIGHT = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_FACE_DOWN = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_FACE_LEFT = nil
---@type integer
GAMEPAD_BUTTON_LEFT_TRIGGER_1 = nil
---@type integer
GAMEPAD_BUTTON_LEFT_TRIGGER_2 = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_TRIGGER_1 = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_TRIGGER_2 = nil
---@type integer
GAMEPAD_BUTTON_MIDDLE_LEFT = nil
---@type integer
GAMEPAD_BUTTON_MIDDLE = nil
---@type integer
GAMEPAD_BUTTON_MIDDLE_RIGHT = nil
---@type integer
GAMEPAD_BUTTON_LEFT_THUMB = nil
---@type integer
GAMEPAD_BUTTON_RIGHT_THUMB = nil

---@type integer
GAMEPAD_AXIS_LEFT_X = nil
---@type integer
GAMEPAD_AXIS_LEFT_Y = nil
---@type integer
GAMEPAD_AXIS_RIGHT_X = nil
---@type integer
GAMEPAD_AXIS_RIGHT_Y = nil
---@type integer
GAMEPAD_AXIS_LEFT_TRIGGER = nil
---@type integer
GAMEPAD_AXIS_RIGHT_TRIGGER = nil

---@param color Color
function rl.ClearBackground(color) end

---@param x number
---@param y number
---@param color Color
function rl.DrawPixel(x, y, color) end

---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param color Color
function rl.DrawLine(x1, y1, x2, y2, color) end

---@param centerX number
---@param centerY number
---@param radius number
---@param color Color
function rl.DrawCircle(centerX, centerY, radius, color) end

---@param centerX number
---@param centerY number
---@param radius number
---@param color Color
function rl.DrawCircleLines(centerX, centerY, radius, color) end

---@param x number
---@param y number
---@param width number
---@param height number
---@param color Color
function rl.DrawRectangle(x, y, width, height, color) end

---@param x number
---@param y number
---@param width number
---@param height number
---@param color Color
function rl.DrawRectangleLines(x, y, width, height, color) end

---@param center Vector2
---@param radius number
---@param color Color
function rl.DrawCircleV(center, radius, color) end

---@param rec Rectangle
---@param color Color
function rl.DrawRectangleRec(rec, color) end

---@param rec Rectangle
---@param origin Vector2
---@param rotation number
---@param color Color
function rl.DrawRectanglePro(rec, origin, rotation, color) end

---@param rec1 Rectangle
---@param rec2 Rectangle
---@return boolean
function rl.CheckCollisionRecs(rec1, rec2) end

---@param center1 Vector2
---@param radius1 number
---@param center2 Vector2
---@param radius2 number
---@return boolean
function rl.CheckCollisionCircles(center1, radius1, center2, radius2) end

---@param point Vector2
---@param rec Rectangle
---@return boolean
function rl.CheckCollisionPointRec(point, rec) end

---@param center Vector2
---@param radius number
---@param rec Rectangle
---@return boolean
function rl.CheckCollisionCircleRec(center, radius, rec) end

---@param point Vector2
---@param center Vector2
---@param radius number
---@return boolean
function rl.CheckCollisionPointCircle(point, center, radius) end

---@param point Vector2
---@param p1 Vector2
---@param p2 Vector2
---@param p3 Vector2
---@return boolean
function rl.CheckCollisionPointTriangle(point, p1, p2, p3) end

---@param rec1 Rectangle
---@param rec2 Rectangle
---@return Rectangle
function rl.GetCollisionRec(rec1, rec2) end

---@param startPos Vector2
---@param endPos Vector2
---@param thick number
---@param color Color
function rl.DrawLineEx(startPos, endPos, thick, color) end

---@param startPos Vector2
---@param endPos Vector2
---@param thick number
---@param color Color
function rl.DrawLineBezier(startPos, endPos, thick, color) end

---@param center Vector2
---@param radius number
---@param startAngle number
---@param endAngle number
---@param segments integer
---@param color Color
function rl.DrawCircleSector(center, radius, startAngle, endAngle, segments, color) end

---@param center Vector2
---@param innerRadius number
---@param outerRadius number
---@param startAngle number
---@param endAngle number
---@param segments integer
---@param color Color
function rl.DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, segments, color) end

---@param centerX number
---@param centerY number
---@param radiusH number
---@param radiusV number
---@param color Color
function rl.DrawEllipse(centerX, centerY, radiusH, radiusV, color) end

---@param centerX number
---@param centerY number
---@param radiusH number
---@param radiusV number
---@param color Color
function rl.DrawEllipseLines(centerX, centerY, radiusH, radiusV, color) end

---@param rec Rectangle
---@param roundness number
---@param segments integer
---@param color Color
function rl.DrawRectangleRounded(rec, roundness, segments, color) end

---@param rec Rectangle
---@param roundness number
---@param segments integer
---@param color Color
function rl.DrawRectangleRoundedLines(rec, roundness, segments, color) end

---@param posX number
---@param posY number
---@param width number
---@param height number
---@param top Color
---@param bottom Color
function rl.DrawRectangleGradientV(posX, posY, width, height, top, bottom) end

---@param posX number
---@param posY number
---@param width number
---@param height number
---@param left Color
---@param right Color
function rl.DrawRectangleGradientH(posX, posY, width, height, left, right) end

---@param v1 Vector2
---@param v2 Vector2
---@param v3 Vector2
---@param color Color
function rl.DrawTriangle(v1, v2, v3, color) end

---@param v1 Vector2
---@param v2 Vector2
---@param v3 Vector2
---@param color Color
function rl.DrawTriangleLines(v1, v2, v3, color) end

---@param center Vector2
---@param sides integer
---@param radius number
---@param rotation number
---@param color Color
function rl.DrawPoly(center, sides, radius, rotation, color) end

---@param center Vector2
---@param sides integer
---@param radius number
---@param rotation number
---@param color Color
function rl.DrawPolyLines(center, sides, radius, rotation, color) end

---@param key integer
---@return boolean
function rl.IsKeyDown(key) end

---@param key integer
---@return boolean
function rl.IsKeyPressed(key) end

---@param key integer
---@return boolean
function rl.IsKeyReleased(key) end

---@param key integer
---@return boolean
function rl.IsKeyUp(key) end

---@param button integer
---@return boolean
function rl.IsMouseButtonDown(button) end

---@param button integer
---@return boolean
function rl.IsMouseButtonPressed(button) end

---@param button integer
---@return boolean
function rl.IsMouseButtonReleased(button) end

---@return Vector2
function rl.GetMousePosition() end

---@return number
function rl.GetMouseWheelMove() end

---@param gamepad integer
---@return boolean
function rl.IsGamepadAvailable(gamepad) end

---@param gamepad integer
---@return string?
function rl.GetGamepadName(gamepad) end

---@param gamepad integer
---@param button integer
---@return boolean
function rl.IsGamepadButtonDown(gamepad, button) end

---@param gamepad integer
---@param button integer
---@return boolean
function rl.IsGamepadButtonPressed(gamepad, button) end

---@param gamepad integer
---@param button integer
---@return boolean
function rl.IsGamepadButtonReleased(gamepad, button) end

---@param gamepad integer
---@param button integer
---@return boolean
function rl.IsGamepadButtonUp(gamepad, button) end

---@param gamepad integer
---@return integer
function rl.GetGamepadAxisCount(gamepad) end

---@param gamepad integer
---@param axis integer
---@return number
function rl.GetGamepadAxisMovement(gamepad, axis) end

---@param text string
---@param posX number
---@param posY number
---@param fontSize number
---@param color Color
function rl.DrawText(text, posX, posY, fontSize, color) end

---@param text string
---@param fontSize number
---@return integer
function rl.MeasureText(text, fontSize) end

---@param posX number
---@param posY number
function rl.DrawFPS(posX, posY) end

---@param fileName string
---@return Sound
function rl.LoadSound(fileName) end

---@param sound Sound
function rl.UnloadSound(sound) end

---@param sound Sound
function rl.PlaySound(sound) end

---@param sound Sound
function rl.StopSound(sound) end

---@param sound Sound
---@return boolean
function rl.IsSoundPlaying(sound) end

---@param sound Sound
---@param volume number
function rl.SetSoundVolume(sound, volume) end

---@param fileName string
---@return Music
function rl.LoadMusicStream(fileName) end

---@param music Music
function rl.UnloadMusicStream(music) end

---@param music Music
function rl.PlayMusicStream(music) end

---@param music Music
function rl.UpdateMusicStream(music) end

---@param music Music
function rl.StopMusicStream(music) end

---@param music Music
function rl.PauseMusicStream(music) end

---@param music Music
function rl.ResumeMusicStream(music) end

---@param music Music
---@return boolean
function rl.IsMusicStreamPlaying(music) end

---@param music Music
---@param volume number
function rl.SetMusicVolume(music, volume) end

---@param fileName string
---@return Font
function rl.LoadFont(fileName) end

---@param fileName string
---@param fontSize integer
---@return Font
function rl.LoadFontEx(fileName, fontSize) end

---@param font Font
function rl.UnloadFont(font) end

---@return Font
function rl.GetFontDefault() end

---@param font Font
---@param text string
---@param position Vector2
---@param fontSize number
---@param spacing number
---@param tint Color
function rl.DrawTextEx(font, text, position, fontSize, spacing, tint) end

---@param font Font
---@param text string
---@param position Vector2
---@param origin Vector2
---@param rotation number
---@param fontSize number
---@param spacing number
---@param tint Color
function rl.DrawTextPro(font, text, position, origin, rotation, fontSize, spacing, tint) end

---@param font Font
---@param text string
---@param fontSize number
---@param spacing number
---@return Vector2
function rl.MeasureTextEx(font, text, fontSize, spacing) end

---@param start number
---@param stop number
---@param amount number
---@return number
function rl.Lerp(start, stop, amount) end

---@param value number
---@param min number
---@param max number
---@return number
function rl.Clamp(value, min, max) end

---@param fileName string
---@return Texture2D
function rl.LoadTexture(fileName) end

---@param texture Texture2D
function rl.UnloadTexture(texture) end

---@param texture Texture2D
---@param posX number
---@param posY number
---@param tint Color
function rl.DrawTexture(texture, posX, posY, tint) end

---@param texture Texture2D
---@param position Vector2
---@param tint Color
function rl.DrawTextureV(texture, position, tint) end

---@param texture Texture2D
---@param position Vector2
---@param rotation number
---@param scale number
---@param tint Color
function rl.DrawTextureEx(texture, position, rotation, scale, tint) end

---@param texture Texture2D
---@param source Rectangle
---@param position Vector2
---@param tint Color
function rl.DrawTextureRec(texture, source, position, tint) end

---@param texture Texture2D
---@param source Rectangle
---@param dest Rectangle
---@param origin Vector2
---@param rotation number
---@param tint Color
function rl.DrawTexturePro(texture, source, dest, origin, rotation, tint) end

---@param width integer
---@param height integer
---@return RenderTexture2D
function rl.LoadRenderTexture(width, height) end

---@param target RenderTexture2D
function rl.UnloadRenderTexture(target) end

---@param target RenderTexture2D
function rl.BeginTextureMode(target) end

function rl.EndTextureMode() end

---@param offset Vector2
---@param target Vector2
---@param rotation? number
---@param zoom? number
---@return Camera2D
function rl.Camera2D(offset, target, rotation, zoom) end

---@param camera Camera2D
function rl.BeginMode2D(camera) end

function rl.EndMode2D() end

---@param position Vector2
---@param camera Camera2D
---@return Vector2
function rl.GetScreenToWorld2D(position, camera) end

---@param position Vector2
---@param camera Camera2D
---@return Vector2
function rl.GetWorldToScreen2D(position, camera) end

---@return number
function rl.GetTime() end

---@param title string
function rl.SetWindowTitle(title) end

---@param width integer
---@param height integer
function rl.SetWindowSize(width, height) end

function rl.ToggleFullscreen() end

---@return boolean
function rl.IsWindowFocused() end

---@return boolean
function rl.IsWindowResized() end

---@return boolean
function rl.IsWindowMinimized() end

---@return boolean
function rl.IsWindowMaximized() end

---@return integer
function rl.GetCurrentMonitor() end

---@param monitor integer
---@return integer
function rl.GetMonitorWidth(monitor) end

---@param monitor integer
---@return integer
function rl.GetMonitorHeight(monitor) end

---@return integer
function rl.GetFPS() end

---@param seed integer
function rl.SetRandomSeed(seed) end

---@param min integer
---@param max integer
---@return integer
function rl.GetRandomValue(min, max) end

---@return integer
function rl.GetScreenWidth() end

---@return integer
function rl.GetScreenHeight() end

---@return number
function rl.GetFrameTime() end

---@param width number
---@param height number
---@param title string
function rl.InitWindow(width, height, title) end

function rl.InitAudioDevice() end

---@param fps number
function rl.SetTargetFPS(fps) end

---@return boolean
function rl.WindowShouldClose() end

function rl.BeginDrawing() end

function rl.EndDrawing() end

function rl.CloseWindow() end

function rl.CloseAudioDevice() end
