#include "ui_input.h"

void ui_input::handle_toggle_keys(unsigned int k)
{
	if (k == g_vars.get_as<int>("keys->on_toggle->ui").value())
		g.m_ui_opened = !g.m_ui_opened;

	if (k == g_vars.get_as<int>("keys->on_toggle->aimbot").value())
		g_vars.set("aimbot->enabled", !g_vars.get_as<bool>("aimbot->enabled").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->triggerbot").value())
		g_vars.set("triggerbot->enabled", !g_vars.get_as<bool>("triggerbot->enabled").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->thirdperson").value())
		g_vars.set("misc->visual->thirdperson", !g_vars.get_as<bool>("misc->visual->thirdperson").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->blockbot").value())
		g_vars.set("misc->movement->blockbot", !g_vars.get_as<bool>("misc->movement->blockbot").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->panic").value())
		g.m_panic_mode = !g.m_panic_mode;
}

void ui_input::handle_input(unsigned int k)
{
	if (!s_opened[UI_SUB_POS])
	{
		if (k == VK_RIGHT)
		{
			s_entry_position = 0;
			s_opened[UI_SUB_POS] = true;
		}
		else if (k == VK_UP)
		{
			if (m_entry_position > 0)
				--m_entry_position;
		}
		else if (k == VK_DOWN)
		{
			if (m_entry_position < (m_entry_size - 1))
				++m_entry_position;
		}
	}
	else
	{
		if (!s_opened[UI_SUB_SUB_POS])
		{
			if (k == VK_UP)
			{
				if (!subm_entry[s_entry_position].m_key_hold)
				{
					if (s_entry_position > 0)
						--s_entry_position;
					else
						s_opened[UI_SUB_POS] = false;
				}
			}
			else if (k == VK_DOWN)
			{
				if (!subm_entry[s_entry_position].m_key_hold)
				{
					if (s_entry_position < (s_entry_size - 1))
						++s_entry_position;
				}
			}
			else if (k == VK_LEFT)
			{
				if (subm_entry[s_entry_position].m_state == UI_BOOL_STATE)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, false);
				}
				else if (subm_entry[s_entry_position].m_state == UI_INT_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value -= subm_entry[s_entry_position].m_int_step);

					if (value < subm_entry[s_entry_position].m_int_min)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_min);
				}
				else if (subm_entry[s_entry_position].m_state == UI_FLOAT_STATE)
				{
					auto value = g_vars.get_as<float>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value -= subm_entry[s_entry_position].m_float_step);

					if (value < subm_entry[s_entry_position].m_float_min)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_float_min);
				}
				else if (subm_entry[s_entry_position].m_state == UI_ITEM_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value -= subm_entry[s_entry_position].m_int_step);

					if (value < subm_entry[s_entry_position].m_int_min)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_min);
				}
			}
			else if (k == VK_RIGHT)
			{
				if (subm_entry[s_entry_position].m_state == UI_BOOL_STATE)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, true);
				}
				else if (subm_entry[s_entry_position].m_state == UI_INT_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value += subm_entry[s_entry_position].m_int_step);

					if (value > subm_entry[s_entry_position].m_int_max)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_max);
				}
				else if (subm_entry[s_entry_position].m_state == UI_FLOAT_STATE)
				{
					auto value = g_vars.get_as<float>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value += subm_entry[s_entry_position].m_float_step);

					if (value > subm_entry[s_entry_position].m_float_max)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_float_max);
				}
				else if (subm_entry[s_entry_position].m_state == UI_ITEM_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value += subm_entry[s_entry_position].m_int_step);

					if (value > subm_entry[s_entry_position].m_int_max)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_max);
				}
				else if (subm_entry[s_entry_position].m_state == UI_TAB_STATE)
				{
					ss_entry_position = 0;
					s_opened[UI_SUB_SUB_POS] = true;
				}
			}
			else if (k == VK_BACK)
			{
				if (subm_entry[s_entry_position].m_state == UI_KEY_STATE &&
					subm_entry[s_entry_position].m_key_hold)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, 0);
					subm_entry[s_entry_position].m_key_hold = false;
				}
				else
					s_opened[UI_SUB_POS] = false;
			}
			else if (k == VK_RETURN)
			{
				if (subm_entry[s_entry_position].m_state == UI_KEY_STATE)
				{
					subm_entry[s_entry_position].m_key_hold = true;
				}
				else if (subm_entry[s_entry_position].m_state == UI_FUNCTION_STATE)
				{
					subm_entry[s_entry_position].m_fn();
				}
			}
			else
			{
				if (subm_entry[s_entry_position].m_state == UI_KEY_STATE &&
					subm_entry[s_entry_position].m_key_hold)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, (int)k);
					subm_entry[s_entry_position].m_key_hold = false;
				}
			}
		}
		else
		{
			if (!s_opened[UI_SUB_SUB_SUB_POS])
			{
				if (k == VK_UP)
				{
					if (!ssubm_entry[ss_entry_position].m_key_hold)
					{
						if (ss_entry_position > 0)
							--ss_entry_position;
						else
							s_opened[UI_SUB_SUB_POS] = false;
					}
				}
				else if (k == VK_DOWN)
				{
					if (!ssubm_entry[ss_entry_position].m_key_hold)
					{
						if (ss_entry_position < (ss_entry_size - 1))
							++ss_entry_position;
					}
				}
				else if (k == VK_LEFT)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, false);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value -= ssubm_entry[ss_entry_position].m_int_step);

						if (value < ssubm_entry[ss_entry_position].m_int_min)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_min);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value -= ssubm_entry[ss_entry_position].m_float_step);

						if (value < ssubm_entry[ss_entry_position].m_float_min)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_float_min);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value -= ssubm_entry[ss_entry_position].m_int_step);

						if (value < ssubm_entry[ss_entry_position].m_int_min)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_min);
					}
				}
				else if (k == VK_RIGHT)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, true);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value += ssubm_entry[ss_entry_position].m_int_step);

						if (value > ssubm_entry[ss_entry_position].m_int_max)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_max);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value += ssubm_entry[ss_entry_position].m_float_step);

						if (value > ssubm_entry[ss_entry_position].m_float_max)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_float_max);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value += ssubm_entry[ss_entry_position].m_int_step);

						if (value > ssubm_entry[ss_entry_position].m_int_max)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_max);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_TAB_STATE)
					{
						sss_entry_position = 0;
						s_opened[UI_SUB_SUB_SUB_POS] = true;
					}
				}
				else if (k == VK_BACK)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_KEY_STATE &&
						ssubm_entry[ss_entry_position].m_key_hold)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, 0);
						ssubm_entry[ss_entry_position].m_key_hold = false;
					}
					else
						s_opened[UI_SUB_SUB_POS] = false;
				}
				else if (k == VK_RETURN)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_KEY_STATE)
					{
						ssubm_entry[ss_entry_position].m_key_hold = true;
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_FUNCTION_STATE)
					{
						ssubm_entry[ss_entry_position].m_fn();
					}
				}
				else
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_KEY_STATE &&
						ssubm_entry[ss_entry_position].m_key_hold)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, (int)k);
						ssubm_entry[ss_entry_position].m_key_hold = false;
					}
				}
			}
			else
			{
				if (k == VK_UP)
				{
					if (!sssubm_entry[sss_entry_position].m_key_hold)
					{
						if (sss_entry_position > 0)
							--sss_entry_position;
						else
							s_opened[UI_SUB_SUB_SUB_POS] = false;
					}
				}
				else if (k == VK_DOWN)
				{
					if (!sssubm_entry[sss_entry_position].m_key_hold)
					{
						if (sss_entry_position < (sss_entry_size - 1))
							++sss_entry_position;
					}
				}
				else if (k == VK_LEFT)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, false);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value -= sssubm_entry[sss_entry_position].m_int_step);

						if (value < sssubm_entry[sss_entry_position].m_int_min)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_min);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value -= sssubm_entry[sss_entry_position].m_float_step);

						if (value < sssubm_entry[sss_entry_position].m_float_min)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_float_min);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value -= sssubm_entry[sss_entry_position].m_int_step);

						if (value < sssubm_entry[sss_entry_position].m_int_min)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_min);
					}
				}
				else if (k == VK_RIGHT)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, true);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value += sssubm_entry[sss_entry_position].m_int_step);

						if (value > sssubm_entry[sss_entry_position].m_int_max)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_max);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value += sssubm_entry[sss_entry_position].m_float_step);

						if (value > sssubm_entry[sss_entry_position].m_float_max)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_float_max);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value += sssubm_entry[sss_entry_position].m_int_step);

						if (value > sssubm_entry[sss_entry_position].m_int_max)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_max);
					}
				}
				else if (k == VK_BACK)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_KEY_STATE &&
						sssubm_entry[sss_entry_position].m_key_hold)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, 0);
						sssubm_entry[sss_entry_position].m_key_hold = false;
					}
					else
						s_opened[UI_SUB_SUB_SUB_POS] = false;
				}
				else if (k == VK_RETURN)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_KEY_STATE)
					{
						sssubm_entry[sss_entry_position].m_key_hold = true;
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_FUNCTION_STATE)
					{
						sssubm_entry[sss_entry_position].m_fn();
					}
				}
				else
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_KEY_STATE &&
						sssubm_entry[sss_entry_position].m_key_hold)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, (int)k);
						sssubm_entry[sss_entry_position].m_key_hold = false;
					}
				}
			}
		}
	}
}

bool ui_input::get_menu_state()
{
	return g.m_ui_opened;
}