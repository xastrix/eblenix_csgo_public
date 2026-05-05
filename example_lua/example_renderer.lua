-- Testing render functions - text, filled rectangles, circles, and sprites

local calibri_font = renderer.create_font("Calibri", 12, 600, 5)
local s = renderer.create_sprite("resources/example_image.png", 30, 30)

local white = color(255, 255, 255, 255)
local red = color(255, 0, 0, 255)
local green = color(0, 255, 0, 155)
local blue = color(0, 0, 255, 255)
local blue2 = color(0, 0, 255, 155)

register_callback(cb.on_present, function()
	renderer.text("Hello from example_renderer.lua!", 5, 5,
		calibri_font, text_flags.text_outline, white)

	renderer.rect(5, 20, 30, 30, red)
	renderer.rect_fill(40, 20, 30, 30, green)
	renderer.rect_fill_cornered(75, 20, 30, 30, 12.0, blue2)
	renderer.circle(123, 35, 13.0, white)
	renderer.circle_fill(153, 35, 13.0, white)
	renderer.gradient_horizontal(173, 20, 30, 30, red, blue)
	renderer.gradient_vertical(210, 20, 30, 30, red, blue)

	s:begin_draw()
	s:draw(245, 20, white)
end)

register_callback(cb.on_present_end, function()
	s:end_draw()
end)

register_callback(cb.on_reset, function()
	s:on_reset()
end)

register_callback(cb.on_reset_end, function()
	s:on_reset_end()
end)