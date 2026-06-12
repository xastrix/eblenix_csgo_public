-- A simple example of using the in-game renderer (surface)

font = surface.font_create()
surface.set_font_glyph(font, "Verdana", 12, 0, 0, 0, surface_text_flags.underline)

register_callback(cb.on_paint_traverse, function()
	surface.set_draw_color(255, 60, 60, 255)
	surface.draw_outlined_rect(10, 26, 30, 30)

	surface.set_draw_color(255, 60, 60, 255)
	surface.draw_filled_rect(45, 26, 30, 30)

	surface.set_draw_color(255, 60, 60, 255)
	surface.draw_line(95, 25, 110, 40)
	surface.draw_line(110, 40, 95, 55)
	surface.draw_line(95, 55, 80, 40)
	surface.draw_line(80, 40, 95, 25)

	surface.set_text_color(255, 255, 255, 255)
	surface.draw_text_font(font)
	surface.draw_text_pos(10, 10)
	surface.draw_render_text("This is simple text!")
end)