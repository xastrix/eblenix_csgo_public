#include "ui_render.h"

void ui_render::draw(int x, int y)
{
	int HeadBoxWidth = 178;
	int HeadBoxHeight = 22;

	int MenuBoxX = x + 1;
	int MenuTextX = MenuBoxX + 6;

	int MenuBoxY = 0;
	int MenuTextY = 0;

	int HeadY = y;

	y += HeadBoxHeight + 4;

	int MenuLogoX = x + 1;
	int MenuLogoY = y - get_sprite(UI_SPRITE_LOGO).get_height() + 1;

	get_sprite(UI_SPRITE_LOGO).draw(MenuLogoX, MenuLogoY);

	for (int i = 0; i < m_entry_size; i++)
	{
		MenuBoxY = y + 1;
		MenuTextY = MenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		g_render.draw_filled_rect(MenuBoxX, MenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

		if (i == m_entry_position)
		{
			if (!s_opened[UI_SUB_POS])
				g_render.draw_filled_rect(MenuBoxX, MenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);

			g_render.draw_filled_rect(MenuBoxX, MenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
		}

		g_render.draw_stringW(menu_entry[i].m_name, MenuTextX + 3, MenuTextY - 11, g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

		g_render.draw_string("+", MenuTextX + HeadBoxWidth - 20, MenuTextY - 11, g_render.get_font(Tahoma12px),
			TEXT_OUTLINE, (i == m_entry_position && s_opened[UI_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);

		y += HeadBoxHeight + 1;

		if (menu_entry[i].m_space)
			y += HeadBoxHeight + 1;
	}

	x += HeadBoxWidth + 1;

	if (s_opened[UI_SUB_POS])
	{
		int SubMenuBoxX = x + 1;
		int SubMenuBoxY = HeadY + HeadBoxHeight + 5;
		int SubMenuTextX = SubMenuBoxX + 6;
		int SubMenuTextY = SubMenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		for (int i = 0; i < s_entry_size; i++)
		{
			g_render.draw_filled_rect(SubMenuBoxX, SubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

			if (i == s_entry_position)
			{
				if (!s_opened[UI_SUB_SUB_POS])
					g_render.draw_filled_rect(SubMenuBoxX, SubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);

				g_render.draw_filled_rect(SubMenuBoxX, SubMenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
			}

			g_render.draw_stringW(subm_entry[i].m_name, SubMenuTextX + 3, SubMenuTextY - 11,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (subm_entry[i].m_state == UI_TAB_STATE)
			{
				g_render.draw_string("+", SubMenuTextX + HeadBoxWidth - 20, SubMenuTextY - 11, g_render.get_font(Tahoma12px),
					TEXT_OUTLINE, (i == s_entry_position && s_opened[UI_SUB_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else
			{
				if (subm_entry[i].m_state != UI_FUNCTION_STATE)
				{
					if (subm_entry[i].m_state == UI_BOOL_STATE)
					{
						draw_bool(g_vars.get_as<bool>(subm_entry[i].m_var).value(),
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_PRIMARY_COL]);
					}
					else if (subm_entry[i].m_state == UI_INT_STATE)
					{
						draw_int(g_vars.get_as<int>(subm_entry[i].m_var).value(),
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (subm_entry[i].m_state == UI_FLOAT_STATE)
					{
						draw_float(g_vars.get_as<float>(subm_entry[i].m_var).value(),
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (subm_entry[i].m_state == UI_KEY_STATE)
					{
						draw_hotkey(g_vars.get_as<int>(subm_entry[i].m_var).value(),
							SubMenuBoxX + HeadBoxWidth, SubMenuTextX + HeadBoxWidth, SubMenuBoxY,
							SubMenuTextY, HeadBoxHeight, subm_entry[i].m_key_hold, m_colors[UI_TEXT_COL]);
					}
					else if (subm_entry[i].m_state == UI_ITEM_STATE)
					{
						draw_item(subm_entry[i].m_items[g_vars.get_as<int>(subm_entry[i].m_var).value()],
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_TEXT_COL]);
					}
				}
			}

			SubMenuBoxY += HeadBoxHeight + 1;
			SubMenuTextY += HeadBoxHeight + 1;

			if (subm_entry[i].m_space)
			{
				SubMenuBoxY += HeadBoxHeight + 1;
				SubMenuTextY += HeadBoxHeight + 1;
			}
		}
	}

	x += HeadBoxWidth + 1;

	if (s_opened[UI_SUB_SUB_POS])
	{
		int SubSubMenuBoxX = x + 1;
		int SubSubMenuBoxY = HeadY + HeadBoxHeight + 5;
		int SubSubMenuTextX = SubSubMenuBoxX + 6;
		int SubSubMenuTextY = SubSubMenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		for (int i = 0; i < ss_entry_size; i++)
		{
			g_render.draw_filled_rect(SubSubMenuBoxX, SubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

			if (i == ss_entry_position)
			{
				if (!s_opened[UI_SUB_SUB_SUB_POS])
					g_render.draw_filled_rect(SubSubMenuBoxX, SubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);

				g_render.draw_filled_rect(SubSubMenuBoxX, SubSubMenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
			}

			g_render.draw_stringW(ssubm_entry[i].m_name, SubSubMenuTextX + 3, SubSubMenuTextY - 11, g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (ssubm_entry[i].m_state == UI_TAB_STATE)
			{
				g_render.draw_string("+", SubSubMenuTextX + HeadBoxWidth - 20, SubSubMenuTextY - 11, g_render.get_font(Tahoma12px),
					TEXT_OUTLINE, (i == ss_entry_position && s_opened[UI_SUB_SUB_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else
			{
				if (ssubm_entry[i].m_state != UI_FUNCTION_STATE)
				{
					if (ssubm_entry[i].m_state == UI_BOOL_STATE)
					{
						draw_bool(g_vars.get_as<bool>(ssubm_entry[i].m_var).value(),
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_PRIMARY_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_INT_STATE)
					{
						draw_int(g_vars.get_as<int>(ssubm_entry[i].m_var).value(),
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_FLOAT_STATE)
					{
						draw_float(g_vars.get_as<float>(ssubm_entry[i].m_var).value(),
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_KEY_STATE)
					{
						draw_hotkey(g_vars.get_as<int>(ssubm_entry[i].m_var).value(),
							SubSubMenuBoxX + HeadBoxWidth, SubSubMenuTextX + HeadBoxWidth, SubSubMenuBoxY,
							SubSubMenuTextY, HeadBoxHeight, ssubm_entry[i].m_key_hold, m_colors[UI_TEXT_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_ITEM_STATE)
					{
						draw_item(ssubm_entry[i].m_items[g_vars.get_as<int>(ssubm_entry[i].m_var).value()],
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_TEXT_COL]);
					}
				}
			}

			SubSubMenuBoxY += HeadBoxHeight + 1;
			SubSubMenuTextY += HeadBoxHeight + 1;

			if (ssubm_entry[i].m_space)
			{
				SubSubMenuBoxY += HeadBoxHeight + 1;
				SubSubMenuTextY += HeadBoxHeight + 1;
			}
		}
	}

	x += HeadBoxWidth + 1;

	if (s_opened[UI_SUB_SUB_SUB_POS])
	{
		int SubSubSubMenuBoxX = x + 1;
		int SubSubSubMenuBoxY = HeadY + HeadBoxHeight + 5;
		int SubSubSubMenuTextX = SubSubSubMenuBoxX + 6;
		int SubSubSubMenuTextY = SubSubSubMenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		for (int i = 0; i < sss_entry_size; i++)
		{
			g_render.draw_filled_rect(SubSubSubMenuBoxX, SubSubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

			if (i == sss_entry_position)
			{
				g_render.draw_filled_rect(SubSubSubMenuBoxX, SubSubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);
				g_render.draw_filled_rect(SubSubSubMenuBoxX, SubSubSubMenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
			}

			g_render.draw_stringW(sssubm_entry[i].m_name, SubSubSubMenuTextX + 3, SubSubSubMenuTextY - 11, g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (sssubm_entry[i].m_state != UI_FUNCTION_STATE)
			{
				if (sssubm_entry[i].m_state == UI_BOOL_STATE)
				{
					draw_bool(g_vars.get_as<bool>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_PRIMARY_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_INT_STATE)
				{
					draw_int(g_vars.get_as<int>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_TEXT_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_FLOAT_STATE)
				{
					draw_float(g_vars.get_as<float>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_TEXT_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_KEY_STATE)
				{
					draw_hotkey(g_vars.get_as<int>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuBoxX + HeadBoxWidth, SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuBoxY,
						SubSubSubMenuTextY, HeadBoxHeight, sssubm_entry[i].m_key_hold, m_colors[UI_TEXT_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_ITEM_STATE)
				{
					draw_item(sssubm_entry[i].m_items[g_vars.get_as<int>(sssubm_entry[i].m_var).value()],
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_TEXT_COL]);
				}
			}

			SubSubSubMenuBoxY += HeadBoxHeight + 1;
			SubSubSubMenuTextY += HeadBoxHeight + 1;

			if (sssubm_entry[i].m_space)
			{
				SubSubSubMenuBoxY += HeadBoxHeight + 1;
				SubSubSubMenuTextY += HeadBoxHeight + 1;
			}
		}
	}
}

color_t& ui_render::get_color(const _ui_colors index)
{
	return m_colors[index];
}

void ui_render::draw_float(float value, int x, int y, color_t color)
{
	char ss[256];
	sprintf_s(ss, "%.2f", value);

	const auto font = g_render.get_font(Tahoma12px);
	const auto string_width = g_render.get_text_width(ss, font);

	g_render.draw_string(ss, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
}

void ui_render::draw_bool(bool value, int x, int y, color_t color)
{
	g_render.draw_filled_rect(x - 38, y - 11, 26, 14, color_t(40, 40, 40));

	value ?
		g_render.draw_filled_rect(x - 25,
			y - 10, 12, 12, color) :

		g_render.draw_filled_rect(x - 37,
			y - 10, 12, 12, color_t(68, 68, 70));
}

void ui_render::draw_int(int value, int x, int y, color_t color)
{
	const auto font = g_render.get_font(Tahoma12px);
	const auto string_width = g_render.get_text_width(std::to_string(value), font);

	g_render.draw_string(std::to_string(value), x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
}

void ui_render::draw_hotkey(int k, int x, int x_text, int y, int y_text, int h, bool hold, color_t color)
{
	const auto kss = g_input.virtual_key_to_wstring(k);

	const auto font = g_render.get_font(Tahoma12px);
	const auto key_string_width = g_render.get_text_widthW(hold ? L"---" : kss, font);

	g_render.draw_filled_rect(x - key_string_width - 47, y, 47 + key_string_width, h, m_colors[UI_MAIN_COL]);

	g_render.draw_stringW(hold ? L"---" : kss, x_text - key_string_width - 30,
		y_text - 11, font, TEXT_OUTLINE, color);
}

void ui_render::draw_item(std::wstring item, int x, int y, color_t color)
{
	if (item.length() > 20)
		item = item.substr(0, 20) + L"...";

	const auto font = g_render.get_font(Tahoma12px);
	const auto string_width = g_render.get_text_widthW(item, font);

	g_render.draw_stringW(item, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
}