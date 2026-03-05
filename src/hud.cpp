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