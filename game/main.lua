local screen_width = 800
local screen_height = 450

local virtual_screen_width = 160
local virtual_screen_height = 90

local virtual_ratio = screen_width / virtual_screen_width

InitWindow(screen_width, screen_height, "raylib [core] example - smooth pixel perfect")

local world_space_camera = Camera2D(Vector2(0, 0), Vector2(0, 0))
world_space_camera.zoom = 1

local screen_space_camera = Camera2D(Vector2(0, 0), Vector2(0, 0))
screen_space_camera.zoom = 1

local target = LoadRenderTexture(virtual_screen_width, virtual_screen_height)

-- vsFileName is nil so LoadShader falls back to raylib's default vertex
-- shader -- tint.fs only overrides the fragment stage.
local tint_shader = LoadShader(nil, "assets/shaders/tint.fs")
local tint_shader_time_loc = GetShaderLocation(tint_shader, "time")
local shader_on = false

local rec01 = Rectangle(70, 35, 20, 20)
local rec02 = Rectangle(90, 55, 30, 10)
local rec03 = Rectangle(80, 65, 15, 25)

local source_rec = Rectangle(0, 0, target.texture.width, -target.texture.height)
local dest_rec = Rectangle(
	(screen_width - screen_width / 1.25) / 2,
	(screen_height - screen_height / 1.25) / 2,
	screen_width / 1.25,
	screen_height / 1.25
)

local origin = Vector2(0, 0)

local rotation = 0

local camera_x = 0
local camera_y = 0

local smooth_on = true
local overscan = false

-- raylib's C truncf() rounds toward zero (not toward -infinity like
-- math.floor). This matters here because camera_x/camera_y swing negative,
-- and using floor instead of trunc shifts where the world camera's integer
-- part snaps relative to the fractional remainder handed to the screen
-- space camera.
local function trunc(x)
	if x < 0 then
		return math.ceil(x)
	end
	return math.floor(x)
end

SetTargetFPS(90)

while not WindowShouldClose() do
	-- update
	rotation = rotation + 60 * GetFrameTime()

	camera_x = math.sin(GetTime()) * 50 - 10
	camera_y = math.cos(GetTime()) * 30

	screen_space_camera.target = Vector2(camera_x, camera_y)

	world_space_camera.target.x = trunc(screen_space_camera.target.x)
	screen_space_camera.target.x = screen_space_camera.target.x - world_space_camera.target.x
	screen_space_camera.target.x = screen_space_camera.target.x * virtual_ratio

	world_space_camera.target.y = trunc(screen_space_camera.target.y)
	screen_space_camera.target.y = screen_space_camera.target.y - world_space_camera.target.y
	screen_space_camera.target.y = screen_space_camera.target.y * virtual_ratio

	if IsKeyPressed(KEY_S) then
		smooth_on = not smooth_on
	end

	if IsKeyPressed(KEY_O) then
		overscan = not overscan
	end

	if IsKeyPressed(KEY_T) then
		shader_on = not shader_on
	end

	if shader_on then
		SetShaderValueFloat(tint_shader, tint_shader_time_loc, GetTime())
	end

	if overscan then
		dest_rec = Rectangle(
			-virtual_ratio,
			-virtual_ratio,
			screen_width + (virtual_ratio * 2),
			screen_height + (virtual_ratio * 2)
		)
	else
		dest_rec = Rectangle(
			(screen_width - screen_width / 1.25) / 2,
			(screen_height - screen_height / 1.25) / 2,
			screen_width / 1.25,
			screen_height / 1.25
		)
	end

	-- draw

	BeginTextureMode(target)
	ClearBackground(RAYWHITE)
	BeginMode2D(world_space_camera)
	DrawCircleV(Vector2(0, 0), 20, RED)
	for i = 1, 10 do
		DrawLineEx(Vector2(i * 20, 0), Vector2(i * 20, 200), 1, LIGHTGRAY)
	end
	for i = 1, 10 do
		DrawLineEx(Vector2(0, i * 20), Vector2(200, i * 20), 1, LIGHTGRAY)
	end
	DrawRectanglePro(rec01, origin, rotation, BLACK)
	DrawRectanglePro(rec02, origin, -rotation, RED)
	DrawRectanglePro(rec03, origin, rotation + 45, BLUE)
	EndMode2D()
	EndTextureMode()

	BeginDrawing()
	ClearBackground(LIGHTGRAY)

	if smooth_on then
		BeginMode2D(screen_space_camera)
	end

	if shader_on then
		BeginShaderMode(tint_shader)
	end

	DrawTexturePro(target.texture, source_rec, dest_rec, origin, 0, WHITE)

	if shader_on then
		EndShaderMode()
	end

	if smooth_on then
		EndMode2D()
	end

	DrawText(string.format("Screen resolution: %dx%d", screen_width, screen_height), 10, 10, 20, DARKBLUE)
	DrawText(
		string.format("World resolution: %dx%d", virtual_screen_width, virtual_screen_height),
		10,
		40,
		20,
		DARKGREEN
	)
	DrawText("Smooth: " .. tostring(smooth_on), 10, screen_height - 90, 20, RED)
	DrawText("Overscan: " .. tostring(overscan), 10, screen_height - 60, 20, RED)
	DrawText("Shader (T): " .. tostring(shader_on), 10, screen_height - 30, 20, RED)
	DrawFPS(GetScreenWidth() - 95, 10)

	EndDrawing()
end

UnloadShader(tint_shader)
UnloadRenderTexture(target)

CloseWindow()
