local Player = require("player")

local BGColor = rl.Color(0x18, 0x18, 0x18)

rl.InitWindow(1280, 720, "Raylib Lua - (c) Omnicorum")
rl.InitAudioDevice()

rl.SetTargetFPS(60)

-- Asset loading (Player.new loads a texture) has to happen after
-- InitWindow, since that's what creates the GL context textures upload to.
local player = Player.new(0, 0)

while not rl.WindowShouldClose() do
	local dt = rl.GetFrameTime()

	player:update(dt)

	rl.BeginDrawing()

	rl.ClearBackground(BGColor)

	player:draw()

	rl.DrawFPS(10, 10)

	rl.EndDrawing()
end

player:unload()

rl.CloseAudioDevice()
rl.CloseWindow()
