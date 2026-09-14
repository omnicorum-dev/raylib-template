---@meta

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
---@field lengthSqr fun(self: Vector2): number
---@field distanceSqr fun(self: Vector2, other: Vector2): number
---@field lineAngle fun(self: Vector2, other: Vector2): number
---@field cross fun(self: Vector2, other: Vector2): number
---@field refract fun(self: Vector2, normal: Vector2, ratio: number): Vector2

---@class Vector3
---@field x number
---@field y number
---@field z number
---@operator add(Vector3): Vector3
---@operator sub(Vector3): Vector3
---@operator mul(number|Vector3): Vector3
---@operator unm(): Vector3
---@field length fun(self: Vector3): number
---@field lengthSqr fun(self: Vector3): number
---@field normalize fun(self: Vector3): Vector3
---@field dot fun(self: Vector3, other: Vector3): number
---@field cross fun(self: Vector3, other: Vector3): Vector3
---@field distance fun(self: Vector3, other: Vector3): number
---@field distanceSqr fun(self: Vector3, other: Vector3): number
---@field lerp fun(self: Vector3, other: Vector3, amount: number): Vector3
---@field angle fun(self: Vector3, other: Vector3): number
---@field reflect fun(self: Vector3, normal: Vector3): Vector3
---@field moveTowards fun(self: Vector3, target: Vector3, maxDistance: number): Vector3
---@field clamp fun(self: Vector3, min: Vector3, max: Vector3): Vector3
---@field invert fun(self: Vector3): Vector3
---@field rotateByAxisAngle fun(self: Vector3, axis: Vector3, angle: number): Vector3
---@field transform fun(self: Vector3, mat: Matrix): Vector3

---@class Vector4
---@field x number
---@field y number
---@field z number
---@field w number
---@operator add(Vector4): Vector4
---@operator sub(Vector4): Vector4
---@operator mul(number|Vector4): Vector4
---@operator unm(): Vector4
---@field length fun(self: Vector4): number
---@field lengthSqr fun(self: Vector4): number
---@field normalize fun(self: Vector4): Vector4
---@field dot fun(self: Vector4, other: Vector4): number
---@field distance fun(self: Vector4, other: Vector4): number
---@field distanceSqr fun(self: Vector4, other: Vector4): number
---@field lerp fun(self: Vector4, other: Vector4, amount: number): Vector4
---@field moveTowards fun(self: Vector4, target: Vector4, maxDistance: number): Vector4
---@field invert fun(self: Vector4): Vector4

---@class Matrix
---@field m0 number
---@field m1 number
---@field m2 number
---@field m3 number
---@field m4 number
---@field m5 number
---@field m6 number
---@field m7 number
---@field m8 number
---@field m9 number
---@field m10 number
---@field m11 number
---@field m12 number
---@field m13 number
---@field m14 number
---@field m15 number
---@operator mul(Matrix): Matrix

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

---@class Image
---@field width integer
---@field height integer
---@field mipmaps integer
---@field format integer

---@class Shader

---@param r number
---@param g number
---@param b number
---@param a? number
---@return Color
function Color(r, g, b, a) end

---@param x number
---@param y number
---@return Vector2
function Vector2(x, y) end

---@param x number
---@param y number
---@param z number
---@return Vector3
function Vector3(x, y, z) end

---@param x number
---@param y number
---@param z number
---@param w number
---@return Vector4
function Vector4(x, y, z, w) end

---@param x number
---@param y number
---@param width number
---@param height number
---@return Rectangle
function Rectangle(x, y, width, height) end

---@return Matrix
function MatrixIdentity() end

---@param x number
---@param y number
---@param z number
---@return Matrix
function MatrixTranslate(x, y, z) end

---@param x number
---@param y number
---@param z number
---@return Matrix
function MatrixScale(x, y, z) end

---@param angle number
---@return Matrix
function MatrixRotateX(angle) end

---@param angle number
---@return Matrix
function MatrixRotateY(angle) end

---@param angle number
---@return Matrix
function MatrixRotateZ(angle) end

---@param axis Vector3
---@param angle number
---@return Matrix
function MatrixRotate(axis, angle) end

---@param angle Vector3
---@return Matrix
function MatrixRotateXYZ(angle) end

---@param left Matrix
---@param right Matrix
---@return Matrix
function MatrixMultiply(left, right) end

---@param mat Matrix
---@return Matrix
function MatrixTranspose(mat) end

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

