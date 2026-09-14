local screen_width = 1280
local screen_height = 720

local gravity = 980

local position = Vector2(screen_width / 2, screen_height / 2)
local radius = 10
local velocity = Vector2(500, 500)

local rotation = 0
local spin_speed = 50

InitWindow(screen_width, screen_height, "RAYLUA")
InitAudioDevice()

while not WindowShouldClose() do
	BeginDrawing()
	ClearBackground(Color(0x18, 0x18, 0x18))

	velocity.y = velocity.y + gravity * GetFrameTime()
	position = position + velocity * GetFrameTime()

	if position.x + radius >= screen_width then
		velocity.x = -velocity.x
		position.x = screen_width - radius - 1
	end
	if position.x - radius <= 0 then
		velocity.x = -velocity.x
		position.x = radius + 1
	end
	if position.y + radius >= screen_height then
		velocity.y = -velocity.y
		position.y = screen_height - radius - 1
	end
	if position.y - radius <= 0 then
		velocity.y = -velocity.y
		position.y = radius + 1
	end

	DrawCircleV(position, radius, SKYBLUE)

	rotation = rotation + spin_speed * GetFrameTime()

	local font_size = MeasureTextEx(GetFontDefault(), "Go Fuck Yourself", 64, 8)

	DrawTextPro(
		GetFontDefault(),
		"Go Fuck Yourself",
		Vector2(screen_width / 2, screen_height / 2),
		font_size * 0.5,
		rotation,
		64,
		8,
		WHITE
	)

	EndDrawing()
end

CloseAudioDevice()
CloseWindow()
