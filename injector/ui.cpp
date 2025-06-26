#include "ui.h"

#include "interface.h"
#include "csgo_icon.hpp"

void gui::base_t::init(IDirect3DDevice9* device, ui_rect_t rect)
{
	m_rect = rect;
	g_ui.csgo_icon_sprite->init(device, csgo_ico, sizeof(csgo_ico), 24, 24);
}

void gui::base_t::begin_frame()
{
	g_ui.csgo_icon_sprite->begin();
}

void gui::base_t::pop_cursor_pos(ui_vec_t group_pos)
{
	m_erect.m_start_pos.x = group_pos.x;
	m_erect.m_start_pos.y = group_pos.y;
}

void gui::base_t::push_log(const std::string& msg)
{
	m_logs.push_back({ msg, std::chrono::steady_clock::now() });
}

void gui::base_t::on_reset()
{
	g_ui.csgo_icon_sprite->on_reset();
}

void gui::base_t::on_reset_end()
{
	g_ui.csgo_icon_sprite->on_reset_end();
}

void gui::base_t::end_frame()
{
	if (!m_logs.empty()) {
		auto now = std::chrono::steady_clock::now();
		auto& n = m_logs.front();

		g_ui.get->text_box(n.m_msg, { 10, g_interface.get_height() - 32 });

		auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - n.m_start_time).count();

		if (duration >= 2)
			m_logs.erase(m_logs.begin());
	}

	g_ui.csgo_icon_sprite->end();
}

bool gui::base_t::is_hovered(const ui_cursor_rect_t& rect)
{
	auto mouse_pos_x = g_interface.get_mouse_pos_x();
	auto mouse_pos_y = g_interface.get_mouse_pos_y();

	return (mouse_pos_x >= rect.m_first.x && mouse_pos_x <= rect.m_first.x + rect.m_second.x
		&& mouse_pos_y >= rect.m_first.y && mouse_pos_y <= rect.m_first.y + rect.m_second.y);
}