---@type integer
MOUSE_CURSOR_DEFAULT = nil
---@type integer
MOUSE_CURSOR_ARROW = nil
---@type integer
MOUSE_CURSOR_IBEAM = nil
---@type integer
MOUSE_CURSOR_CROSSHAIR = nil
---@type integer
MOUSE_CURSOR_POINTING_HAND = nil
---@type integer
MOUSE_CURSOR_RESIZE_EW = nil
---@type integer
MOUSE_CURSOR_RESIZE_NS = nil
---@type integer
MOUSE_CURSOR_RESIZE_NWSE = nil
---@type integer
MOUSE_CURSOR_RESIZE_NESW = nil
---@type integer
MOUSE_CURSOR_RESIZE_ALL = nil
---@type integer
MOUSE_CURSOR_NOT_ALLOWED = nil

---@param color Color
function ClearBackground(color) end

---@param x number
---@param y number
---@param color Color
function DrawPixel(x, y, color) end

---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param color Color
function DrawLine(x1, y1, x2, y2, color) end

---@param centerX number
---@param centerY number
---@param radius number
---@param color Color
function DrawCircle(centerX, centerY, radius, color) end

---@param centerX number
---@param centerY number
---@param radius number
---@param color Color
function DrawCircleLines(centerX, centerY, radius, color) end

---@param x number
---@param y number
---@param width number
---@param height number
---@param color Color
function DrawRectangle(x, y, width, height, color) end

---@param x number
---@param y number
---@param width number
---@param height number
---@param color Color
function DrawRectangleLines(x, y, width, height, color) end

---@param center Vector2
---@param radius number
---@param color Color
function DrawCircleV(center, radius, color) end

---@param rec Rectangle
---@param color Color
function DrawRectangleRec(rec, color) end

---@param rec Rectangle
---@param origin Vector2
---@param rotation number
---@param color Color
function DrawRectanglePro(rec, origin, rotation, color) end

---@param rec1 Rectangle
---@param rec2 Rectangle
---@return boolean
function CheckCollisionRecs(rec1, rec2) end

---@param center1 Vector2
---@param radius1 number
---@param center2 Vector2
---@param radius2 number
---@return boolean
function CheckCollisionCircles(center1, radius1, center2, radius2) end

---@param point Vector2
---@param rec Rectangle
---@return boolean
function CheckCollisionPointRec(point, rec) end

---@param center Vector2
---@param radius number
---@param rec Rectangle
---@return boolean
function CheckCollisionCircleRec(center, radius, rec) end

---@param point Vector2
---@param center Vector2
---@param radius number
---@return boolean
function CheckCollisionPointCircle(point, center, radius) end

---@param point Vector2
---@param p1 Vector2
---@param p2 Vector2
---@param p3 Vector2
---@return boolean
function CheckCollisionPointTriangle(point, p1, p2, p3) end

---@param rec1 Rectangle
---@param rec2 Rectangle
---@return Rectangle
function GetCollisionRec(rec1, rec2) end

---@param startPos Vector2
---@param endPos Vector2
---@param thick number
---@param color Color
function DrawLineEx(startPos, endPos, thick, color) end

---@param startPos Vector2
---@param endPos Vector2
---@param thick number
---@param color Color
function DrawLineBezier(startPos, endPos, thick, color) end

---@param center Vector2
---@param radius number
---@param startAngle number
---@param endAngle number
---@param segments integer
---@param color Color
function DrawCircleSector(center, radius, startAngle, endAngle, segments, color) end

---@param center Vector2
---@param innerRadius number
---@param outerRadius number
---@param startAngle number
---@param endAngle number
---@param segments integer
---@param color Color
function DrawRing(center, innerRadius, outerRadius, startAngle, endAngle, segments, color) end

---@param centerX number
---@param centerY number
---@param radiusH number
---@param radiusV number
---@param color Color
function DrawEllipse(centerX, centerY, radiusH, radiusV, color) end

---@param centerX number
---@param centerY number
---@param radiusH number
---@param radiusV number
---@param color Color
function DrawEllipseLines(centerX, centerY, radiusH, radiusV, color) end

---@param rec Rectangle
---@param roundness number
---@param segments integer
---@param color Color
function DrawRectangleRounded(rec, roundness, segments, color) end

---@param rec Rectangle
---@param roundness number
---@param segments integer
---@param color Color
function DrawRectangleRoundedLines(rec, roundness, segments, color) end

---@param posX number
---@param posY number
---@param width number
---@param height number
---@param top Color
---@param bottom Color
function DrawRectangleGradientV(posX, posY, width, height, top, bottom) end

