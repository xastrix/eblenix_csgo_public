#pragma once

#include <string>
#include <vector>
#include <functional>

enum _ui_states {
	UI_NONE_STATE,
	UI_BOOL_STATE,
	UI_INT_STATE,
	UI_FLOAT_STATE,
	UI_FUNCTION_STATE,
	UI_ITEM_STATE,
	UI_KEY_STATE,
	UI_TAB_STATE,
};

enum _ui_subs {
	UI_SUB_POS,
	UI_SUB_SUB_POS,
	UI_SUB_SUB_SUB_POS,
	maxUISubs,
};

struct m_entry
{
	std::wstring              m_name{};
	bool                      m_space{};
};

struct s_entry
{
	std::wstring              m_name{};
	std::string               m_var{};

	float                     m_float_step{},
		                      m_float_min{},
		                      m_float_max{};

	int                       m_int_step{},
		                      m_int_min{},
		                      m_int_max{};

	bool                      m_space{},
		                      m_key_hold{};

	_ui_states                m_state{};

	std::function<void(void)> m_fn{};
	std::vector<std::wstring> m_items{};
};

struct ui_globals {
	bool                      s_opened[maxUISubs]{};

	int                       m_entry_position{};
	int                       m_entry_size{};
	int                       s_entry_position{};
	int                       s_entry_size{};
	int                       ss_entry_position{};
	int                       ss_entry_size{};
	int                       sss_entry_position{};
	int                       sss_entry_size{};

	m_entry                   menu_entry[10]{};
	s_entry                   subm_entry[16]{};
	s_entry                   ssubm_entry[16]{};
	s_entry                   sssubm_entry[16]{};

	std::vector<std::wstring> cfg_list{ L"1", L"2", L"3", L"4", L"5" };
};