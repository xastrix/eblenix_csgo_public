#include "ui.h"

#include "input.h"
#include "interfaces.h"

void c_ui::draw(int x, int y)
{
	auto menu_box_x = x + 1;
	auto menu_text_x = menu_box_x + 6;

	auto menu_box_y = 0;
	auto menu_text_y = 0;

	auto head_y = y;
	auto head_box_width = 178;
	auto head_box_height = 22;

	y += head_box_height + 4;

	auto menu_logo_x = x + 1;
	auto menu_logo_y = y - m_sprites[UI_SPRITE_LOGO].get_height() + 1;

	auto draw_float = [](float value, int x, int y, c_color color) {
		char ss[256];
		sprintf_s(ss, "%.2f", value);

		const auto font = FONT(Tahoma12px);
		const auto string_width = g_font->get_text_width(ss, font);

		g_font->draw_string(ss, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
	};

	auto idx{ 0 };
	auto draw_bool = [&](bool value, int x, int y, c_color first, c_color second) {
		static std::vector<float> anim_x{};

		if (idx >= anim_x.size())
			anim_x.resize(idx + 1, value ? 25.0f : 37.0f);

		const float target_x = value ? 25.0f : 38.0f;
		const float speed = g_var->get_as<float>(V_UI_ANIM_CHECKBOX_SWITCH).value();

		float delta_time = std::clamp(g_cs->m_globals->frame_time, 0.0001f, 0.1f);

		float fps_factor = delta_time * 60.0f;
		float final_step = std::clamp(speed * fps_factor, 0.0f, 1.0f);

		anim_x[idx] += (target_x - anim_x[idx]) * final_step;
		anim_x[idx] = std::clamp(anim_x[idx], 25.0f, 37.0f);

		g_renderer->rect_fill(x - 38, y - 11, 26, 14, m_colors[UI_OUTLINE_COL]);
		g_renderer->gradient_h(x - static_cast<int>(anim_x[idx]), y - 10, 12, 12,
			value ? first : second, value ? second : first);

		++idx;
	};

	auto draw_int = [](int value, int x, int y, c_color color) {
		const auto font = FONT(Tahoma12px);
		const auto string_width = g_font->get_text_width(std::to_string(value), font);

		g_font->draw_string(std::to_string(value), x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
	};

	auto draw_hotkey = [](int k, int x, int x_text, int y, int y_text, int h, bool hold, c_color color, c_color background_color) {
		const auto kss = g_input->virtual_key_to_wstring(k);

		const auto font = FONT(Tahoma12px);
		const auto key_string_width = g_font->get_text_widthW(hold ? L"---" : kss, font);

		g_renderer->rect_fill(x - key_string_width - 47, y, 47 + key_string_width, h, background_color);

		g_font->draw_stringW(hold ? L"---" : kss, x_text - key_string_width - 30,
			y_text - 11, font, TEXT_OUTLINE, color);
	};

	auto draw_item = [](std::wstring item, int x, int y, c_color color) {
		if (item.length() > 20)
			item = item.substr(0, 20) + L"...";

		const auto font = FONT(Tahoma12px);
		const auto string_width = g_font->get_text_widthW(item, font);

		g_font->draw_stringW(item, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
	};

	m_sprites[UI_SPRITE_LOGO].draw(menu_logo_x, menu_logo_y, c_color(255, 255, 255,
		static_cast<int>(m_target_animation_progress * 255.0f)));

	for (int i = 0; i < m_entry_sz; i++)
	{
		menu_box_y = y + 1;
		menu_text_y = menu_box_y + int(head_box_height / 2.0f) + 4;

		g_renderer->rect_fill(menu_box_x, menu_box_y, head_box_width, head_box_height, m_colors[UI_SHADOW_COL]);

		if (i == m_entry_pos)
		{
			if (!m_sopened[UI_SUB_POS])
				g_renderer->rect_fill(menu_box_x, menu_box_y, head_box_width, head_box_height, m_colors[UI_PRIMARY_COL]);

			g_renderer->rect_fill(menu_box_x, menu_box_y, 2, head_box_height, m_colors[UI_MAIN_COL]);
		}

		g_font->draw_stringW(m_entry[i].name, menu_text_x + 3, menu_text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

		g_font->draw_string("+", menu_text_x + head_box_width - 20, menu_text_y - 11, FONT(Tahoma12px),
			TEXT_OUTLINE, (i == m_entry_pos && m_sopened[UI_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);

		y += head_box_height + 1;

		if (m_entry[i].space)
			y += head_box_height + 1;
	}

	x += head_box_width + 1;

	if (m_sopened[UI_SUB_POS])
	{
		auto sub_menu_box_x = x + 1;
		auto sub_menu_box_y = head_y + head_box_height + 5;
		auto sub_menu_text_x = sub_menu_box_x + 6;
		auto sub_menu_text_y = sub_menu_box_y + int(head_box_height / 2.0f) + 4;

		for (int i = 0; i < s_entry_sz[UI_SUB_POS]; i++)
		{
			g_renderer->rect_fill(sub_menu_box_x, sub_menu_box_y, head_box_width, head_box_height, m_colors[UI_SHADOW_COL]);

			if (i == s_entry_pos[UI_SUB_POS])
			{
				if (!m_sopened[UI_SUB_SUB_POS])
					g_renderer->rect_fill(sub_menu_box_x, sub_menu_box_y, head_box_width, head_box_height, m_colors[UI_PRIMARY_COL]);

				g_renderer->rect_fill(sub_menu_box_x, sub_menu_box_y, 2, head_box_height, m_colors[UI_MAIN_COL]);
			}

			g_font->draw_stringW(s_entries[i][UI_SUB_POS].name, sub_menu_text_x + 3, sub_menu_text_y - 11,
				FONT(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (s_entries[i][UI_SUB_POS].state == UI_TAB_STATE)
			{
				g_font->draw_string("+", sub_menu_text_x + head_box_width - 20, sub_menu_text_y - 11, FONT(Tahoma12px),
					TEXT_OUTLINE, (i == s_entry_pos[UI_SUB_POS] && m_sopened[UI_SUB_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else
			{
				if (s_entries[i][UI_SUB_POS].state != UI_FUNCTION_STATE)
				{
					if (s_entries[i][UI_SUB_POS].state == UI_BOOL_STATE)
					{
						draw_bool(g_var->get_as<bool>(s_entries[i][UI_SUB_POS].var).value(),
							sub_menu_text_x + head_box_width, sub_menu_text_y, m_colors[UI_PRIMARY_COL], m_colors[UI_MAIN_COL]);
					}
					else if (s_entries[i][UI_SUB_POS].state == UI_INT_STATE)
					{
						draw_int(g_var->get_as<int>(s_entries[i][UI_SUB_POS].var).value(),
							sub_menu_text_x + head_box_width, sub_menu_text_y, m_colors[UI_TEXT_COL]);
					}
					else if (s_entries[i][UI_SUB_POS].state == UI_FLOAT_STATE)
					{
						draw_float(g_var->get_as<float>(s_entries[i][UI_SUB_POS].var).value(),
							sub_menu_text_x + head_box_width, sub_menu_text_y, m_colors[UI_TEXT_COL]);
					}
					else if (s_entries[i][UI_SUB_POS].state == UI_KEY_STATE)
					{
						draw_hotkey(g_var->get_as<int>(s_entries[i][UI_SUB_POS].var).value(),
							sub_menu_box_x + head_box_width, sub_menu_text_x + head_box_width, sub_menu_box_y,
							sub_menu_text_y, head_box_height, s_entries[i][UI_SUB_POS].key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
					}
					else if (s_entries[i][UI_SUB_POS].state == UI_ITEM_STATE)
					{
						draw_item(s_entries[i][UI_SUB_POS].items[g_var->get_as<int>(s_entries[i][UI_SUB_POS].var).value()],
							sub_menu_text_x + head_box_width, sub_menu_text_y, s_entries[i][UI_SUB_POS].item_text_col);
					}
				}
			}

			sub_menu_box_y += head_box_height + 1;
			sub_menu_text_y += head_box_height + 1;

			if (s_entries[i][UI_SUB_POS].space)
			{
				sub_menu_box_y += head_box_height + 1;
				sub_menu_text_y += head_box_height + 1;
			}
		}
	}

	x += head_box_width + 1;

	if (m_sopened[UI_SUB_SUB_POS])
	{
		auto sub_sub_menu_box_x = x + 1;
		auto sub_sub_menu_box_y = head_y + head_box_height + 5;
		auto sub_sub_menu_text_x = sub_sub_menu_box_x + 6;
		auto sub_sub_menu_text_y = sub_sub_menu_box_y + int(head_box_height / 2.0f) + 4;

		for (int i = 0; i < s_entry_sz[UI_SUB_SUB_POS]; i++)
		{
			g_renderer->rect_fill(sub_sub_menu_box_x, sub_sub_menu_box_y, head_box_width, head_box_height, m_colors[UI_SHADOW_COL]);

			if (i == s_entry_pos[UI_SUB_SUB_POS])
			{
				if (!m_sopened[UI_SUB_SUB_SUB_POS])
					g_renderer->rect_fill(sub_sub_menu_box_x, sub_sub_menu_box_y, head_box_width, head_box_height, m_colors[UI_PRIMARY_COL]);

				g_renderer->rect_fill(sub_sub_menu_box_x, sub_sub_menu_box_y, 2, head_box_height, m_colors[UI_MAIN_COL]);
			}

			g_font->draw_stringW(s_entries[i][UI_SUB_SUB_POS].name, sub_sub_menu_text_x + 3, sub_sub_menu_text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (s_entries[i][UI_SUB_SUB_POS].state == UI_TAB_STATE)
			{
				g_font->draw_string("+", sub_sub_menu_text_x + head_box_width - 20, sub_sub_menu_text_y - 11, FONT(Tahoma12px),
					TEXT_OUTLINE, (i == s_entry_pos[UI_SUB_SUB_POS] && m_sopened[UI_SUB_SUB_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else
			{
				if (s_entries[i][UI_SUB_SUB_POS].state != UI_FUNCTION_STATE)
				{
					if (s_entries[i][UI_SUB_SUB_POS].state == UI_BOOL_STATE)
					{
						draw_bool(g_var->get_as<bool>(s_entries[i][UI_SUB_SUB_POS].var).value(),
							sub_sub_menu_text_x + head_box_width, sub_sub_menu_text_y, m_colors[UI_PRIMARY_COL], m_colors[UI_MAIN_COL]);
					}
					else if (s_entries[i][UI_SUB_SUB_POS].state == UI_INT_STATE)
					{
						draw_int(g_var->get_as<int>(s_entries[i][UI_SUB_SUB_POS].var).value(),
							sub_sub_menu_text_x + head_box_width, sub_sub_menu_text_y, m_colors[UI_TEXT_COL]);
					}
					else if (s_entries[i][UI_SUB_SUB_POS].state == UI_FLOAT_STATE)
					{
						draw_float(g_var->get_as<float>(s_entries[i][UI_SUB_SUB_POS].var).value(),
							sub_sub_menu_text_x + head_box_width, sub_sub_menu_text_y, m_colors[UI_TEXT_COL]);
					}
					else if (s_entries[i][UI_SUB_SUB_POS].state == UI_KEY_STATE)
					{
						draw_hotkey(g_var->get_as<int>(s_entries[i][UI_SUB_SUB_POS].var).value(),
							sub_sub_menu_box_x + head_box_width, sub_sub_menu_text_x + head_box_width, sub_sub_menu_box_y,
							sub_sub_menu_text_y, head_box_height, s_entries[i][UI_SUB_SUB_POS].key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
					}
					else if (s_entries[i][UI_SUB_SUB_POS].state == UI_ITEM_STATE)
					{
						draw_item(s_entries[i][UI_SUB_SUB_POS].items[g_var->get_as<int>(s_entries[i][UI_SUB_SUB_POS].var).value()],
							sub_sub_menu_text_x + head_box_width, sub_sub_menu_text_y, s_entries[i][UI_SUB_SUB_POS].item_text_col);
					}
				}
			}

			sub_sub_menu_box_y += head_box_height + 1;
			sub_sub_menu_text_y += head_box_height + 1;

			if (s_entries[i][UI_SUB_SUB_POS].space)
			{
				sub_sub_menu_box_y += head_box_height + 1;
				sub_sub_menu_text_y += head_box_height + 1;
			}
		}
	}

	x += head_box_width + 1;

	if (m_sopened[UI_SUB_SUB_SUB_POS])
	{
		auto sub_sub_sub_menu_box_x = x + 1;
		auto sub_sub_sub_menu_box_y = head_y + head_box_height + 5;
		auto sub_sub_sub_menu_text_x = sub_sub_sub_menu_box_x + 6;
		auto sub_sub_sub_menu_text_y = sub_sub_sub_menu_box_y + int(head_box_height / 2.0f) + 4;

		for (int i = 0; i < s_entry_sz[UI_SUB_SUB_SUB_POS]; i++)
		{
			g_renderer->rect_fill(sub_sub_sub_menu_box_x, sub_sub_sub_menu_box_y, head_box_width, head_box_height, m_colors[UI_SHADOW_COL]);

			if (i == s_entry_pos[UI_SUB_SUB_SUB_POS])
			{
				g_renderer->rect_fill(sub_sub_sub_menu_box_x, sub_sub_sub_menu_box_y, head_box_width, head_box_height, m_colors[UI_PRIMARY_COL]);
				g_renderer->rect_fill(sub_sub_sub_menu_box_x, sub_sub_sub_menu_box_y, 2, head_box_height, m_colors[UI_MAIN_COL]);
			}

			g_font->draw_stringW(s_entries[i][UI_SUB_SUB_SUB_POS].name, sub_sub_sub_menu_text_x + 3, sub_sub_sub_menu_text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (s_entries[i][UI_SUB_SUB_SUB_POS].state != UI_FUNCTION_STATE)
			{
				if (s_entries[i][UI_SUB_SUB_SUB_POS].state == UI_BOOL_STATE)
				{
					draw_bool(g_var->get_as<bool>(s_entries[i][UI_SUB_SUB_SUB_POS].var).value(),
						sub_sub_sub_menu_text_x + head_box_width, sub_sub_sub_menu_text_y, m_colors[UI_PRIMARY_COL], m_colors[UI_MAIN_COL]);
				}
				else if (s_entries[i][UI_SUB_SUB_SUB_POS].state == UI_INT_STATE)
				{
					draw_int(g_var->get_as<int>(s_entries[i][UI_SUB_SUB_SUB_POS].var).value(),
						sub_sub_sub_menu_text_x + head_box_width, sub_sub_sub_menu_text_y, m_colors[UI_TEXT_COL]);
				}
				else if (s_entries[i][UI_SUB_SUB_SUB_POS].state == UI_FLOAT_STATE)
				{
					draw_float(g_var->get_as<float>(s_entries[i][UI_SUB_SUB_SUB_POS].var).value(),
						sub_sub_sub_menu_text_x + head_box_width, sub_sub_sub_menu_text_y, m_colors[UI_TEXT_COL]);
				}
				else if (s_entries[i][UI_SUB_SUB_SUB_POS].state == UI_KEY_STATE)
				{
					draw_hotkey(g_var->get_as<int>(s_entries[i][UI_SUB_SUB_SUB_POS].var).value(),
						sub_sub_sub_menu_box_x + head_box_width, sub_sub_sub_menu_text_x + head_box_width, sub_sub_sub_menu_box_y,
						sub_sub_sub_menu_text_y, head_box_height, s_entries[i][UI_SUB_SUB_SUB_POS].key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
				}
				else if (s_entries[i][UI_SUB_SUB_SUB_POS].state == UI_ITEM_STATE)
				{
					draw_item(s_entries[i][UI_SUB_SUB_SUB_POS].items[g_var->get_as<int>(s_entries[i][UI_SUB_SUB_SUB_POS].var).value()],
						sub_sub_sub_menu_text_x + head_box_width, sub_sub_sub_menu_text_y, s_entries[i][UI_SUB_SUB_SUB_POS].item_text_col);
				}
			}

			sub_sub_sub_menu_box_y += head_box_height + 1;
			sub_sub_sub_menu_text_y += head_box_height + 1;

			if (s_entries[i][UI_SUB_SUB_SUB_POS].space)
			{
				sub_sub_sub_menu_box_y += head_box_height + 1;
				sub_sub_sub_menu_text_y += head_box_height + 1;
			}
		}
	}
}