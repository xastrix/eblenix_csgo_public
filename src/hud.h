#pragma once

#include "vars.h"
#include "renderer.h"

struct status_bar_t {
	status_bar_t(const status_bar_t&) = delete;
	status_bar_t& operator=(const status_bar_t&) = delete;

	static status_bar_t& get_instance() {
		static status_bar_t i{};
		return i;
	}

	void run();
	void think();

	void handle_click(bool left_click, bool right_click);
	void handle_move(UINT m);

private:
	status_bar_t() = default;

	void draw();

	vec2 m_rect_min{};
	vec2 m_rect_max{};

	std::vector<std::pair<std::string, std::string>> m_vars = {
		{ "Aim",          V_VISUALS_INTERFACE_STATUS_AIMBOT },
		{ "Trigger",      V_VISUALS_INTERFACE_STATUS_TRIGGERBOT },
		{ "Esp",          V_VISUALS_INTERFACE_STATUS_ESP },
		{ "Fps",          V_VISUALS_INTERFACE_STATUS_FPS },
		{ "Velocity",     V_VISUALS_INTERFACE_STATUS_VELOCITY },
		{ "Incoming",     V_VISUALS_INTERFACE_STATUS_INCOMING },
		{ "Outgoing",     V_VISUALS_INTERFACE_STATUS_OUTGOING },
		{ "C4",           V_VISUALS_INTERFACE_STATUS_C4 },
		{ "Time",         V_VISUALS_INTERFACE_STATUS_TIME },
		{ "Time playing", V_VISUALS_INTERFACE_STATUS_TIME_PLAYING },
	};

	bool m_ctx_menu_open{};
	float m_string_spacing{ 40.0f };
};

struct spectators_t {
	spectators_t(const spectators_t&) = delete;
	spectators_t& operator=(const spectators_t&) = delete;

	static spectators_t& get_instance() {
		static spectators_t i{};
		return i;
	}

	void run();
	void think();

	void handle_move(UINT m);

private:
	spectators_t() = default;

	void draw();

	vec2 m_rect_min{};
	vec2 m_rect_max{};
};

class c_hud {
public:
	void run();

	static std::shared_ptr<c_hud> make_shared() {
		return std::shared_ptr<c_hud>(new c_hud());
	}

	void process_message(UINT m);
};

inline std::shared_ptr<c_hud> g_hud = c_hud::make_shared();