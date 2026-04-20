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

namespace ui
{
	struct ui_base_t {
		void init(IDirect3DDevice9* device, ui_rect_t rect);
		void begin_frame();

		void button(const std::string& field, int w, int h, std::function<void()> fn);
		void group_box(const std::string& field, ui_vec_t pos, ui_vec_t size);
		void game_selector(const std::string& field, sprite_t* icon, int *selected, int id);
		void util_box(ui_vec_t pos, std::vector<ui_util_t> list);
		void text_box(const std::string& field, ui_vec_t pos);

		void set_cursor_pos(ui_vec_t pos);
		void push_log(const std::string& msg);

		void on_reset();
		void on_reset_end();

		void end_frame();

	private:
		bool is_hovered(const ui_cursor_rect_t& rect);
		bool is_blocked() { return m_blocked; }
		void set_blocked(bool state) { m_blocked = state; }

	private:
		ui_rect_t                m_rect{};
		ui_erect_t               m_erect{};
		bool                     m_blocked{};
		std::vector<ui_notify_t> m_logs{};
	};

	inline ui_base_t* get = new ui_base_t();
	inline sprite_t* csgo_ico = new sprite_t();
}