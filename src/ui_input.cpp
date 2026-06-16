#include "ui.h"

#include "globals.h"

void c_ui::handle_toggle_keys(unsigned int vk)
{
	if (vk == g_var->get_as<int>(V_KEYS_ON_TOGGLE_UI).value())
		m_opened = !m_opened;

	if (vk == g_var->get_as<int>(V_KEYS_ON_TOGGLE_AIMBOT).value())
		g_var->set(V_AIMBOT_ENABLED, !g_var->get_as<bool>(V_AIMBOT_ENABLED).value());

	if (vk == g_var->get_as<int>(V_KEYS_ON_TOGGLE_TRIGGERBOT).value())
		g_var->set(V_TRIGGERBOT_ENABLED, !g_var->get_as<bool>(V_TRIGGERBOT_ENABLED).value());

	if (vk == g_var->get_as<int>(V_KEYS_ON_TOGGLE_THIRDPERSON).value())
		g_var->set(V_MISC_VISUAL_THIRDPERSON, !g_var->get_as<bool>(V_MISC_VISUAL_THIRDPERSON).value());

	if (vk == g_var->get_as<int>(V_KEYS_ON_TOGGLE_PANIC).value())
		GLOBAL(b_flags[BF_PANIC]) = !GLOBAL(b_flags[BF_PANIC]);
}

