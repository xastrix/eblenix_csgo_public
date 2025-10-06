#pragma once

#include <string>
#include <vector>
#include <functional>
#include <sprites.hpp>

#include "../render_manager/render_manager.h"
#include "../render_manager/sprite_manager/sprite_manager.h"

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

enum _ui_colors {
	UI_MAIN_COL,
	UI_PRIMARY_COL,
	UI_SHADOW_COL,
	UI_TEXT_COL,
	maxUIColors,
};

enum _ui_sprite_list {
	UI_SPRITE_NONE,
	UI_SPRITE_LOGO,
	maxUISprites
};

struct m_entry
{
	std::wstring m_name{};
	bool m_space{};
};

struct s_entry
{
	std::wstring m_name{};
	std::string m_var{};

	float m_float_step{},
		  m_float_min{},
		  m_float_max{};

	int m_int_step{},
		m_int_min{},
		m_int_max{};

	bool m_space{},
		 m_key_hold{};

	_ui_states m_state{};

	std::function<void(void)> m_fn{};
	std::vector<std::wstring> m_items{};
};

struct ui {
	void instance();

	void init_sprites(IDirect3DDevice9* device) {
		m_sprites[UI_SPRITE_LOGO].init(device, m_ui_logotype, sizeof(m_ui_logotype), 178, 33);
	}

	void on_reset_sprites() {
		for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
			if (i == UI_SPRITE_NONE) continue;
			m_sprites[i].on_reset();
		}
	}

	void on_reset_end_sprites() {
		for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
			if (i == UI_SPRITE_NONE) continue;
			m_sprites[i].on_reset_end();
		}
	}

	void handle_toggle_keys(unsigned int k);
	void handle_input(unsigned int k);

	bool get_menu_state() {
		return m_opened;
	}

