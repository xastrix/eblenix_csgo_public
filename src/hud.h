#pragma once

#include "globals.h"
#include "interfaces.h"
#include "input.h"
#include "ui.h"
#include "helpers.h"

#define RECT_WIDTH  2
#define RECT_HEIGHT 11

struct status_bar_t {
	status_bar_t(const status_bar_t&) = delete;
	status_bar_t& operator=(const status_bar_t&) = delete;

	static status_bar_t& get_instance() {
		static status_bar_t i{};
		return i;
	}

	void run() {
		draw();
	}

	void handle_click(bool click) {
		const auto string_font = g_font[Tahoma12px];
		const auto string_width = g_font.get_text_width("EBLENIX", string_font) + m_string_spacing;

		vec2 screen_size = g_renderer.get_screen_size();

		vec2 ctx_min(screen_size.x - string_width - 15, 10);
		vec2 ctx_max(screen_size.x, 17);

		if (g_input.is_hovered(ctx_min, ctx_min + ctx_max))
		{
			if (!m_ctx_menu_open && click)
				m_ctx_menu_open = true;
		}

		if (m_ctx_menu_open)
		{
			vec2 ctx_content_min(ctx_min.x + 10, 30);
			vec2 ctx_content_max(ctx_max.x - 10, 17 + (m_vars.size() * 18));

			for (int i = 0; i < m_vars.size(); i++)
			{
				vec2 ctx_list_min(ctx_min.x + 10, 30 + (i * 18));
				vec2 ctx_list_max(ctx_max.x - 10, 17);

				if (g_input.is_hovered(ctx_list_min, ctx_list_min + ctx_list_max) && click)
					g_vars.set(m_vars[i].second, !g_vars.get_as<bool>(m_vars[i].second).value());
			}

			if (!g_input.is_hovered(ctx_min, ctx_min + ctx_max) &&
				!g_input.is_hovered(ctx_content_min, ctx_content_min + ctx_content_max))
			{
				if (click)
					m_ctx_menu_open = false;
			}
		}
	}

private:
	status_bar_t() = default;

	void draw() {
		const auto string_font = g_font[Tahoma12px];
		const auto string_width = g_font.get_text_width("EBLENIX", string_font) + m_string_spacing;

		const auto menu_shadow_alpha = g_vars.get_as<int>(V_UI_COL_A).value();

		vec2 screen_size = g_renderer.get_screen_size();

		vec2 ctx_min(screen_size.x - string_width - 15, 10);
		vec2 ctx_max(screen_size.x, 17);

		g_renderer.rect_fill(ctx_min, ctx_max, color_t(20, 20, 20, menu_shadow_alpha));

		g_renderer.rect_fill(ctx_min.x + 3, ctx_min.y + 3, RECT_WIDTH, RECT_HEIGHT,
			GLOBAL(b_flags[BF_INITIALISED]) ? color_t(V_UI_COL) : color_t(164, 164, 164));

		g_font.draw_string("EBLENIX", ctx_min.x + 9, ctx_min.y + 2, string_font, TEXT_OUTLINE, color_t(255, 255, 255));

		if (g_ui.get_menu_state())
		{
			if (m_ctx_menu_open)
			{
				for (int i = 0; i < m_vars.size(); i++)
				{
					vec2 ctx_list_min(ctx_min.x + 10, 30 + (i * 18));
					vec2 ctx_list_max(ctx_max.x - 10, 17);

					bool is_hovered = g_input.is_hovered(ctx_list_min, ctx_list_min + ctx_list_max);
					bool is_actived = g_vars.get_as<bool>(m_vars[i].second).value();

					g_renderer.rect_fill(ctx_list_min, ctx_list_max, color_t(20, 20, 20, menu_shadow_alpha - 20));
					g_renderer.rect_fill(ctx_list_min.x + 3, ctx_list_min.y + 3, RECT_WIDTH, RECT_HEIGHT,
						is_actived ? color_t(V_UI_COL) : color_t(164, 164, 164));

					g_font.draw_string(m_vars[i].first, ctx_list_min.x + 9, ctx_list_min.y + 2, string_font, TEXT_OUTLINE,
						is_hovered ? color_t(V_UI_COL) : color_t(255, 255, 255));
				}
			}
		}
		else
			m_ctx_menu_open = false;

		int x = 0;
		auto push_item = [&](const std::string& label, ID3DXFont* font, bool v) {
			const auto string_width = g_font.get_text_width(label, font);

			g_renderer.rect_fill(ctx_min.x - string_width - x - 13, 10, string_width + 11, 17, color_t(20, 20, 20, menu_shadow_alpha));
			g_renderer.rect_fill(ctx_min.x - string_width - x - 10, 13, RECT_WIDTH, RECT_HEIGHT, v ? color_t(V_UI_COL) : color_t(164, 164, 164));

			g_font.draw_string(label, ctx_min.x - string_width - x - 6, 12, font, TEXT_OUTLINE, color_t(255, 255, 255));

			x += string_width + 13;
		};

		if (g_csgo.m_engine->is_connected())
		{
			if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS_ESP).value())
				push_item("E", g_font[Verdana12px], g_vars.get_as<bool>(V_ESP_ENABLED).value());

