local BGColor = rl.Color(0x18, 0x18, 0x18)

local pos = rl.Vector2(0, 0)
local vel = rl.Vector2(0, 0)

local speed = 100
local accel = 30
local maxVel = 40
local gravity = 9

rl.InitWindow(1280, 720, "Raylib Lua - (c) Omnicorum")
rl.InitAudioDevice()

rl.SetTargetFPS(60)

local sprite = rl.LoadTexture("assets/netskie.png")

while not rl.WindowShouldClose() do
	rl.BeginDrawing()

	rl.ClearBackground(BGColor)

	local mouse = rl.GetMousePosition()
	local toMouse = (mouse - pos):normalize()

	if rl.IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
		rl.DrawCircleV(mouse, 10, YELLOW)
		rl.DrawLine(mouse.x, mouse.y, pos.x, pos.y, BLUE)

		vel = vel + toMouse * accel * rl.GetFrameTime()
		if vel:length() > maxVel then
			vel = vel:normalize() * maxVel
		end
	end

	vel.y = vel.y + gravity * rl.GetFrameTime()
	pos = pos + vel * speed * rl.GetFrameTime()

	rl.DrawTextureV(sprite, rl.Vector2(pos.x - sprite.width / 2, pos.y - sprite.height / 2), WHITE)

	rl.DrawFPS(10, 10)

	rl.EndDrawing()
end

rl.CloseAudioDevice()
rl.CloseWindow()
