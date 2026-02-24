#pragma once

#include "globals.h"
#include "interfaces.h"
#include "input.h"
#include "ui.h"
#include "helpers.h"

struct status_bar_t {
	status_bar_t(const status_bar_t&) = delete;
	status_bar_t& operator=(const status_bar_t&) = delete;

	static status_bar_t& get_instance() {
		static status_bar_t i{};
		return i;
	}

	void think() {
		const auto string_width = g_font->get_text_width(_PRODUCT_NAME, FONT(Tahoma12px)) + m_string_spacing;

		vec2 screen_size = g_renderer->get_screen_size();

		m_rect_min = vec2(screen_size.x - string_width - 15, g_var->get_as<float>(V_VISUALS_INTERFACE_STATUS_POS_Y).value());
		m_rect_max = vec2(screen_size.x, 17);

		m_rect_min.y = std::max(0.0f, std::min(m_rect_min.y, screen_size.y - m_rect_max.y));
	}

	void run() {
		think();
		draw();
	}

	void handle_click(bool left_click, bool right_click) {
		if (g_input->is_hovered(m_rect_min, m_rect_min + m_rect_max))
		{
			if (!m_ctx_menu_open && right_click)
				m_ctx_menu_open = true;
		}

		if (m_ctx_menu_open)
		{
			vec2 ctx_content_min(m_rect_min.x + 10, m_rect_min.y + 20);
			vec2 ctx_content_max(m_rect_max.x - 10, 17 + (m_vars.size() * 18));

			for (int i = 0; i < m_vars.size(); i++)
			{
				vec2 ctx_list_min(m_rect_min.x + 10, m_rect_min.y + 20 + (i * 18));
				vec2 ctx_list_max(m_rect_max.x - 10, 17);

				if (g_input->is_hovered(ctx_list_min, ctx_list_min + ctx_list_max) && left_click)
					g_var->set(m_vars[i].second, !g_var->get_as<bool>(m_vars[i].second).value());
			}

			if ((!g_input->is_hovered(m_rect_min, m_rect_min + m_rect_max) &&
				 !g_input->is_hovered(ctx_content_min, ctx_content_min + ctx_content_max)) && (left_click || right_click))
				m_ctx_menu_open = false;
		}
	}

	void handle_move(UINT m) {
		if (!m_rect_min && !m_rect_max)
			return;

		static draggable_object_t bar_drag_obj{ m_rect_min.x, m_rect_min.y, m_rect_max.x, m_rect_max.y };

		if (g_input->move_object(bar_drag_obj, m))
		{
			m_rect_min.y = bar_drag_obj.y;
			g_var->set(V_VISUALS_INTERFACE_STATUS_POS_Y, m_rect_min.y);
		}

		else
			bar_drag_obj.y = m_rect_min.y;
	}

private:
	status_bar_t() = default;

	void draw() {
		const auto menu_shadow_alpha = g_var->get_as<int>(V_UI_COL_A).value();

		vec2 screen_size = g_renderer->get_screen_size();

		g_renderer->rect_fill(m_rect_min, m_rect_max, color_t(20, 20, 20, menu_shadow_alpha));

		g_renderer->rect_fill(m_rect_min.x + 3, m_rect_min.y + 3, 2, 11,
			GLOBAL(b_flags[BF_INITIALISED]) ? color_t(V_UI_COL) : color_t(164, 164, 164));

		g_font->draw_string(_PRODUCT_NAME, m_rect_min.x + 10, m_rect_min.y + 2,
			FONT(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));

		if (g_ui->get_menu_state())
		{
			if (m_ctx_menu_open)
			{
				for (int i = 0; i < m_vars.size(); i++)
				{
					vec2 ctx_list_min(m_rect_min.x + 10, m_rect_min.y + 20 + (i * 18));
					vec2 ctx_list_max(m_rect_max.x - 10, 17);

					bool is_hovered = g_input->is_hovered(ctx_list_min, ctx_list_min + ctx_list_max);
					bool is_actived = g_var->get_as<bool>(m_vars[i].second).value();

					g_renderer->rect_fill(ctx_list_min, ctx_list_max, color_t(20, 20, 20, menu_shadow_alpha - 20));
					g_renderer->rect_fill(ctx_list_min.x + 3, ctx_list_min.y + 3, 2, 11,
						is_actived ? color_t(V_UI_COL) : color_t(164, 164, 164));

					g_font->draw_string(m_vars[i].first, ctx_list_min.x + 9, ctx_list_min.y + 2, FONT(Tahoma12px), TEXT_OUTLINE,
						is_hovered ? color_t(V_UI_COL) : color_t(255, 255, 255));
				}
			}
		}
		else
			m_ctx_menu_open = false;

		int x = 0;
		auto push_item = [&](const std::string& label, ID3DXFont* font, bool v) {
			const auto string_width = g_font->get_text_width(label, font);

			g_renderer->rect_fill(m_rect_min.x - string_width - x - 13, m_rect_min.y, string_width + 11, 17, color_t(20, 20, 20, menu_shadow_alpha));
			g_renderer->rect_fill(m_rect_min.x - string_width - x - 10, m_rect_min.y + 3, 2, 11, v ? color_t(V_UI_COL) : color_t(164, 164, 164));

			g_font->draw_string(label, m_rect_min.x - string_width - x - 6, m_rect_min.y + 2, font, TEXT_OUTLINE, color_t(255, 255, 255));

			x += string_width + 13;
		};

		if (g_cs->m_engine->is_connected())
		{
			if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_ESP).value())
				push_item("E", FONT(Verdana12px), g_var->get_as<bool>(V_ESP_ENABLED).value());

