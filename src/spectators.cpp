#include "hud.h"

#include "interfaces.h"
#include "input.h"
#include "helpers.h"

void spectators_t::run()
{
	think();
	draw();
}

void spectators_t::think()
{
	vec2 screen_size = g_renderer->get_screen_size();

	m_rect_min = vec2(screen_size.x - 105, g_var->get_as<float>(V_VISUALS_INTERFACE_SPECTATORS_POS_Y).value());
	m_rect_max = vec2(screen_size.x, 18);

	m_rect_min.y = std::max(0.0f, std::min(m_rect_min.y, screen_size.y - m_rect_max.y));
}

void spectators_t::draw()
{
	const auto menu_shadow_alpha = g_var->get_as<int>(V_UI_COL_A).value();

	if (!g_cs->m_engine->is_in_game())
		return;

	g_renderer->rect_fill(m_rect_min, m_rect_max, c_color(20, 20, 20, menu_shadow_alpha));
	g_renderer->rect_fill(m_rect_min.x + 3, m_rect_min.y + 3, 2, 12, c_color(V_UI_COL));

	g_font->draw_string("Spectators", m_rect_min.x + 9, m_rect_min.y + 2,
		FONT(Tahoma12px), TEXT_OUTLINE, c_color(255, 255, 255));

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

		g_renderer->rect_fill(m_rect_min.x, m_rect_min.y + (19 * y), m_rect_max.x, m_rect_max.y, c_color(20, 20, 20, menu_shadow_alpha));
		g_renderer->rect_fill(m_rect_min.x + 3, m_rect_min.y + 3 + (19 * y), 2, 12, c_color(164, 164, 164));

		player_info_t info;
		g_cs->m_engine->get_player_info(i, &info);

		auto player_name = Helpers::stws(std::string{ info.m_player_name });

		if (player_name.length() > 14)
			player_name = player_name.substr(0, 14) + L"...";

		g_font->draw_stringW(player_name, m_rect_min.x + 9, m_rect_min.y + 2 + (19 * y),
			FONT(Tahoma12px), TEXT_OUTLINE, c_color(255, 255, 255));

		y++;
	}
}

void spectators_t::handle_move(UINT m)
{
	if (!m_rect_min && !m_rect_max)
		return;

	static draggable_object_t spec_drag_obj{ m_rect_min.x, m_rect_min.y, m_rect_max.x, m_rect_max.y };

	if (g_input->move_object(spec_drag_obj, m))
	{
		m_rect_min.y = spec_drag_obj.get_y();
		g_var->set(V_VISUALS_INTERFACE_SPECTATORS_POS_Y, m_rect_min.y);
	}

	else
		spec_drag_obj.set_y(m_rect_min.y);
}