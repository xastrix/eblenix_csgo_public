#pragma once

#include "interface.h"

struct i_group_box {
	i_group_box() = default;
	i_group_box(const std::string& name, int x, int y, int w, int h)
		: m_name(name), m_x(x), m_y(y), m_w(w), m_h(h) {
		draw();
	};

	int                       m_x{},
		                      m_y{},
		                      m_w{},
		                      m_h{}, m_ho{ 10 };
	std::string               m_name{};
	int                       m_element_count{};
private:
	void draw() {
		g_d3d.draw_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(66, 84, 114, 255));
		g_d3d.draw_filled_rect(m_x + 1, m_y + 1, m_w - 1, m_h - 1, D3DCOLOR_RGBA(54, 72, 102, 255));

		g_d3d.draw_string(m_name, m_x + 10, m_y - 7, D3DCOLOR_RGBA(255, 255, 255, 255));
	}
};

struct i_game_selector : private i_group_box {
	i_game_selector(i_group_box* parent, sprite_t game_icon, const std::string& game_name, int* selected, int game_id)
		: m_parent(parent), m_game_icon(game_icon), m_game_name(game_name), m_selected(selected), m_game_id(game_id) {
		if (m_parent) {
			m_x = m_parent->m_x + 1;
			m_y = m_parent->m_y + m_parent->m_ho;
			m_w = m_parent->m_w - 1;
			m_h = 30;

			m_parent->m_ho += (m_h + 1);
			m_parent->m_element_count++;
		}

		draw();
	};

private:
	bool is_hovered() {
		const auto mouse_pos_x = g_interface.get_mouse_pos_x();
		const auto mouse_pos_y = g_interface.get_mouse_pos_y();

		return mouse_pos_x >= m_x && mouse_pos_x <= m_x + m_w
			&& mouse_pos_y >= m_y && mouse_pos_y <= m_y + m_h;
	}

	void draw() {
		static bool m_held[3]{};

		if (is_hovered()) {
			g_d3d.draw_filled_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(68, 99, 153, 255));

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
				m_held[m_parent->m_element_count] = true;
				g_d3d.draw_filled_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(69, 107, 173, 255));
			}
			else {
				if (m_held[m_parent->m_element_count]) {
					*m_selected = !(*m_selected == m_game_id) ? m_game_id : -1;
					m_held[m_parent->m_element_count] = false;
				}
			}
		}
		else {
			if (m_held[m_parent->m_element_count])
				m_held[m_parent->m_element_count] = false;

			if (*m_selected == m_game_id)
				g_d3d.draw_filled_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(68, 90, 129, 255));
		}

		m_game_icon.draw(m_x + 4, m_y + 3);

		g_d3d.draw_rect(m_x + 3, m_y + 2, m_game_icon.get_width() + 1,
			m_game_icon.get_height() + 1, D3DCOLOR_RGBA(66, 84, 114, 255));

		if (m_game_name.length() > 28)
			m_game_name = m_game_name.substr(0, 28) + "..";

		g_d3d.draw_string(m_game_name, m_x + 35, m_y + 3,
			(*m_selected == m_game_id) ? D3DCOLOR_RGBA(255, 255, 255, 255) : D3DCOLOR_RGBA(200, 200, 200, 255));
	}

	int                       m_x{},
		                      m_y{},
		                      m_w{},
		                      m_h{};
	sprite_t                  m_game_icon{};
	std::string               m_game_name{};
	int*                      m_selected{};
	i_group_box*              m_parent{};
	int                       m_game_id{};
};

struct i_button : private i_group_box {
	i_button(i_group_box* parent, const std::string& name, int w, int h, std::function<void(void)> fn)
		: m_parent(parent), m_name(name), m_fn(fn) {
		if (m_parent) {
			m_x = m_parent->m_x + 10;
			m_y = m_parent->m_y + m_parent->m_ho;

			m_parent->m_ho += (h + 10);
			m_parent->m_element_count++;
		}

		m_w = w;
		m_h = h;

		draw();
	};

private:
	bool is_hovered() {
		const auto mouse_pos_x = g_interface.get_mouse_pos_x();
		const auto mouse_pos_y = g_interface.get_mouse_pos_y();

		return mouse_pos_x >= m_x && mouse_pos_x <= m_x + m_w
			&& mouse_pos_y >= m_y && mouse_pos_y <= m_y + m_h;
	}

	void draw() {
		static bool m_held[3]{};

		if (is_hovered()) {
			g_d3d.draw_filled_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(68, 99, 153, 255));

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
				m_held[m_parent->m_element_count] = true;
				g_d3d.draw_filled_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(69, 107, 173, 255));
			}
			else {
				if (m_held[m_parent->m_element_count]) {
					m_fn();
					m_held[m_parent->m_element_count] = false;
				}
			}
		}
		else {
			if (m_held[m_parent->m_element_count])
				m_held[m_parent->m_element_count] = false;

			g_d3d.draw_filled_rect(m_x, m_y, m_w, m_h, D3DCOLOR_RGBA(68, 90, 129, 255));
		}

		const auto text_width = g_d3d.get_text_width(m_name);
		const auto text_height = g_d3d.get_text_height(m_name);

		g_d3d.draw_string(m_name, m_x + m_w / 2 - text_width / 2,
			m_y + (m_h / 2) - (text_height / 2), D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	int                       m_x{},
		                      m_y{},
		                      m_w{},
		                      m_h{};
	std::string               m_name{};
	std::function<void(void)> m_fn{};
	i_group_box*              m_parent{};
};

struct ui {
	void begin();
	void end();
public:
	sprite_t m_csgo_icon{};
};

extern ui g_ui;