			if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_TRIGGERBOT).value())
				push_item("T", FONT(Verdana12px), g_var->get_as<bool>(V_TRIGGERBOT_ENABLED).value());

			if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_AIMBOT).value())
				push_item("A", FONT(Verdana12px), g_var->get_as<bool>(V_AIMBOT_ENABLED).value());
		}

		if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_FPS).value())
		{
			char buffer[50]{};
			sprintf(buffer, " %i FPS", int(1.0f / g_cs->m_globals->frame_time));

			push_item(buffer, FONT(Tahoma12px), true);
		}

		if (g_cs->m_engine->is_connected())
		{
			if (g_cs->get_local())
			{
				if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_VELOCITY).value())
				{
					float velocity = length(g_cs->get_local()->get_velocity());

					char buffer[50]{};
					sprintf(buffer, " Velocity: %.2f", velocity);

					push_item(buffer, FONT(Tahoma12px), velocity > 0.0f);
				}
			}

			auto net_channel_info = g_cs->m_engine->get_net_channel_info();

			if (net_channel_info)
			{
				int incoming = std::max(static_cast<int>(net_channel_info->get_latency(FLOW_INCOMING) * 1000.0f), 0);
				int outgoing = std::max(static_cast<int>(net_channel_info->get_latency(FLOW_OUTGOING) * 1000.0f), 0);

				if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_INCOMING).value())
					push_item(" Incoming: " + std::to_string(incoming) + "ms", FONT(Tahoma12px), incoming > 0);

				if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_OUTGOING).value())
					push_item(" Outgoing: " + std::to_string(outgoing) + "ms", FONT(Tahoma12px), outgoing > 0);
			}

			if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_C4).value())
			{
				if (GLOBAL(b_flags[BF_BOMB_PLANTED]))
				{
					switch (GLOBAL(i_flags[IF_BOMB_SITE_ID])) {
					case BS_A: {
						push_item(" C4 (A): " + std::to_string(GLOBAL(i_flags[IF_BOMB_TIMER])), FONT(Tahoma12px), true);
						break;
					}
					case BS_B: {
						push_item(" C4 (B): " + std::to_string(GLOBAL(i_flags[IF_BOMB_TIMER])), FONT(Tahoma12px), true);
						break;
					}
					}
				}
			}
		}
	}

	vec2 m_rect_min{};
	vec2 m_rect_max{};

