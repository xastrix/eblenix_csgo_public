#include "ui.h"

std::unordered_map<std::string, bool> g_Buttons{};
void ui::ui_base_t::button(const std::string& field, int w, int h, std::function<void()> fn)
{
	const auto x = m_erect.m_start_pos.x + 5;
	const auto y = m_erect.m_start_pos.y + 5;

	ui_cursor_rect_t button_rect = {
		{ x, y }, { w, h }
	};

	if (is_hovered(button_rect) && !is_blocked()) {
		g_d3d.draw_filled_rect(x, y, w, h, D3DCOLOR_RGBA(68, 99, 153, 255));
		g_d3d.draw_rect(x, y, w, h, D3DCOLOR_RGBA(58, 87, 137, 255));

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			g_Buttons[field] = true;
			g_d3d.draw_filled_rect(x + 1, y + 1, w - 1, h - 1, D3DCOLOR_RGBA(69, 107, 173, 255));
		}
		else {
			if (g_Buttons[field]) {
				fn();
				g_Buttons[field] = false;
			}
		}
	}
	else {
		if (g_Buttons[field])
			g_Buttons[field] = false;

		g_d3d.draw_filled_rect(x, y, w, h, D3DCOLOR_RGBA(68, 90, 129, 255));
	}

	const auto font = g_d3d.get_font(VerdanaBold12px);
	const auto text_width = g_d3d.get_text_width(field, font);
	const auto text_height = g_d3d.get_text_height(field, font);

	g_d3d.draw_string(field, x + w / 2 - text_width / 2,
		y + (h / 2) - (text_height / 2), font, D3DCOLOR_RGBA(233, 233, 233, 255));

	m_erect.m_start_pos.y += (h + 8);
}