			if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS_TRIGGERBOT).value())
				push_item("T", g_font[Verdana12px], g_vars.get_as<bool>(V_TRIGGERBOT_ENABLED).value());

			if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS_AIMBOT).value())
				push_item("A", g_font[Verdana12px], g_vars.get_as<bool>(V_AIMBOT_ENABLED).value());

			if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS_C4).value())
			{
				if (GLOBAL(b_flags[BF_BOMB_PLANTED]))
				{
					switch (GLOBAL(i_flags[IF_BOMB_SITE_ID])) {
					case BS_A: {
						push_item("C4 (A): " + std::to_string(GLOBAL(i_flags[IF_BOMB_TIMER])), g_font[Tahoma12px], true);
						break;
					}
					case BS_B: {
						push_item("C4 (B): " + std::to_string(GLOBAL(i_flags[IF_BOMB_TIMER])), g_font[Tahoma12px], true);
						break;
					}
					}
				}
			}
		}
	}

private:
	std::vector<std::pair<std::string, std::string>> m_vars = {
		{ "Aim", V_VISUALS_INTERFACE_STATUS_AIMBOT },
		{ "Trigger", V_VISUALS_INTERFACE_STATUS_TRIGGERBOT },
		{ "Esp", V_VISUALS_INTERFACE_STATUS_ESP },
		{ "C4 Timer", V_VISUALS_INTERFACE_STATUS_C4 },
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

	void run() {
		draw();
	}

private:
	spectators_t() = default;

	void draw() {
		vec2 screen_size = g_renderer.get_screen_size();
		const auto menu_shadow_alpha = g_vars.get_as<int>(V_UI_COL_A).value();

		if (!g_csgo.m_engine->is_in_game())
			return;

		const float center_y = screen_size.y * 0.5f;

		g_renderer.rect_fill(screen_size.x - 105, center_y, screen_size.x, 18, color_t(20, 20, 20, menu_shadow_alpha));
		g_renderer.rect_fill(screen_size.x - 102, center_y + 3, 2, 12, color_t(V_UI_COL));

		const auto string_font = g_font[Tahoma12px];

		g_font.draw_string("Spectators", screen_size.x - 96, center_y + 2,
			string_font, TEXT_OUTLINE, color_t(255, 255, 255));

		int y = 1;
		for (int i = 1; i <= g_csgo.m_globals->max_clients; i++) {
			auto entity = g_csgo.m_entity_list->get_client_entity<c_base_player*>(i);

			if (!entity)
				continue;

			if (entity == g_csgo.get_local())
				continue;

			if (entity->is_life_state())
				continue;

			if (entity->get_dormant())
				continue;

			const auto target = entity->get_observer_target();

			if (!target)
				continue;

			const auto spectator_target = g_csgo.m_entity_list->get_client_entity_handle<c_base_entity*>(target);

			if (spectator_target != g_csgo.get_local())
				continue;

			g_renderer.rect_fill(screen_size.x - 105, center_y + (19 * y), screen_size.x, 18, color_t(20, 20, 20, menu_shadow_alpha));
			g_renderer.rect_fill(screen_size.x - 102, center_y + 3 + (19 * y), 2, 12, color_t(164, 164, 164));

			player_info_t info;
			g_csgo.m_engine->get_player_info(i, &info);

			auto player_name = Helpers::stws(std::string{ info.player_name });

			if (player_name.length() > 14) {
				player_name = player_name.substr(0, 14) + L"...";
			}

			g_font.draw_stringW(player_name, screen_size.x - 96, center_y + 2 + (19 * y),
				string_font, TEXT_OUTLINE, color_t(255, 255, 255));

			y++;
		}
	}
};

class c_hud {
public:
	void run() {
		if (!g_vars.get_as<bool>(V_VISUALS_ENABLED).value())
			return;

		if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS).value())
			status_bar_t::get_instance().run();

		if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_SPECTATORS).value())
			spectators_t::get_instance().run();
	}

	void process_message(UINT m) {
		if (!g_vars.get_as<bool>(V_VISUALS_ENABLED).value())
			return;

		switch (m) {
		case WM_LBUTTONDOWN: {
			if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS).value())
				status_bar_t::get_instance().handle_click(true);
			break;
		}
		case WM_LBUTTONUP: {
			if (g_vars.get_as<bool>(V_VISUALS_INTERFACE_STATUS).value())
				status_bar_t::get_instance().handle_click(false);
			break;
		}
		}
	}

	void notify_hud(const std::string& msg, color_t rect_c) {
		const auto string_font = g_font[Tahoma12px];
		const auto string_width = g_font.get_text_width(msg, string_font);

		vec2 screen_size = g_renderer.get_screen_size();

		g_renderer.rect_fill(screen_size.x - string_width - 15, 10, screen_size.x, 17, color_t(20, 20, 20, 140));
		g_renderer.rect_fill(screen_size.x - string_width - 12, 13, 2, 11, rect_c);

		g_font.draw_string(msg, screen_size.x - string_width - 5, 12, string_font, TEXT_OUTLINE, color_t(255, 255, 255));
	}
};

inline c_hud g_hud;