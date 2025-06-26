#include "ui.h"

#include "globals.h"

void ui::handle_toggle_keys(unsigned int k)
{
	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_UI).value())
		m_opened = !m_opened;

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_AIMBOT).value())
		g_vars.set(V_AIMBOT_ENABLED, !g_vars.get_as<bool>(V_AIMBOT_ENABLED).value());

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_TRIGGERBOT).value())
		g_vars.set(V_TRIGGERBOT_ENABLED, !g_vars.get_as<bool>(V_TRIGGERBOT_ENABLED).value());

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_THIRDPERSON).value())
		g_vars.set(V_MISC_VISUAL_THIRDPERSON, !g_vars.get_as<bool>(V_MISC_VISUAL_THIRDPERSON).value());

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_PANIC).value())
		GLOBAL(b_flags[BF_PANIC]) = !GLOBAL(b_flags[BF_PANIC]);
}

void ui::handle_input(unsigned int k)
{
	if (!s_opened[UI_SUB_POS])
	{
		if (k == VK_RIGHT)
		{
			s_entry_pos[UI_SUB_POS] = 0;
			s_opened[UI_SUB_POS] = true;
		}
		else if (k == VK_UP)
		{
			if (m_entry_pos > 0)
				--m_entry_pos;
		}
		else if (k == VK_DOWN)
		{
			if (m_entry_pos < (m_entry_sz - 1))
				++m_entry_pos;
		}
	}
	else
	{
		if (!s_opened[UI_SUB_SUB_POS])
		{
			if (k == VK_UP)
			{
				if (!s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold)
				{
					if (s_entry_pos[UI_SUB_POS] > 0)
						--s_entry_pos[UI_SUB_POS];
					else
						s_opened[UI_SUB_POS] = false;
				}
			}
			else if (k == VK_DOWN)
			{
				if (!s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold)
				{
					if (s_entry_pos[UI_SUB_POS] < (s_entry_sz[UI_SUB_POS] - 1))
						++s_entry_pos[UI_SUB_POS];
				}
			}
			else if (k == VK_LEFT)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_BOOL_STATE)
				{
					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, false);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_INT_STATE)
				{
					auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var).value();

					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_step);

					if (value < s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_min)
						g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_min);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_FLOAT_STATE)
				{
					auto value = g_vars.get_as<float>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var).value();

					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_float_step);

					if (value < s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_float_min)
						g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_float_min);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_ITEM_STATE)
				{
					auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var).value();

					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_step);

					if (value < s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_min)
						g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_min);
				}
			}
			else if (k == VK_RIGHT)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_BOOL_STATE)
				{
					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, true);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_INT_STATE)
				{
					auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var).value();

					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_step);

					if (value > s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_max)
						g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_max);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_FLOAT_STATE)
				{
					auto value = g_vars.get_as<float>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var).value();

					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_float_step);

					if (value > s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_float_max)
						g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_float_max);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_ITEM_STATE)
				{
					auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var).value();

					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_step);

					if (value > s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_max)
						g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_int_max);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_TAB_STATE)
				{
					s_entry_pos[UI_SUB_SUB_POS] = 0;
					s_opened[UI_SUB_SUB_POS] = true;
				}
			}
			else if (k == VK_BACK)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_KEY_STATE &&
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold)
				{
					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, 0);
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold = false;
				}
				else
					s_opened[UI_SUB_POS] = false;
			}
			else if (k == VK_RETURN)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_KEY_STATE)
				{
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold = true;
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_FUNCTION_STATE)
				{
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_fn();
				}
			}
			else
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_state == UI_KEY_STATE &&
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold)
				{
					g_vars.set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_var, static_cast<int>(k));
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].m_key_hold = false;
				}
			}
		}
		else
		{
			if (!s_opened[UI_SUB_SUB_SUB_POS])
			{
				if (k == VK_UP)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_POS] > 0)
							--s_entry_pos[UI_SUB_SUB_POS];
						else
							s_opened[UI_SUB_SUB_POS] = false;
					}
				}
				else if (k == VK_DOWN)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_POS] < (s_entry_sz[UI_SUB_SUB_POS] - 1))
							++s_entry_pos[UI_SUB_SUB_POS];
					}
				}
				else if (k == VK_LEFT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_BOOL_STATE)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, false);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_min)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_float_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_float_min)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_float_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_min)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_min);
					}
				}
				else if (k == VK_RIGHT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_BOOL_STATE)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, true);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_max)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_float_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_float_max)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_float_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_max)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_int_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_TAB_STATE)
					{
						s_entry_pos[UI_SUB_SUB_SUB_POS] = 0;
						s_opened[UI_SUB_SUB_SUB_POS] = true;
					}
				}
				else if (k == VK_BACK)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, 0);
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold = false;
					}
					else
						s_opened[UI_SUB_SUB_POS] = false;
				}
				else if (k == VK_RETURN)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_KEY_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold = true;
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_FUNCTION_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_fn();
					}
				}
				else
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_var, static_cast<int>(k));
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].m_key_hold = false;
					}
				}
			}
			else
			{
				if (k == VK_UP)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_SUB_POS] > 0)
							--s_entry_pos[UI_SUB_SUB_SUB_POS];
						else
							s_opened[UI_SUB_SUB_SUB_POS] = false;
					}
				}
				else if (k == VK_DOWN)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_SUB_POS] < (s_entry_sz[UI_SUB_SUB_SUB_POS] - 1))
							++s_entry_pos[UI_SUB_SUB_SUB_POS];
					}
				}
				else if (k == VK_LEFT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_BOOL_STATE)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, false);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_min)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_float_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_float_min)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_float_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, value -= s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_min)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_min);
					}
				}
				else if (k == VK_RIGHT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_BOOL_STATE)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, true);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_max)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_float_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_float_max)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_float_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var).value();

						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, value += s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_max)
							g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_int_max);
					}
				}
				else if (k == VK_BACK)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, 0);
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold = false;
					}
					else
						s_opened[UI_SUB_SUB_SUB_POS] = false;
				}
				else if (k == VK_RETURN)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_KEY_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold = true;
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_FUNCTION_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_fn();
					}
				}
				else
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold)
					{
						g_vars.set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_var, static_cast<int>(k));
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].m_key_hold = false;
					}
				}
			}
		}
	}
}