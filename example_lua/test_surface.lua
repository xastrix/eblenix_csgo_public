-- A simple example of using it inside an in-game renderer (surface) by drawing a filled rect

register_callback(cb.on_paint_traverse, function()
	surface.set_draw_color(255, 255, 255, 255)
	surface.draw_filled_rect(10, 10, 30, 30)
end)