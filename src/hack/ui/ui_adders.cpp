#include "ui_adders.h"

void ui_adders::add(const std::wstring& name, bool space)
{
	menu_entry[m_entry_size].m_name = name;
	menu_entry[m_entry_size].m_space = space;

	++m_entry_size;
}

void ui_adders::add_bool(const std::wstring& name, const std::string& var, bool space)
{
	subm_entry[s_entry_size].m_name = name;
	subm_entry[s_entry_size].m_var = var;

	subm_entry[s_entry_size].m_state = UI_BOOL_STATE;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space)
{
	subm_entry[s_entry_size].m_name = name;
	subm_entry[s_entry_size].m_var = var;

	subm_entry[s_entry_size].m_int_min = min;
	subm_entry[s_entry_size].m_int_max = max;

	subm_entry[s_entry_size].m_int_step = step;

	subm_entry[s_entry_size].m_state = UI_INT_STATE;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space)
{
	subm_entry[s_entry_size].m_name = name;
	subm_entry[s_entry_size].m_var = var;

	subm_entry[s_entry_size].m_float_min = min;
	subm_entry[s_entry_size].m_float_max = max;

	subm_entry[s_entry_size].m_float_step = step;

	subm_entry[s_entry_size].m_state = UI_FLOAT_STATE;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space)
{
	subm_entry[s_entry_size].m_name = name;
	subm_entry[s_entry_size].m_var = var;

	subm_entry[s_entry_size].m_int_min = 0;
	subm_entry[s_entry_size].m_int_max = items.size() - 1;

	subm_entry[s_entry_size].m_int_step = 1;

	subm_entry[s_entry_size].m_state = UI_ITEM_STATE;
	subm_entry[s_entry_size].m_items = items;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_key(const std::wstring& name, const std::string& var, bool space)
{
	subm_entry[s_entry_size].m_name = name;
	subm_entry[s_entry_size].m_var = var;

	subm_entry[s_entry_size].m_state = UI_KEY_STATE;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_function(const std::wstring& name, std::function<void(void)> fn, bool space)
{
	subm_entry[s_entry_size].m_name = name;

	subm_entry[s_entry_size].m_state = UI_FUNCTION_STATE;
	subm_entry[s_entry_size].m_fn = fn;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_tab(const std::wstring& name, bool space)
{
	subm_entry[s_entry_size].m_name = name;

	subm_entry[s_entry_size].m_state = UI_TAB_STATE;

	subm_entry[s_entry_size].m_space = space;

	++s_entry_size;
}

void ui_adders::add_sub_bool(const std::wstring& name, const std::string& var, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;
	ssubm_entry[ss_entry_size].m_var = var;

	ssubm_entry[ss_entry_size].m_state = UI_BOOL_STATE;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;
	ssubm_entry[ss_entry_size].m_var = var;

	ssubm_entry[ss_entry_size].m_int_min = min;
	ssubm_entry[ss_entry_size].m_int_max = max;

	ssubm_entry[ss_entry_size].m_int_step = step;

	ssubm_entry[ss_entry_size].m_state = UI_INT_STATE;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;
	ssubm_entry[ss_entry_size].m_var = var;

	ssubm_entry[ss_entry_size].m_float_min = min;
	ssubm_entry[ss_entry_size].m_float_max = max;

	ssubm_entry[ss_entry_size].m_float_step = step;

	ssubm_entry[ss_entry_size].m_state = UI_FLOAT_STATE;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;
	ssubm_entry[ss_entry_size].m_var = var;

	ssubm_entry[ss_entry_size].m_int_min = 0;
	ssubm_entry[ss_entry_size].m_int_max = items.size() - 1;

	ssubm_entry[ss_entry_size].m_int_step = 1;

	ssubm_entry[ss_entry_size].m_state = UI_ITEM_STATE;
	ssubm_entry[ss_entry_size].m_items = items;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_key(const std::wstring& name, const std::string& var, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;
	ssubm_entry[ss_entry_size].m_var = var;

	ssubm_entry[ss_entry_size].m_state = UI_KEY_STATE;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_function(const std::wstring& name, std::function<void(void)> fn, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;

	ssubm_entry[ss_entry_size].m_state = UI_FUNCTION_STATE;
	ssubm_entry[ss_entry_size].m_fn = fn;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_tab(const std::wstring& name, bool space)
{
	ssubm_entry[ss_entry_size].m_name = name;

	ssubm_entry[ss_entry_size].m_state = UI_TAB_STATE;

	ssubm_entry[ss_entry_size].m_space = space;

	++ss_entry_size;
}

void ui_adders::add_sub_sub_bool(const std::wstring& name, const std::string& var, bool space)
{
	sssubm_entry[sss_entry_size].m_name = name;
	sssubm_entry[sss_entry_size].m_var = var;

	sssubm_entry[sss_entry_size].m_state = UI_BOOL_STATE;

	sssubm_entry[sss_entry_size].m_space = space;

	++sss_entry_size;
}

void ui_adders::add_sub_sub_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space)
{
	sssubm_entry[sss_entry_size].m_name = name;
	sssubm_entry[sss_entry_size].m_var = var;

	sssubm_entry[sss_entry_size].m_int_min = min;
	sssubm_entry[sss_entry_size].m_int_max = max;

	sssubm_entry[sss_entry_size].m_int_step = step;

	sssubm_entry[sss_entry_size].m_state = UI_INT_STATE;

	sssubm_entry[sss_entry_size].m_space = space;

	++sss_entry_size;
}

void ui_adders::add_sub_sub_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space)
{
	sssubm_entry[sss_entry_size].m_name = name;
	sssubm_entry[sss_entry_size].m_var = var;

	sssubm_entry[sss_entry_size].m_float_min = min;
	sssubm_entry[sss_entry_size].m_float_max = max;

	sssubm_entry[sss_entry_size].m_float_step = step;

	sssubm_entry[sss_entry_size].m_state = UI_FLOAT_STATE;

	sssubm_entry[sss_entry_size].m_space = space;

	++sss_entry_size;
}

void ui_adders::add_sub_sub_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space)
{
	sssubm_entry[sss_entry_size].m_name = name;
	sssubm_entry[sss_entry_size].m_var = var;

	sssubm_entry[sss_entry_size].m_int_min = 0;
	sssubm_entry[sss_entry_size].m_int_max = items.size() - 1;

	sssubm_entry[sss_entry_size].m_int_step = 1;

	sssubm_entry[sss_entry_size].m_state = UI_ITEM_STATE;
	sssubm_entry[sss_entry_size].m_items = items;

	sssubm_entry[sss_entry_size].m_space = space;

	++sss_entry_size;
}

void ui_adders::add_sub_sub_key(const std::wstring& name, const std::string& var, bool space)
{
	sssubm_entry[sss_entry_size].m_name = name;
	sssubm_entry[sss_entry_size].m_var = var;

	sssubm_entry[sss_entry_size].m_state = UI_KEY_STATE;

	sssubm_entry[sss_entry_size].m_space = space;

	++sss_entry_size;
}

void ui_adders::add_sub_sub_function(const std::wstring& name, std::function<void(void)> fn, bool space)
{
	sssubm_entry[sss_entry_size].m_name = name;

	sssubm_entry[sss_entry_size].m_state = UI_FUNCTION_STATE;
	sssubm_entry[sss_entry_size].m_fn = fn;

	sssubm_entry[sss_entry_size].m_space = space;

	++sss_entry_size;
}