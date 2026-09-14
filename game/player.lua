-- Player: position/velocity state, sprite, and simple mouse-follow physics.
--
-- Player.new() loads the player's texture, so it must only be called after
-- InitWindow() has created the GL context textures need. main.lua is
-- responsible for that ordering -- this module just defines the class.

local Player = {}
Player.__index = Player

local SPEED    = 100
local ACCEL    = 30
local MAX_VEL  = 40
local GRAVITY  = 9

function Player.new(x, y)
	local self = setmetatable({}, Player)

	self.pos = Vector2(x, y)
	self.vel = Vector2(0, 0)
	self.sprite = LoadTexture("assets/netskie.png")

	return self
end

function Player:update(dt)
	if IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
		local mouse = GetMousePosition()
		local toMouse = (mouse - self.pos):normalize()

		self.vel = self.vel + toMouse * ACCEL * dt
		if self.vel:length() > MAX_VEL then
			self.vel = self.vel:normalize() * MAX_VEL
		end
	end

	self.vel.y = self.vel.y + GRAVITY * dt
	self.pos = self.pos + self.vel * SPEED * dt
end

function Player:draw()
	if IsMouseButtonDown(MOUSE_BUTTON_LEFT) then
		local mouse = GetMousePosition()
		DrawCircleV(mouse, 10, YELLOW)
		DrawLine(mouse.x, mouse.y, self.pos.x, self.pos.y, BLUE)
	end

	DrawTextureV(
		self.sprite,
		Vector2(self.pos.x - self.sprite.width / 2, self.pos.y - self.sprite.height / 2),
		WHITE
	)
end

function Player:unload()
	UnloadTexture(self.sprite)
end

return Player
