#include "ui.h"

void gui::base_t::group_box(const std::string& field, ui_vec_t pos, ui_vec_t size)
{
	g_d3d.draw_filled_rect(pos.x + 1, pos.y + 1, size.x - 1, size.y - 1, D3DCOLOR_RGBA(54, 72, 102, 255));

	const auto font = g_d3d.get_font(Tahoma12px);
	const auto text_width = g_d3d.get_text_width(field, font);

	g_d3d.draw_line(pos.x, pos.y, pos.x + 7, pos.y, D3DCOLOR_RGBA(66, 84, 114, 255));
	g_d3d.draw_line(pos.x + text_width + 12, pos.y, pos.x + size.x, pos.y, D3DCOLOR_RGBA(66, 84, 114, 255));
	g_d3d.draw_line(pos.x, pos.y, pos.x, pos.y + size.y, D3DCOLOR_RGBA(66, 84, 114, 255));
	g_d3d.draw_line(pos.x + size.x, pos.y, pos.x + size.x, pos.y + size.y, D3DCOLOR_RGBA(66, 84, 114, 255));
	g_d3d.draw_line(pos.x, pos.y + size.y, pos.x + size.x, pos.y + size.y, D3DCOLOR_RGBA(66, 84, 114, 255));

	g_d3d.draw_string(field, pos.x + 10, pos.y - 7, font, D3DCOLOR_RGBA(255, 255, 255, 255));

	m_erect.m_start_pos.x = pos.x + 5;
	m_erect.m_start_pos.y = pos.y + 7;

	m_erect.m_end_pos.x = size.x;
	m_erect.m_end_pos.y = size.y;
}

void gui::base_t::text_box(const std::string& field, ui_vec_t pos)
{
	const auto font = g_d3d.get_font(Tahoma12px);
	// const auto text_width = g_d3d.get_text_width(field, font);

	// g_d3d.draw_filled_rect(pos.x, pos.y, text_width + 11, 18, D3DCOLOR_RGBA(68, 90, 129, 255));
	// g_d3d.draw_rect(pos.x, pos.y, text_width + 11, 18, D3DCOLOR_RGBA(90, 121, 175, 255));

	g_d3d.draw_string(field, pos.x + 7, pos.y + 2, font, D3DCOLOR_RGBA(255, 255, 255, 255));
}