---@param posX number
---@param posY number
---@param width number
---@param height number
---@param left Color
---@param right Color
function DrawRectangleGradientH(posX, posY, width, height, left, right) end

---@param v1 Vector2
---@param v2 Vector2
---@param v3 Vector2
---@param color Color
function DrawTriangle(v1, v2, v3, color) end

---@param v1 Vector2
---@param v2 Vector2
---@param v3 Vector2
---@param color Color
function DrawTriangleLines(v1, v2, v3, color) end

---@param center Vector2
---@param sides integer
---@param radius number
---@param rotation number
---@param color Color
function DrawPoly(center, sides, radius, rotation, color) end

---@param center Vector2
---@param sides integer
---@param radius number
---@param rotation number
---@param color Color
function DrawPolyLines(center, sides, radius, rotation, color) end

---@param points Vector2[]
---@param thick number
---@param color Color
function DrawSplineLinear(points, thick, color) end

---@param points Vector2[]
---@param thick number
---@param color Color
function DrawSplineBasis(points, thick, color) end

---@param points Vector2[]
---@param thick number
---@param color Color
function DrawSplineCatmullRom(points, thick, color) end

---@param points Vector2[]
---@param thick number
---@param color Color
function DrawSplineBezierQuadratic(points, thick, color) end

---@param points Vector2[]
---@param thick number
---@param color Color
function DrawSplineBezierCubic(points, thick, color) end

---@param key integer
---@return boolean
function IsKeyDown(key) end

---@param key integer
---@return boolean
function IsKeyPressed(key) end

---@param key integer
---@return boolean
function IsKeyReleased(key) end

---@param key integer
---@return boolean
function IsKeyUp(key) end

---@param button integer
---@return boolean
function IsMouseButtonDown(button) end

---@param button integer
---@return boolean
function IsMouseButtonPressed(button) end

---@param button integer
---@return boolean
function IsMouseButtonReleased(button) end

---@return Vector2
function GetMousePosition() end

---@return number
function GetMouseWheelMove() end

function ShowCursor() end

function HideCursor() end

---@return boolean
function IsCursorHidden() end

function EnableCursor() end

function DisableCursor() end

---@return boolean
function IsCursorOnScreen() end

---@return Vector2
function GetMouseDelta() end

---@param x integer
---@param y integer
function SetMousePosition(x, y) end

---@param offsetX integer
---@param offsetY integer
function SetMouseOffset(offsetX, offsetY) end

---@param scaleX number
---@param scaleY number
function SetMouseScale(scaleX, scaleY) end

---@param cursor integer
function SetMouseCursor(cursor) end

---@return integer
function GetKeyPressed() end

---@return integer
function GetCharPressed() end

---@param key integer
function SetExitKey(key) end

---@return string?
function GetClipboardText() end

---@param text string
function SetClipboardText(text) end

---@param gamepad integer
---@return boolean
function IsGamepadAvailable(gamepad) end

---@param gamepad integer
---@return string?
function GetGamepadName(gamepad) end

---@param gamepad integer
---@param button integer
---@return boolean
function IsGamepadButtonDown(gamepad, button) end

---@param gamepad integer
---@param button integer
---@return boolean
function IsGamepadButtonPressed(gamepad, button) end

---@param gamepad integer
---@param button integer
---@return boolean
function IsGamepadButtonReleased(gamepad, button) end

---@param gamepad integer
---@param button integer
---@return boolean
function IsGamepadButtonUp(gamepad, button) end

---@param gamepad integer
---@return integer
function GetGamepadAxisCount(gamepad) end

---@param gamepad integer
---@param axis integer
---@return number
function GetGamepadAxisMovement(gamepad, axis) end

---@param text string
---@param posX number
---@param posY number
---@param fontSize number
---@param color Color
function DrawText(text, posX, posY, fontSize, color) end

---@param text string
---@param fontSize number
---@return integer
function MeasureText(text, fontSize) end

---@param posX number
---@param posY number
function DrawFPS(posX, posY) end

---@param fileName string
---@return Sound
function LoadSound(fileName) end

---@param sound Sound
function UnloadSound(sound) end

---@param sound Sound
function PlaySound(sound) end

---@param sound Sound
function StopSound(sound) end

---@param sound Sound
---@return boolean
function IsSoundPlaying(sound) end

---@param sound Sound
---@param volume number
function SetSoundVolume(sound, volume) end

