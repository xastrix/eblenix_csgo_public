#include "ui.h"

void gui::base_t::util_box(ui_vec_t pos, std::vector<ui_util_t> ulist)
{
	static bool box_opened{};
	static int sx, sy;

	// huh?
	if (ulist.empty())
		return;

	if ((GetAsyncKeyState(VK_RBUTTON) & 1) && !box_opened) {
		sx = pos.x;
		sy = pos.y;
		box_opened = true;
	}

	if (box_opened)
	{
		for (int i = 0; i < ulist.size(); i++)
		{
			ui_cursor_rect_t frame_rect = {
				{ sx + 5, sy + (i * 18) },
			    { 70, 18 }
			};

			auto hovered = is_hovered(frame_rect);

			g_d3d.draw_rect(frame_rect.m_first.x, frame_rect.m_first.y,
				frame_rect.m_second.x, frame_rect.m_second.y, D3DCOLOR_RGBA(66, 84, 114, 255));

			g_d3d.draw_filled_rect(frame_rect.m_first.x + 1, frame_rect.m_first.y + 1,
				frame_rect.m_second.x - 1, frame_rect.m_second.y - 1,
				hovered ? D3DCOLOR_RGBA(68, 99, 153, 255) : D3DCOLOR_RGBA(54, 72, 102, 255));

			g_d3d.draw_string(ulist[i].m_name, frame_rect.m_first.x + 5,
				frame_rect.m_first.y + 3, g_d3d.get_font(Tahoma12px), D3DCOLOR_RGBA(255, 255, 255, 255));

			if (hovered && (GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
				ulist[i].m_fn();
				box_opened = false;
			}
			else if (!hovered) {
				if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
					box_opened = false;
			}
		}
	}
}