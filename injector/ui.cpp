#include "ui.h"

ui g_ui;

void ui::begin()
{
	m_csgo_icon.begin();

	g_d3d.draw_filled_rect(0, 0, g_interface.get_width(),
		g_interface.get_height(), D3DCOLOR_RGBA(68, 90, 129, 255));
}

void ui::end()
{
	m_csgo_icon.end();
}