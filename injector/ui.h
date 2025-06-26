#pragma once

#include "d3d.h"

#include <functional>
#include <unordered_map>
#include <thread>

struct ui_vec_t {
	ui_vec_t(int x = 0, int y = 0) : x(x), y(y) {}
	int x, y;
};

struct ui_rect_t {
	ui_vec_t m_pos{ 0, 0 };
	ui_vec_t m_size;
};

struct ui_erect_t {
	ui_vec_t m_start_pos;
	ui_vec_t m_end_pos;
};

struct ui_cursor_rect_t {
	ui_vec_t m_first;
	ui_vec_t m_second;
};

struct ui_util_t {
	std::string m_name;
	std::function<void()> m_fn;
};
struct ui_notify_t {
	std::string m_msg{};
	std::chrono::steady_clock::time_point m_start_time{};
};

struct gui {
	struct base_t {
		void init(IDirect3DDevice9* device, ui_rect_t rect);
		void begin_frame();

		void button(const std::string& field, std::function<void()> fn);
		void group_box(const std::string& field, ui_vec_t pos, ui_vec_t size);
		void game_selector(const std::string& field, sprite_t icon, int *selected, int id);
		void util_box(ui_vec_t pos, std::vector<ui_util_t> ulist);
		void text_box(const std::string& field, ui_vec_t pos);

		void pop_cursor_pos(ui_vec_t group_pos);
		void push_log(const std::string& msg);

		void on_reset();
		void on_reset_end();

		void end_frame();

	private:
		bool is_hovered(const ui_cursor_rect_t& rect);

	private:
		ui_rect_t m_rect{};
		ui_erect_t m_erect{};
		std::vector<ui_notify_t> m_logs{};
	};

	void clear_frame(IDirect3DDevice9* device) {
		device->Clear(
			0, NULL,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			D3DCOLOR_ARGB(255, 68, 90, 129),
			1.0f, 0
		);
	}

	base_t* get = new base_t();
	sprite_t* csgo_icon_sprite = new sprite_t();
};

inline gui g_ui;