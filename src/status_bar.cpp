#include "hud.h"

#include "globals.h"
#include "interfaces.h"
#include "input.h"
#include "ui.h"
#include "helpers.h"

void status_bar_t::run()
{
	think();
	draw();
}

void status_bar_t::think()
{
	const auto string_width = g_font->get_text_width(_PRODUCT_NAME, FONT(Tahoma12px)) + m_string_spacing;

	vec2 screen_size = g_renderer->get_screen_size();

	m_rect_min = vec2(screen_size.x - string_width - 15, g_var->get_as<float>(V_VISUALS_INTERFACE_STATUS_POS_Y).value());
	m_rect_max = vec2(screen_size.x, 17);

	m_rect_min.y = std::max(0.0f, std::min(m_rect_min.y, screen_size.y - m_rect_max.y));
}

void status_bar_t::draw()
{
	const auto menu_shadow_alpha = g_var->get_as<int>(V_UI_COL_A).value();

	vec2 screen_size = g_renderer->get_screen_size();

	g_renderer->rect_fill(m_rect_min, m_rect_max, c_color(20, 20, 20, menu_shadow_alpha));

	g_renderer->rect_fill(m_rect_min.x + 3, m_rect_min.y + 3, 2, 11,
		GLOBAL(b_flags[BF_INITIALISED]) ? c_color(V_UI_COL) : c_color(164, 164, 164));

	g_font->draw_string(_PRODUCT_NAME, m_rect_min.x + 10, m_rect_min.y + 2,
		FONT(Tahoma12px), TEXT_OUTLINE, c_color(255, 255, 255));

	if (g_ui->get_menu_state())
	{
		if (m_ctx_menu_open)
		{
			for (int i = 0; i < m_vars.size(); i++)
			{
				vec2 ctx_list_min(m_rect_min.x + 10, m_rect_min.y + 20 + (i * 18));
				vec2 ctx_list_max(m_rect_max.x - 10, 17);

				bool is_hovered = g_input->mouse_is_hovered(ctx_list_min, ctx_list_min + ctx_list_max);
				bool is_actived = g_var->get_as<bool>(m_vars[i].second).value();

				g_renderer->rect_fill(ctx_list_min, ctx_list_max, c_color(20, 20, 20, menu_shadow_alpha - 20));
				g_renderer->rect_fill(ctx_list_min.x + 3, ctx_list_min.y + 3, 2, 11,
					is_actived ? c_color(V_UI_COL) : c_color(164, 164, 164));

				g_font->draw_string(m_vars[i].first, ctx_list_min.x + 9, ctx_list_min.y + 2, FONT(Tahoma12px), TEXT_OUTLINE,
					is_hovered ? c_color(V_UI_COL) : c_color(255, 255, 255));
			}
		}
	}
	else
		m_ctx_menu_open = false;

	int x = 0;
	auto push_item = [&](const std::string& label, ID3DXFont* font, bool v) {
		const auto string_width = g_font->get_text_width(label, font);

		g_renderer->rect_fill(m_rect_min.x - string_width - x - 13, m_rect_min.y, string_width + 11, 17, c_color(20, 20, 20, menu_shadow_alpha));
		g_renderer->rect_fill(m_rect_min.x - string_width - x - 10, m_rect_min.y + 3, 2, 11, v ? c_color(V_UI_COL) : c_color(164, 164, 164));

		g_font->draw_string(label, m_rect_min.x - string_width - x - 6, m_rect_min.y + 2, font, TEXT_OUTLINE, c_color(255, 255, 255));

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
			if (GLOBAL(b_flags[BF_BOMB_PLANTED]) && GLOBAL(i_flags[IF_BOMB_TIMER]) > 0)
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

	if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_TIME).value())
	{
		push_item(" " + Helpers::get_current_time(), FONT(Tahoma12px), true);
	}
	
	if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS_TIME_PLAYING).value())
	{
		char buffer[64]{};

		if (GLOBAL(i_flags[IF_HOURS_IN_GAME]) > 0)
		{
			if (GLOBAL(i_flags[IF_HOURS_IN_GAME]) >= 24)
				sprintf(buffer, " %ih Playing", GLOBAL(i_flags[IF_HOURS_IN_GAME]));

			else
				sprintf(buffer, " %ih %im Playing", GLOBAL(i_flags[IF_HOURS_IN_GAME]), GLOBAL(i_flags[IF_MINUTES_IN_GAME]));
		}
		else
			sprintf(buffer, " %im %is Playing", GLOBAL(i_flags[IF_MINUTES_IN_GAME]), GLOBAL(i_flags[IF_SECONDS_IN_GAME]));

		push_item(buffer, FONT(Tahoma12px), true);
	}
}

void status_bar_t::handle_click(bool left_click, bool right_click)
{
	if (g_input->mouse_is_hovered(m_rect_min, m_rect_min + m_rect_max))
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

			if (g_input->mouse_is_hovered(ctx_list_min, ctx_list_min + ctx_list_max) && left_click)
				g_var->set(m_vars[i].second, !g_var->get_as<bool>(m_vars[i].second).value());
		}

		if ((!g_input->mouse_is_hovered(m_rect_min, m_rect_min + m_rect_max) &&
			 !g_input->mouse_is_hovered(ctx_content_min, ctx_content_min + ctx_content_max)) && (left_click || right_click))
			m_ctx_menu_open = false;
	}
}

void status_bar_t::handle_move(UINT m)
{
	if (!m_rect_min && !m_rect_max)
		return;

	static draggable_object_t bar_drag_obj{ m_rect_min.x, m_rect_min.y, m_rect_max.x, m_rect_max.y };

	if (g_input->move_object(bar_drag_obj, m))
	{
		m_rect_min.y = bar_drag_obj.get_y();
		g_var->set(V_VISUALS_INTERFACE_STATUS_POS_Y, m_rect_min.y);
	}

	else
		bar_drag_obj.set_y(m_rect_min.y);
}