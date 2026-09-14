local screen_width = 800
local screen_height = 450

local virtual_screen_width = 160
local virtual_screen_height = 90

local virtual_ratio = screen_width / virtual_screen_width

rl.InitWindow(screen_width, screen_height, "raylib [core] example - smooth pixel perfect")

local world_space_camera = rl.Camera2D(rl.Vector2(0, 0), rl.Vector2(0, 0))
world_space_camera.zoom = 1

local screen_space_camera = rl.Camera2D(rl.Vector2(0, 0), rl.Vector2(0, 0))
screen_space_camera.zoom = 1

local target = rl.LoadRenderTexture(virtual_screen_width, virtual_screen_height)

local rec01 = rl.Rectangle(70, 35, 20, 20)
local rec02 = rl.Rectangle(90, 55, 30, 10)
local rec03 = rl.Rectangle(80, 65, 15, 25)

local source_rec = rl.Rectangle(0, 0, target.texture.width, -target.texture.height)
local dest_rec = rl.Rectangle(
	(screen_width - screen_width / 1.25) / 2,
	(screen_height - screen_height / 1.25) / 2,
	screen_width / 1.25,
	screen_height / 1.25
)

local origin = rl.Vector2(0, 0)

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

rl.SetTargetFPS(90)

while not rl.WindowShouldClose() do
	-- update
	rotation = rotation + 60 * rl.GetFrameTime()

	camera_x = math.sin(rl.GetTime()) * 50 - 10
	camera_y = math.cos(rl.GetTime()) * 30

	screen_space_camera.target = rl.Vector2(camera_x, camera_y)

	world_space_camera.target.x = trunc(screen_space_camera.target.x)
	screen_space_camera.target.x = screen_space_camera.target.x - world_space_camera.target.x
	screen_space_camera.target.x = screen_space_camera.target.x * virtual_ratio

	world_space_camera.target.y = trunc(screen_space_camera.target.y)
	screen_space_camera.target.y = screen_space_camera.target.y - world_space_camera.target.y
	screen_space_camera.target.y = screen_space_camera.target.y * virtual_ratio

	if rl.IsKeyPressed(KEY_S) then
		smooth_on = not smooth_on
	end

	if rl.IsKeyPressed(KEY_O) then
		overscan = not overscan
	end

	if overscan then
		dest_rec = rl.Rectangle(
			-virtual_ratio,
			-virtual_ratio,
			screen_width + (virtual_ratio * 2),
			screen_height + (virtual_ratio * 2)
		)
	else
		dest_rec = rl.Rectangle(
			(screen_width - screen_width / 1.25) / 2,
			(screen_height - screen_height / 1.25) / 2,
			screen_width / 1.25,
			screen_height / 1.25
		)
	end

	-- draw

	rl.BeginTextureMode(target)
	rl.ClearBackground(RAYWHITE)
	rl.BeginMode2D(world_space_camera)
	rl.DrawCircleV(rl.Vector2(0, 0), 20, RED)
	for i = 1, 10 do
		rl.DrawLineEx(rl.Vector2(i * 20, 0), rl.Vector2(i * 20, 200), 1, LIGHTGRAY)
	end
	for i = 1, 10 do
		rl.DrawLineEx(rl.Vector2(0, i * 20), rl.Vector2(200, i * 20), 1, LIGHTGRAY)
	end
	rl.DrawRectanglePro(rec01, origin, rotation, BLACK)
	rl.DrawRectanglePro(rec02, origin, -rotation, RED)
	rl.DrawRectanglePro(rec03, origin, rotation + 45, BLUE)
	rl.EndMode2D()
	rl.EndTextureMode()

	rl.BeginDrawing()
	rl.ClearBackground(LIGHTGRAY)

	if smooth_on then
		rl.BeginMode2D(screen_space_camera)
		rl.DrawTexturePro(target.texture, source_rec, dest_rec, origin, 0, WHITE)
		rl.EndMode2D()
	else
		rl.DrawTexturePro(target.texture, source_rec, dest_rec, origin, 0, WHITE)
	end

	rl.DrawText(string.format("Screen resolution: %dx%d", screen_width, screen_height), 10, 10, 20, DARKBLUE)
	rl.DrawText(
		string.format("World resolution: %dx%d", virtual_screen_width, virtual_screen_height),
		10,
		40,
		20,
		DARKGREEN
	)
	rl.DrawText("Smooth: " .. tostring(smooth_on), 10, screen_height - 60, 20, RED)
	rl.DrawText("Overscan: " .. tostring(overscan), 10, screen_height - 30, 20, RED)
	rl.DrawFPS(rl.GetScreenWidth() - 95, 10)

	rl.EndDrawing()
end

rl.UnloadRenderTexture(target)

rl.CloseWindow()
