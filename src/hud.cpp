#include "hud.h"

void c_hud::run()
{
	if (!g_var->get_as<bool>(V_VISUALS_ENABLED).value())
		return;

	if (g_var->get_as<bool>(V_VISUALS_INTERFACE_SPECTATORS).value())
		spectators_t::get_instance().run();

	if (g_var->get_as<bool>(V_VISUALS_INTERFACE_STATUS).value())
		status_bar_t::get_instance().run();
}

void c_hud::process_message(UINT m)
{
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

void c_hud::notify_hud(const std::string& msg, color_t rect_c)
{
	const auto string_font = FONT(Tahoma12px);
	const auto string_width = g_font->get_text_width(msg, string_font);

	vec2 screen_size = g_renderer->get_screen_size();

	g_renderer->rect_fill(screen_size.x - string_width - 15, 10, screen_size.x, 17, color_t(20, 20, 20, 140));
	g_renderer->rect_fill(screen_size.x - string_width - 12, 13, 2, 11, rect_c);

	g_font->draw_string(msg, screen_size.x - string_width - 5, 12, string_font, TEXT_OUTLINE, color_t(255, 255, 255));
}