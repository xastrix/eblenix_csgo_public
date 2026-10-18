#include "ui.h"

#include "input.h"
#include "interfaces.h"

static int g_belem_index = 0;

static void draw_int(int value, int x, int y, c_color color);
static void draw_bool(bool value, int x, int y, c_color first, c_color second);
static void draw_float(float value, int x, int y, c_color color);
static void draw_hotkey(int k, int x, int x_text, int y, int y_text, int h, bool hold, c_color color, c_color background_color);
static void draw_item(std::wstring item, int x, int y, c_color color);

void c_ui::draw(int x, int y)
{
	const auto head_box_width = MENU_BOX_WIDTH;
	const auto head_box_height = MENU_BOX_HEIGHT;

	const auto menu_logo_x = x + 1;
	const auto menu_logo_y = (y + head_box_height + 4) - m_sprites[UI_SPRITE_LOGO].get_height() + 1;

	m_menu_width = head_box_width + 2;
	m_sprites[UI_SPRITE_LOGO].draw(menu_logo_x, menu_logo_y, c_color(255, 255, 255, static_cast<int>(m_target_animation_progress * 255.0f)));

	auto menu_max_entries = m_entry_sz;
	auto menu_max_y = 0;

	auto padding_y = y + head_box_height + 5;
	auto current_y = padding_y;

	g_belem_index = 0;

	for (int i = 0; i < m_entry_sz; i++)
	{
		const auto text_y = current_y + int(head_box_height / 2.0f) + 4;

		g_renderer.rect_fill(x + 1, current_y, head_box_width, head_box_height, m_colors[UI_SHADOW_COL]);

		if (i == m_entry_pos)
		{
			if (!m_sopened[UI_SUB_POS])
				g_renderer.rect_fill(x + 1, current_y, head_box_width, head_box_height, m_colors[UI_PRIMARY_COL]);

			g_renderer.rect_fill(x + 1, current_y, 2, head_box_height, m_colors[UI_MAIN_COL]);
		}

		g_font.draw_stringW(m_entry[i].name, x + 10, text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);
		g_font.draw_string("+", x + head_box_width - 13, text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE,
			(i == m_entry_pos && m_sopened[UI_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);

		current_y += head_box_height + 1 + (m_entry[i].space ? (head_box_height + 1) : 0);
	}

	menu_max_y = current_y + 2;

	auto draw_sub_menu = [&](int pos_index, int open_condition, int next_open_condition)
	{
		x += head_box_width + 1;
		m_menu_width += head_box_width + 1;

		if (s_entry_sz[pos_index] > menu_max_entries) menu_max_entries = s_entry_sz[pos_index];

		auto column_y = padding_y;

		for (int i = 0; i < s_entry_sz[pos_index]; i++)
		{
			const auto& entry = s_entries[i][pos_index];

			const auto text_x = x + 7;
			const auto text_y = column_y + int(head_box_height / 2.0f) + 4;

			g_renderer.rect_fill(x + 1, column_y, head_box_width, head_box_height, m_colors[UI_SHADOW_COL]);

			if (i == s_entry_pos[pos_index])
			{
				if (next_open_condition == -1 || !m_sopened[next_open_condition])
					g_renderer.rect_fill(x + 1, column_y, head_box_width, head_box_height, m_colors[UI_PRIMARY_COL]);

				g_renderer.rect_fill(x + 1, column_y, 2, head_box_height, m_colors[UI_MAIN_COL]);
			}

			g_font.draw_stringW(entry.name, text_x + 3, text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (entry.state == UI_TAB_STATE)
			{
				bool is_active_tab = (next_open_condition != -1
					&& i == s_entry_pos[pos_index] && m_sopened[next_open_condition]);

				g_font.draw_string("+", text_x + head_box_width - 20, text_y - 11, FONT(Tahoma12px), TEXT_OUTLINE,
					is_active_tab ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else if (entry.state != UI_FUNCTION_STATE)
			{
				const auto control_x = text_x + head_box_width;

				switch (entry.state) {
				case UI_BOOL_STATE: {
					draw_bool(g_var.get_as<bool>(entry.var).value(), control_x, text_y, m_colors[UI_PRIMARY_COL], m_colors[UI_MAIN_COL]);
					break;
				}
				case UI_INT_STATE: {
					draw_int(g_var.get_as<int>(entry.var).value(), control_x, text_y, m_colors[UI_TEXT_COL]);
					break;
				}
				case UI_FLOAT_STATE: {
					draw_float(g_var.get_as<float>(entry.var).value(), control_x, text_y, m_colors[UI_TEXT_COL]);
					break;
				}
				case UI_KEY_STATE: {
					draw_hotkey(g_var.get_as<int>(entry.var).value(), x + 1 + head_box_width, control_x, column_y, text_y, head_box_height, entry.key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
					break;
				}
				case UI_ITEM_STATE: {
					draw_item(entry.items[g_var.get_as<int>(entry.var).value()], control_x, text_y, entry.item_text_col);
					break;
				}
				}
			}

			column_y += head_box_height + 1 + (entry.space ? (head_box_height + 1) : 0);
		}

		if (column_y + 2 > menu_max_y) menu_max_y = column_y + 2;
	};

	if (m_sopened[UI_SUB_POS])         draw_sub_menu(UI_SUB_POS, UI_SUB_POS, UI_SUB_SUB_POS);
	if (m_sopened[UI_SUB_SUB_POS])     draw_sub_menu(UI_SUB_SUB_POS, UI_SUB_SUB_POS, UI_SUB_SUB_SUB_POS);
	if (m_sopened[UI_SUB_SUB_SUB_POS]) draw_sub_menu(UI_SUB_SUB_SUB_POS, UI_SUB_SUB_SUB_POS, -1);

	m_menu_height = menu_max_y - y;
}

static void draw_int(int value, int x, int y, c_color color)
{
	const auto font = FONT(Tahoma12px);
	const auto string_width = g_font.get_text_width(std::to_string(value), font);

	g_font.draw_string(std::to_string(value), x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
}

static void draw_bool(bool value, int x, int y, c_color first, c_color second)
{
	static std::vector<float> anim_x{};

	if (g_belem_index >= anim_x.size())
		anim_x.resize(g_belem_index + 1, value ? 25.0f : 37.0f);

	const float target_x = value ? 25.0f : 38.0f;
	const float speed = g_var.get_as<float>(V_UI_ANIM_CHECKBOX_SWITCH).value();

	float delta_time = std::clamp(g_cs.m_globals->frame_time, 0.0001f, 0.1f);

	float fps_factor = delta_time * 60.0f;
	float final_step = std::clamp(speed * fps_factor, 0.0f, 1.0f);

	anim_x[g_belem_index] += (target_x - anim_x[g_belem_index]) * final_step;
	anim_x[g_belem_index] = std::clamp(anim_x[g_belem_index], 25.0f, 37.0f);

	g_renderer.rect_fill(x - 38, y - 11, 26, 14, g_ui.get_menu_color(UI_OUTLINE_COL));
	g_renderer.gradient_h(x - static_cast<int>(anim_x[g_belem_index]), y - 10, 12, 12,
		value ? first : second, value ? second : first);

	++g_belem_index;
}

static void draw_float(float value, int x, int y, c_color color)
{
	char ss[256];
	sprintf_s(ss, "%.2f", value);

	const auto font = FONT(Tahoma12px);
	const auto string_width = g_font.get_text_width(ss, font);

	g_font.draw_string(ss, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
}

static void draw_hotkey(int k, int x, int x_text, int y, int y_text, int h, bool hold, c_color color, c_color background_color)
{
	const auto kss = g_input.virtual_key_to_wstring(k);

	const auto font = FONT(Tahoma12px);
	const auto key_string_width = g_font.get_text_widthW(hold ? L"---" : kss, font);

	g_renderer.rect_fill(x - key_string_width - 47, y, 47 + key_string_width, h, background_color);

	g_font.draw_stringW(hold ? L"---" : kss, x_text - key_string_width - 30,
		y_text - 11, font, TEXT_OUTLINE, color);
}

static void draw_item(std::wstring item, int x, int y, c_color color)
{
	if (item.length() > 20)
		item = item.substr(0, 20) + L"...";

	const auto font = FONT(Tahoma12px);
	const auto string_width = g_font.get_text_widthW(item, font);

	g_font.draw_stringW(item, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
}