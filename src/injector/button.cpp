#include "ui.h"

void c_button::think()
{
	c_vec2 draw_position = m_parent->get_child_draw_position() + m_pos + c_vec2(16, 0);
	c_vec2 button_size(m_parent->get_size().x - 65, 24);

	c_vec2 item_min(draw_position);
	c_vec2 item_max(item_min + c_vec2(button_size.x, button_size.y));

	if (g_ui.is_hovered(item_min, item_max) && g_ui.is_key_pressed(VK_LBUTTON) && !m_holding) {
		m_holding = true;
	}

	else if (g_ui.is_hovered(item_min, item_max) && g_ui.is_key_released(VK_LBUTTON) && m_holding) {
		m_callback();
		m_holding = false;
	}

	else if (!g_ui.is_hovered(item_min, item_max) && g_ui.is_key_released(VK_LBUTTON) && m_holding) {
		m_holding = false;
	}
}

void c_button::draw()
{
	c_vec2 draw_position = m_parent->get_child_draw_position() + m_pos + c_vec2(16, 0);
	c_vec2 button_size(m_parent->get_size().x - 65, 24);

	c_vec2 item_min(draw_position);
	c_vec2 item_max(item_min + c_vec2(button_size.x, button_size.y));

	g_renderer.rect_cornered(item_min.x - 1, item_min.y - 1, button_size.x + 2, button_size.y + 2, 5.0f, c_color(10, 10, 10));
	g_renderer.rect_cornered(item_min.x, item_min.y, button_size.x, button_size.y, 5.0f, c_color(50, 50, 50));

	if (m_holding) {
		g_renderer.rect_cornered(item_min.x + 1, item_min.y + 1,
			button_size.x - 2, button_size.y - 2, 5.0f, c_color(10, 10, 10));
	}
	else {
		if (g_ui.is_hovered(item_min, item_max))
			g_renderer.rect_cornered(item_min.x + 1, item_min.y + 1,
				button_size.x - 2, button_size.y - 2, 5.0f, c_color(10, 10, 10));
	}

	const auto text_font = g_font[Verdana12px];

	c_vec2 text_sz = c_vec2(g_font.get_text_width(m_label, text_font),
		g_font.get_text_height(m_label, text_font));

	c_vec2 label_min(
		draw_position.x + button_size.x / 2 - text_sz.x / 2,
		draw_position.y + button_size.y / 2 - text_sz.y / 2
	);

	g_font.draw_string(m_label, label_min.x, label_min.y, text_font, TEXT_OUTLINE, c_color(200, 200, 200));
}