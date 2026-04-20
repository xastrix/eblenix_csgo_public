#include "ui.h"

std::unordered_map<std::string, bool> g_Selected{};
void ui::ui_base_t::game_selector(const std::string& field, sprite_t* icon, int *selected, int id)
{
	const auto w = m_erect.m_end_pos.x - 1;
	const auto h = 30;

	ui_cursor_rect_t button_rect = {
		{ m_erect.m_start_pos.x, m_erect.m_start_pos.y },
		{ w, h }
	};

	if (is_hovered(button_rect) && !is_blocked()) {
		g_d3d.draw_filled_rect(button_rect.m_first.x, button_rect.m_first.y,
			button_rect.m_second.x, button_rect.m_second.y, D3DCOLOR_RGBA(68, 99, 153, 255));

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			g_Selected[field] = true;
			g_d3d.draw_filled_rect(button_rect.m_first.x, button_rect.m_first.y,
				button_rect.m_second.x, button_rect.m_second.y, D3DCOLOR_RGBA(69, 107, 173, 255));
		}
		else {
			if (g_Selected[field]) {
				*selected = !(*selected == id) ? id : -1;
				g_Selected[field] = false;
			}
		}
	}
	else {
		if (g_Selected[field])
			g_Selected[field] = false;

		if (*selected == id)
			g_d3d.draw_filled_rect(button_rect.m_first.x, button_rect.m_first.y,
				button_rect.m_second.x, button_rect.m_second.y, D3DCOLOR_RGBA(68, 90, 129, 255));
	}

	icon->draw(button_rect.m_first.x + 4, button_rect.m_first.y + 3);

	g_d3d.draw_rect(button_rect.m_first.x + 3, button_rect.m_first.y + 2,
		icon->get_width() + 1, icon->get_height() + 1, D3DCOLOR_RGBA(66, 84, 114, 255));

	// if (field.length() > 28)
	// 	field = field.substr(0, 28) + "..";

	g_d3d.draw_string(field, button_rect.m_first.x + 35, button_rect.m_first.y + 3, g_d3d.get_font(Tahoma12px), D3DCOLOR_RGBA(255, 255, 255, 255));

	m_erect.m_start_pos.y += (h + 1);
}