private:
	void add(const std::wstring& name, bool space = false) {
		menu_entry[m_entry_size].m_name = name;
		menu_entry[m_entry_size].m_space = space;

		++m_entry_size;
	}

	void add_bool(const std::wstring& name, const std::string& var, bool space = false) {
		subm_entry[s_entry_size].m_name = name;
		subm_entry[s_entry_size].m_var = var;

		subm_entry[s_entry_size].m_state = UI_BOOL_STATE;

		subm_entry[s_entry_size].m_space = space;

		++s_entry_size;
	}

	void add_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space = false) {
		subm_entry[s_entry_size].m_name = name;
		subm_entry[s_entry_size].m_var = var;

		subm_entry[s_entry_size].m_int_min = min;
		subm_entry[s_entry_size].m_int_max = max;

		subm_entry[s_entry_size].m_int_step = step;

		subm_entry[s_entry_size].m_state = UI_INT_STATE;

		subm_entry[s_entry_size].m_space = space;

		++s_entry_size;
	}

	void add_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false) {
		subm_entry[s_entry_size].m_name = name;
		subm_entry[s_entry_size].m_var = var;

		subm_entry[s_entry_size].m_float_min = min;
		subm_entry[s_entry_size].m_float_max = max;

		subm_entry[s_entry_size].m_float_step = step;

		subm_entry[s_entry_size].m_state = UI_FLOAT_STATE;

		subm_entry[s_entry_size].m_space = space;

		++s_entry_size;
	}

	void add_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space = false) {
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

	void add_key(const std::wstring& name, const std::string& var, bool space = false) {
		subm_entry[s_entry_size].m_name = name;
		subm_entry[s_entry_size].m_var = var;

		subm_entry[s_entry_size].m_state = UI_KEY_STATE;

		subm_entry[s_entry_size].m_space = space;

		++s_entry_size;
	}

	void add_function(const std::wstring& name, std::function<void(void)> fn, bool space = false) {
		subm_entry[s_entry_size].m_name = name;

		subm_entry[s_entry_size].m_state = UI_FUNCTION_STATE;
		subm_entry[s_entry_size].m_fn = fn;

		subm_entry[s_entry_size].m_space = space;

		++s_entry_size;
	}

	void add_tab(const std::wstring& name, bool space = false) {
		subm_entry[s_entry_size].m_name = name;

		subm_entry[s_entry_size].m_state = UI_TAB_STATE;

		subm_entry[s_entry_size].m_space = space;

		++s_entry_size;
	}

	void add_sub_bool(const std::wstring& name, const std::string& var, bool space = false) {
		ssubm_entry[ss_entry_size].m_name = name;
		ssubm_entry[ss_entry_size].m_var = var;

		ssubm_entry[ss_entry_size].m_state = UI_BOOL_STATE;

		ssubm_entry[ss_entry_size].m_space = space;

		++ss_entry_size;
	}

	void add_sub_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space = false) {
		ssubm_entry[ss_entry_size].m_name = name;
		ssubm_entry[ss_entry_size].m_var = var;

		ssubm_entry[ss_entry_size].m_int_min = min;
		ssubm_entry[ss_entry_size].m_int_max = max;

		ssubm_entry[ss_entry_size].m_int_step = step;

		ssubm_entry[ss_entry_size].m_state = UI_INT_STATE;

		ssubm_entry[ss_entry_size].m_space = space;

		++ss_entry_size;
	}

	void add_sub_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false) {
		ssubm_entry[ss_entry_size].m_name = name;
		ssubm_entry[ss_entry_size].m_var = var;

		ssubm_entry[ss_entry_size].m_float_min = min;
		ssubm_entry[ss_entry_size].m_float_max = max;

		ssubm_entry[ss_entry_size].m_float_step = step;

		ssubm_entry[ss_entry_size].m_state = UI_FLOAT_STATE;

		ssubm_entry[ss_entry_size].m_space = space;

		++ss_entry_size;
	}

	void add_sub_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space = false) {
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

	void add_sub_key(const std::wstring& name, const std::string& var, bool space = false) {
		ssubm_entry[ss_entry_size].m_name = name;
		ssubm_entry[ss_entry_size].m_var = var;

		ssubm_entry[ss_entry_size].m_state = UI_KEY_STATE;

		ssubm_entry[ss_entry_size].m_space = space;

		++ss_entry_size;
	}

	void add_sub_function(const std::wstring& name, std::function<void(void)> fn, bool space = false) {
		ssubm_entry[ss_entry_size].m_name = name;

		ssubm_entry[ss_entry_size].m_state = UI_FUNCTION_STATE;
		ssubm_entry[ss_entry_size].m_fn = fn;

		ssubm_entry[ss_entry_size].m_space = space;

		++ss_entry_size;
	}

	void add_sub_tab(const std::wstring& name, bool space = false) {
		ssubm_entry[ss_entry_size].m_name = name;

		ssubm_entry[ss_entry_size].m_state = UI_TAB_STATE;

		ssubm_entry[ss_entry_size].m_space = space;

		++ss_entry_size;
	}

	void add_sub_sub_bool(const std::wstring& name, const std::string& var, bool space = false) {
		sssubm_entry[sss_entry_size].m_name = name;
		sssubm_entry[sss_entry_size].m_var = var;

		sssubm_entry[sss_entry_size].m_state = UI_BOOL_STATE;

		sssubm_entry[sss_entry_size].m_space = space;

		++sss_entry_size;
	}

	void add_sub_sub_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space = false) {
		sssubm_entry[sss_entry_size].m_name = name;
		sssubm_entry[sss_entry_size].m_var = var;

		sssubm_entry[sss_entry_size].m_int_min = min;
		sssubm_entry[sss_entry_size].m_int_max = max;

		sssubm_entry[sss_entry_size].m_int_step = step;

		sssubm_entry[sss_entry_size].m_state = UI_INT_STATE;

		sssubm_entry[sss_entry_size].m_space = space;

		++sss_entry_size;
	}

	void add_sub_sub_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false) {
		sssubm_entry[sss_entry_size].m_name = name;
		sssubm_entry[sss_entry_size].m_var = var;

		sssubm_entry[sss_entry_size].m_float_min = min;
		sssubm_entry[sss_entry_size].m_float_max = max;

		sssubm_entry[sss_entry_size].m_float_step = step;

		sssubm_entry[sss_entry_size].m_state = UI_FLOAT_STATE;

		sssubm_entry[sss_entry_size].m_space = space;

		++sss_entry_size;
	}

	void add_sub_sub_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, bool space = false) {
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

	void add_sub_sub_key(const std::wstring& name, const std::string& var, bool space = false) {
		sssubm_entry[sss_entry_size].m_name = name;
		sssubm_entry[sss_entry_size].m_var = var;

		sssubm_entry[sss_entry_size].m_state = UI_KEY_STATE;

		sssubm_entry[sss_entry_size].m_space = space;

		++sss_entry_size;
	}

	void add_sub_sub_function(const std::wstring& name, std::function<void(void)> fn, bool space = false) {
		sssubm_entry[sss_entry_size].m_name = name;

		sssubm_entry[sss_entry_size].m_state = UI_FUNCTION_STATE;
		sssubm_entry[sss_entry_size].m_fn = fn;

		sssubm_entry[sss_entry_size].m_space = space;

		++sss_entry_size;
	}

	void draw(int x, int y);
	void setup();

	void clear() {
		for (int i = 0; i < m_entry_size; i++)
		{
			menu_entry[i].m_name.clear();
			menu_entry[i].m_space = false;
		}

		for (int i = 0; i < s_entry_size; i++)
		{
			subm_entry[i].m_name.clear();
			subm_entry[i].m_var.clear();

			subm_entry[i].m_int_min = 0;
			subm_entry[i].m_int_max = 0;
			subm_entry[i].m_int_step = 0;

			subm_entry[i].m_float_min = 0.0f;
			subm_entry[i].m_float_max = 0.0f;
			subm_entry[i].m_float_step = 0.0f;

			subm_entry[i].m_state = UI_NONE_STATE;

			subm_entry[i].m_items.clear();
			subm_entry[i].m_fn = []() {};
		}

		for (int i = 0; i < ss_entry_size; i++)
		{
			ssubm_entry[i].m_name.clear();
			ssubm_entry[i].m_var.clear();

			ssubm_entry[i].m_int_min = 0;
			ssubm_entry[i].m_int_max = 0;
			ssubm_entry[i].m_int_step = 0;

			ssubm_entry[i].m_float_min = 0.0f;
			ssubm_entry[i].m_float_max = 0.0f;
			ssubm_entry[i].m_float_step = 0.0f;

			ssubm_entry[i].m_state = UI_NONE_STATE;

			ssubm_entry[i].m_items.clear();
			ssubm_entry[i].m_fn = []() {};
		}

		for (int i = 0; i < sss_entry_size; i++)
		{
			sssubm_entry[i].m_name.clear();
			sssubm_entry[i].m_var.clear();

			sssubm_entry[i].m_int_min = 0;
			sssubm_entry[i].m_int_max = 0;
			sssubm_entry[i].m_int_step = 0;

			sssubm_entry[i].m_float_min = 0.0f;
			sssubm_entry[i].m_float_max = 0.0f;
			sssubm_entry[i].m_float_step = 0.0f;

			ssubm_entry[i].m_state = UI_NONE_STATE;

			sssubm_entry[i].m_items.clear();
			sssubm_entry[i].m_fn = []() {};
		}

		m_entry_size = 0;
		s_entry_size = 0;
		ss_entry_size = 0;
		sss_entry_size = 0;
	}
private:
	bool m_opened{},
	     s_opened[maxUISubs]{};

	int m_entry_position{},
	    m_entry_size{},
		s_entry_position{},
		s_entry_size{},
		ss_entry_position{},
		ss_entry_size{},
		sss_entry_position{},
		sss_entry_size{};

	m_entry menu_entry[10]{};
	s_entry subm_entry[16]{},
		    ssubm_entry[16]{},
		    sssubm_entry[16]{};

	color_t m_colors[maxUIColors]{};
	sprite_t m_sprites[maxUISprites]{};

	std::vector<std::wstring> cfg_list{ L"1", L"2", L"3", L"4", L"5" };
};

extern ui g_ui;