void c_ui::handle_input(unsigned int vk)
{
	if (!m_sopened[UI_SUB_POS])
	{
		if (vk == VK_RIGHT)
		{
			s_entry_pos[UI_SUB_POS] = 0;
			m_sopened[UI_SUB_POS] = true;
		}
		else if (vk == VK_UP)
		{
			if (m_entry_pos > 0)
				--m_entry_pos;
		}
		else if (vk == VK_DOWN)
		{
			if (m_entry_pos < (m_entry_sz - 1))
				++m_entry_pos;
		}
	}
	else
	{
		if (!m_sopened[UI_SUB_SUB_POS])
		{
			if (vk == VK_UP)
			{
				if (!s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold)
				{
					if (s_entry_pos[UI_SUB_POS] > 0)
						--s_entry_pos[UI_SUB_POS];
					else
						m_sopened[UI_SUB_POS] = false;
				}
			}
			else if (vk == VK_DOWN)
			{
				if (!s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold)
				{
					if (s_entry_pos[UI_SUB_POS] < (s_entry_sz[UI_SUB_POS] - 1))
						++s_entry_pos[UI_SUB_POS];
				}
			}
			else if (vk == VK_LEFT)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_BOOL_STATE)
				{
					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, false);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_INT_STATE)
				{
					auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var).value();

					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_step);

					if (value < s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_min)
						g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_min);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_FLOAT_STATE)
				{
					auto value = g_var->get_as<float>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var).value();

					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].float_step);

					if (value < s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].float_min)
						g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].float_min);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_ITEM_STATE)
				{
					auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var).value();

					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_step);

					if (value < s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_min)
						g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_min);
				}
			}
			else if (vk == VK_RIGHT)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_BOOL_STATE)
				{
					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, true);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_INT_STATE)
				{
					auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var).value();

					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_step);

					if (value > s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_max)
						g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_max);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_FLOAT_STATE)
				{
					auto value = g_var->get_as<float>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var).value();

					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].float_step);

					if (value > s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].float_max)
						g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].float_max);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_ITEM_STATE)
				{
					auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var).value();

					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_step);

					if (value > s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_max)
						g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].int_max);
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_TAB_STATE)
				{
					s_entry_pos[UI_SUB_SUB_POS] = 0;
					m_sopened[UI_SUB_SUB_POS] = true;
				}
			}
			else if (vk == VK_BACK)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_KEY_STATE &&
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold)
				{
					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, 0);
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold = false;
				}
				else
					m_sopened[UI_SUB_POS] = false;
			}
			else if (vk == VK_RETURN)
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_KEY_STATE)
				{
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold = true;
				}
				else if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_FUNCTION_STATE)
				{
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].fn();
				}
			}
			else
			{
				if (s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].state == UI_KEY_STATE &&
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold)
				{
					g_var->set(s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].var, static_cast<int>(vk));
					s_entries[s_entry_pos[UI_SUB_POS]][UI_SUB_POS].key_hold = false;
				}
			}
		}
		else
		{
			if (!m_sopened[UI_SUB_SUB_SUB_POS])
			{
				if (vk == VK_UP)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_POS] > 0)
							--s_entry_pos[UI_SUB_SUB_POS];
						else
							m_sopened[UI_SUB_SUB_POS] = false;
					}
				}
				else if (vk == VK_DOWN)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_POS] < (s_entry_sz[UI_SUB_SUB_POS] - 1))
							++s_entry_pos[UI_SUB_SUB_POS];
					}
				}
				else if (vk == VK_LEFT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_BOOL_STATE)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, false);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_INT_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_min)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_FLOAT_STATE)
					{
						auto value = g_var->get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].float_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].float_min)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].float_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_ITEM_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_min)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_min);
					}
				}
				else if (vk == VK_RIGHT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_BOOL_STATE)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, true);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_INT_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_max)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_FLOAT_STATE)
					{
						auto value = g_var->get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].float_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].float_max)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].float_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_ITEM_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_max)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].int_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_TAB_STATE)
					{
						s_entry_pos[UI_SUB_SUB_SUB_POS] = 0;
						m_sopened[UI_SUB_SUB_SUB_POS] = true;
					}
				}
				else if (vk == VK_BACK)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, 0);
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold = false;
					}
					else
						m_sopened[UI_SUB_SUB_POS] = false;
				}
				else if (vk == VK_RETURN)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_KEY_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold = true;
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_FUNCTION_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].fn();
					}
				}
				else
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].var, static_cast<int>(vk));
						s_entries[s_entry_pos[UI_SUB_SUB_POS]][UI_SUB_SUB_POS].key_hold = false;
					}
				}
			}
			else
			{
				if (vk == VK_UP)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_SUB_POS] > 0)
							--s_entry_pos[UI_SUB_SUB_SUB_POS];
						else
							m_sopened[UI_SUB_SUB_SUB_POS] = false;
					}
				}
				else if (vk == VK_DOWN)
				{
					if (!s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold)
					{
						if (s_entry_pos[UI_SUB_SUB_SUB_POS] < (s_entry_sz[UI_SUB_SUB_SUB_POS] - 1))
							++s_entry_pos[UI_SUB_SUB_SUB_POS];
					}
				}
				else if (vk == VK_LEFT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_BOOL_STATE)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, false);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_INT_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_min)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_FLOAT_STATE)
					{
						auto value = g_var->get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].float_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].float_min)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].float_min);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_ITEM_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, value -= s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_step);

						if (value < s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_min)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_min);
					}
				}
				else if (vk == VK_RIGHT)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_BOOL_STATE)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, true);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_INT_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_max)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_FLOAT_STATE)
					{
						auto value = g_var->get_as<float>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].float_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].float_max)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].float_max);
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_ITEM_STATE)
					{
						auto value = g_var->get_as<int>(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var).value();

						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, value += s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_step);

						if (value > s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_max)
							g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].int_max);
					}
				}
				else if (vk == VK_BACK)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, 0);
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold = false;
					}
					else
						m_sopened[UI_SUB_SUB_SUB_POS] = false;
				}
				else if (vk == VK_RETURN)
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_KEY_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold = true;
					}
					else if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_FUNCTION_STATE)
					{
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].fn();
					}
				}
				else
				{
					if (s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].state == UI_KEY_STATE &&
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold)
					{
						g_var->set(s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].var, static_cast<int>(vk));
						s_entries[s_entry_pos[UI_SUB_SUB_SUB_POS]][UI_SUB_SUB_SUB_POS].key_hold = false;
					}
				}
			}
		}
	}
}