---@param sound Sound
---@param pitch number
function SetSoundPitch(sound, pitch) end

---@param sound Sound
---@param pan number
function SetSoundPan(sound, pan) end

---@param fileName string
---@return Music
function LoadMusicStream(fileName) end

---@param music Music
function UnloadMusicStream(music) end

---@param music Music
function PlayMusicStream(music) end

---@param music Music
function UpdateMusicStream(music) end

---@param music Music
function StopMusicStream(music) end

---@param music Music
function PauseMusicStream(music) end

---@param music Music
function ResumeMusicStream(music) end

---@param music Music
---@return boolean
function IsMusicStreamPlaying(music) end

---@param music Music
---@param volume number
function SetMusicVolume(music, volume) end

---@param music Music
---@param pitch number
function SetMusicPitch(music, pitch) end

---@param music Music
---@param pan number
function SetMusicPan(music, pan) end

---@param music Music
---@return number
function GetMusicTimeLength(music) end

---@param music Music
---@return number
function GetMusicTimePlayed(music) end

---@param music Music
---@param position number
function SeekMusicStream(music, position) end

---@param fileName string
---@return Font
function LoadFont(fileName) end

---@param fileName string
---@param fontSize integer
---@return Font
function LoadFontEx(fileName, fontSize) end

---@param font Font
function UnloadFont(font) end

---@return Font
function GetFontDefault() end

---@param font Font
---@param text string
---@param position Vector2
---@param fontSize number
---@param spacing number
---@param tint Color
function DrawTextEx(font, text, position, fontSize, spacing, tint) end

---@param font Font
---@param text string
---@param position Vector2
---@param origin Vector2
---@param rotation number
---@param fontSize number
---@param spacing number
---@param tint Color
function DrawTextPro(font, text, position, origin, rotation, fontSize, spacing, tint) end

---@param font Font
---@param text string
---@param fontSize number
---@param spacing number
---@return Vector2
function MeasureTextEx(font, text, fontSize, spacing) end

---@param start number
---@param stop number
---@param amount number
---@return number
function Lerp(start, stop, amount) end

---@param value number
---@param min number
---@param max number
---@return number
function Clamp(value, min, max) end

---@param fileName string
---@return Texture2D
function LoadTexture(fileName) end

---@param texture Texture2D
function UnloadTexture(texture) end

---@param fileName string
---@return Image
function LoadImage(fileName) end

---@param image Image
function UnloadImage(image) end

---@param image Image
---@param fileName string
---@return boolean
function ExportImage(image, fileName) end

---@param image Image
---@param newWidth integer
---@param newHeight integer
function ImageResize(image, newWidth, newHeight) end

---@param image Image
---@param crop Rectangle
function ImageCrop(image, crop) end

---@param image Image
function ImageFlipVertical(image) end

---@param image Image
function ImageFlipHorizontal(image) end

---@param image Image
---@param degrees integer
function ImageRotate(image, degrees) end

---@param image Image
---@param x integer
---@param y integer
---@return Color
function GetImageColor(image, x, y) end

---@param image Image
---@param posX integer
---@param posY integer
---@param color Color
function ImageDrawPixel(image, posX, posY, color) end

---@param image Image
---@param centerX integer
---@param centerY integer
---@param radius integer
---@param color Color
function ImageDrawCircle(image, centerX, centerY, radius, color) end

---@param image Image
---@param posX integer
---@param posY integer
---@param width integer
---@param height integer
---@param color Color
function ImageDrawRectangle(image, posX, posY, width, height, color) end

---@param image Image
---@return Texture2D
function LoadTextureFromImage(image) end

---@param texture Texture2D
---@param posX number
---@param posY number
---@param tint Color
function DrawTexture(texture, posX, posY, tint) end

---@param texture Texture2D
---@param position Vector2
---@param tint Color
function DrawTextureV(texture, position, tint) end

---@param texture Texture2D
---@param position Vector2
---@param rotation number
---@param scale number
---@param tint Color
function DrawTextureEx(texture, position, rotation, scale, tint) end

---@param texture Texture2D
---@param source Rectangle
---@param position Vector2
---@param tint Color
function DrawTextureRec(texture, source, position, tint) end

---@param texture Texture2D
---@param source Rectangle
---@param dest Rectangle
---@param origin Vector2
---@param rotation number
---@param tint Color
function DrawTexturePro(texture, source, dest, origin, rotation, tint) end

---@param width integer
---@param height integer
---@return RenderTexture2D
function LoadRenderTexture(width, height) end