private:
	std::vector<std::pair<std::string, std::string>> m_vars = {
		{ "Aim",      V_VISUALS_INTERFACE_STATUS_AIMBOT },
		{ "Trigger",  V_VISUALS_INTERFACE_STATUS_TRIGGERBOT },
		{ "Esp",      V_VISUALS_INTERFACE_STATUS_ESP },
		{ "Fps",      V_VISUALS_INTERFACE_STATUS_FPS },
		{ "Velocity", V_VISUALS_INTERFACE_STATUS_VELOCITY },
		{ "Incoming", V_VISUALS_INTERFACE_STATUS_INCOMING },
		{ "Outgoing", V_VISUALS_INTERFACE_STATUS_OUTGOING },
		{ "C4",       V_VISUALS_INTERFACE_STATUS_C4 },
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

	void think() {
		vec2 screen_size = g_renderer->get_screen_size();

		m_rect_min = vec2(screen_size.x - 105, g_var->get_as<float>(V_VISUALS_INTERFACE_SPECTATORS_POS_Y).value());
		m_rect_max = vec2(screen_size.x, 18);

		m_rect_min.y = std::max(0.0f, std::min(m_rect_min.y, screen_size.y - m_rect_max.y));
	}

	void run() {
		think();
		draw();
	}

	void handle_move(UINT m) {
		if (!m_rect_min && !m_rect_max)
			return;

		static draggable_object_t spec_drag_obj{ m_rect_min.x, m_rect_min.y, m_rect_max.x, m_rect_max.y };

		if (g_input->move_object(spec_drag_obj, m))
		{
			m_rect_min.y = spec_drag_obj.y;
			g_var->set(V_VISUALS_INTERFACE_SPECTATORS_POS_Y, m_rect_min.y);
		}

		else
			spec_drag_obj.y = m_rect_min.y;
	}

private:
	spectators_t() = default;

	void draw() {
		const auto menu_shadow_alpha = g_var->get_as<int>(V_UI_COL_A).value();

		if (!g_cs->m_engine->is_in_game())
			return;

		g_renderer->rect_fill(m_rect_min, m_rect_max, color_t(20, 20, 20, menu_shadow_alpha));
		g_renderer->rect_fill(m_rect_min.x + 3, m_rect_min.y + 3, 2, 12, color_t(V_UI_COL));

		g_font->draw_string("Spectators", m_rect_min.x + 9, m_rect_min.y + 2,
			FONT(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));

		int y = 1;
		for (int i = 1; i <= g_cs->m_globals->max_clients; i++) {
			auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

			if (!entity)
				continue;

			if (entity == g_cs->get_local())
				continue;

			if (entity->is_life_state())
				continue;

			if (entity->get_dormant())
				continue;

			const auto target = entity->get_observer_target();

			if (!target)
				continue;

			const auto spectator_target = g_cs->m_entity_list->get_client_entity_handle<c_base_entity*>(target);

			if (spectator_target != g_cs->get_local())
				continue;

			g_renderer->rect_fill(m_rect_min.x, m_rect_min.y + (19 * y), m_rect_max.x, m_rect_max.y, color_t(20, 20, 20, menu_shadow_alpha));
			g_renderer->rect_fill(m_rect_min.x + 3, m_rect_min.y + 3 + (19 * y), 2, 12, color_t(164, 164, 164));

			player_info_t info;
			g_cs->m_engine->get_player_info(i, &info);

			auto player_name = Helpers::stws(std::string{ info.player_name });

			if (player_name.length() > 14)
				player_name = player_name.substr(0, 14) + L"...";

			g_font->draw_stringW(player_name, m_rect_min.x + 9, m_rect_min.y + 2 + (19 * y),
				FONT(Tahoma12px), TEXT_OUTLINE, color_t(255, 255, 255));

			y++;
		}
	}

	vec2 m_rect_min{};
	vec2 m_rect_max{};
};

class c_hud {
public:
	void run() {
		if (!g_var->get_as<bool>(V_VISUALS_ENABLED).value())
			return;

		if (g_var->get_as<bool>(V_VISUALS_INTERFACE_SPECTATORS).value())
			spectators_t::get_instance().run();

		if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS).value())
			status_bar_t::get_instance().run();
	}

	static std::shared_ptr<c_hud> make_shared() {
		return std::shared_ptr<c_hud>(new c_hud());
	}

	void process_message(UINT m) {
		if (!g_var->get_as<bool>(V_VISUALS_ENABLED).value())
			return;

		if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS).value())
		{
			switch (m) {
			case WM_LBUTTONDOWN: {
				status_bar_t::get_instance().handle_click(true, false);
				break;
			}
			case WM_LBUTTONUP: {
				status_bar_t::get_instance().handle_click(false, false);
				break;
			}
			case WM_RBUTTONDOWN: {
				status_bar_t::get_instance().handle_click(false, true);
				break;
			}
			case WM_RBUTTONUP: {
				status_bar_t::get_instance().handle_click(false, false);
				break;
			}
			}

			status_bar_t::get_instance().handle_move(m);
		}

		if (g_var->get_as<bool>(V_VISUALS_INTERFACE_SPECTATORS).value())
			spectators_t::get_instance().handle_move(m);
	}

	void notify_hud(const std::string& msg, color_t rect_c) {
		const auto string_font = FONT(Tahoma12px);
		const auto string_width = g_font->get_text_width(msg, string_font);

		vec2 screen_size = g_renderer->get_screen_size();

		g_renderer->rect_fill(screen_size.x - string_width - 15, 10, screen_size.x, 17, color_t(20, 20, 20, 140));
		g_renderer->rect_fill(screen_size.x - string_width - 12, 13, 2, 11, rect_c);

		g_font->draw_string(msg, screen_size.x - string_width - 5, 12, string_font, TEXT_OUTLINE, color_t(255, 255, 255));
	}
};

inline std::shared_ptr<c_hud> g_hud = c_hud::make_shared();