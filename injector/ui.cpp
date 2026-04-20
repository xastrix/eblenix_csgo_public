#include "ui.h"

#include "interface.h"
#include "csgo_icon.hpp"

void ui::ui_base_t::init(IDirect3DDevice9* device, ui_rect_t rect)
{
	m_rect = rect;
	ui::csgo_ico->init(device, csgo_icon, sizeof(csgo_icon), 24, 24);
}

void ui::ui_base_t::begin_frame()
{
	ui::csgo_ico->begin();

	g_d3d.draw_filled_rect(0, 0, m_rect.m_size.x, m_rect.m_size.y,
		D3DCOLOR_RGBA(68, 90, 129, 255));

	g_d3d.draw_rect(0, 0, m_rect.m_size.x - 1, m_rect.m_size.y - 1,
		D3DCOLOR_RGBA(66, 84, 114, 255));
}

void ui::ui_base_t::set_cursor_pos(ui_vec_t pos)
{
	m_erect.m_start_pos.x = pos.x;
	m_erect.m_start_pos.y = pos.y;
}

void ui::ui_base_t::push_log(const std::string& msg)
{
	m_logs.push_back({ msg, std::chrono::steady_clock::now() });
}

void ui::ui_base_t::on_reset()
{
	ui::csgo_ico->on_reset();
}

void ui::ui_base_t::on_reset_end()
{
	ui::csgo_ico->on_reset_end();
}

void ui::ui_base_t::end_frame()
{
	if (!m_logs.empty()) {
		auto now = std::chrono::steady_clock::now();
		auto& n = m_logs.front();

		ui::get->text_box(n.m_msg, { 10, g_interface.get_height() - 32 });

		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - n.m_start_time).count();

		if (duration >= 2)
			m_logs.erase(m_logs.begin());
	}

	ui::csgo_ico->end();
}

bool ui::ui_base_t::is_hovered(const ui_cursor_rect_t& rect)
{
	auto mouse_pos_x = g_interface.get_mouse_pos_x();
	auto mouse_pos_y = g_interface.get_mouse_pos_y();

	return (mouse_pos_x >= rect.m_first.x && mouse_pos_x <= rect.m_first.x + rect.m_second.x
		&& mouse_pos_y >= rect.m_first.y && mouse_pos_y <= rect.m_first.y + rect.m_second.y);
}