---@param target RenderTexture2D
function UnloadRenderTexture(target) end

---@param target RenderTexture2D
function BeginTextureMode(target) end

function EndTextureMode() end

---@param vsFileName string?
---@param fsFileName string?
---@return Shader
function LoadShader(vsFileName, fsFileName) end

---@param shader Shader
function UnloadShader(shader) end

---@param shader Shader
---@return boolean
function IsShaderValid(shader) end

---@param shader Shader
---@param uniformName string
---@return integer
function GetShaderLocation(shader, uniformName) end

---@param shader Shader
---@param attribName string
---@return integer
function GetShaderLocationAttrib(shader, attribName) end

---@param shader Shader
---@param loc integer
---@param value number
function SetShaderValueFloat(shader, loc, value) end

---@param shader Shader
---@param loc integer
---@param x number
---@param y number
function SetShaderValueVec2(shader, loc, x, y) end

---@param shader Shader
---@param loc integer
---@param vec Vector3
function SetShaderValueVec3(shader, loc, vec) end

---@param shader Shader
---@param loc integer
---@param vec Vector4
function SetShaderValueVec4(shader, loc, vec) end

---@param shader Shader
---@param loc integer
---@param value integer
function SetShaderValueInt(shader, loc, value) end

---@param shader Shader
---@param loc integer
---@param texture Texture2D
function SetShaderValueTexture(shader, loc, texture) end

---@param shader Shader
---@param loc integer
---@param mat Matrix
function SetShaderValueMatrix(shader, loc, mat) end

---@param shader Shader
function BeginShaderMode(shader) end

function EndShaderMode() end

---@param offset Vector2
---@param target Vector2
---@param rotation? number
---@param zoom? number
---@return Camera2D
function Camera2D(offset, target, rotation, zoom) end

---@param camera Camera2D
function BeginMode2D(camera) end

function EndMode2D() end

---@param position Vector2
---@param camera Camera2D
---@return Vector2
function GetScreenToWorld2D(position, camera) end

---@param position Vector2
---@param camera Camera2D
---@return Vector2
function GetWorldToScreen2D(position, camera) end

---@return number
function GetTime() end

---@param title string
function SetWindowTitle(title) end

---@param width integer
---@param height integer
function SetWindowSize(width, height) end

function ToggleFullscreen() end

---@return boolean
function IsWindowFocused() end

---@return boolean
function IsWindowResized() end

---@return boolean
function IsWindowMinimized() end

---@return boolean
function IsWindowMaximized() end

---@return integer
function GetCurrentMonitor() end

---@param monitor integer
---@return integer
function GetMonitorWidth(monitor) end

---@param monitor integer
---@return integer
function GetMonitorHeight(monitor) end

---@param x integer
---@param y integer
function SetWindowPosition(x, y) end

---@return Vector2
function GetWindowPosition() end

---@param width integer
---@param height integer
function SetWindowMinSize(width, height) end

---@param width integer
---@param height integer
function SetWindowMaxSize(width, height) end

---@param opacity number
function SetWindowOpacity(opacity) end

---@return Vector2
function GetWindowScaleDPI() end

---@param monitor integer
---@return string?
function GetMonitorName(monitor) end

---@return integer
function GetMonitorCount() end

---@param monitor integer
---@return Vector2
function GetMonitorPosition(monitor) end

---@param monitor integer
---@return integer
function GetMonitorRefreshRate(monitor) end

---@param fileName string
function TakeScreenshot(fileName) end

---@return integer
function GetFPS() end

---@param seed integer
function SetRandomSeed(seed) end

---@param min integer
---@param max integer
---@return integer
function GetRandomValue(min, max) end

---@param fileName string
---@return boolean
function FileExists(fileName) end

---@param dirPath string
---@return boolean
function DirectoryExists(dirPath) end

---@param fileName string
---@return string?
function LoadFileText(fileName) end

---@param fileName string
---@param text string
---@return boolean
function SaveFileText(fileName, text) end

---@return integer
function GetScreenWidth() end

---@return integer
function GetScreenHeight() end

---@return number
function GetFrameTime() end

---@param width number
---@param height number
---@param title string
function InitWindow(width, height, title) end

function InitAudioDevice() end

---@param fps number
function SetTargetFPS(fps) end

---@return boolean
function WindowShouldClose() end

function BeginDrawing() end

function EndDrawing() end

function CloseWindow() end

function CloseAudioDevice() end
