#pragma once

#include <string>
#include <vector>
#include <functional>

#include "renderer.h"

#define MAX_MENU_ENTRIES 11
#define MAX_SUB_ENTRIES  16

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
	UI_TEXT_COL,
	UI_MAIN_COL,
	UI_PRIMARY_COL,
	UI_SHADOW_COL,
	UI_OUTLINE_COL,
	maxUIColors,
};

enum _ui_sprites {
	UI_SPRITE_LOGO,
	maxUISprites,
};

struct m_entry {
	std::wstring name;
	bool space;
};

struct s_entry {
	std::wstring name;
	std::string var;

	float float_step,
		  float_min,
		  float_max;

	int int_step,
		int_min,
		int_max;

	bool space,
		 key_hold;

	_ui_states state;

	std::function<void()> fn{};
	std::vector<std::wstring> items{};

	c_color item_text_col{};
};

class c_ui {
public:
	void init(IDirect3DDevice9* device);
	void run();

	void on_reset();
	void on_reset_end();

	void handle_toggle_keys(unsigned int vk);
	void handle_input(unsigned int vk);

	bool get_menu_state() { return m_opened; }
	void set_menu_state(bool v) { m_opened = v; }

	void undo();

	static std::shared_ptr<c_ui> make_shared() {
		return std::shared_ptr<c_ui>(new c_ui());
	}

private:
	void calc_animation_progress(float anim_time, float delta_time, float& old_alpha);
	void draw(int x, int y);
	void setup();

	void add(const std::wstring& name, std::function<void()> fn, bool space = false) {
		m_entry[m_entry_sz].name = name;
		m_entry[m_entry_sz].space = space;

		if (m_entry_pos == m_entry_sz)
			fn();

		++m_entry_sz;
	}

	template <int pos>
	void add_bool(const std::wstring& name, const std::string& var, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;
		s_entries[s_entry_sz[pos]][pos].var = var;

		s_entries[s_entry_sz[pos]][pos].state = UI_BOOL_STATE;

		s_entries[s_entry_sz[pos]][pos].space = space;

		++s_entry_sz[pos];
	}

	template <int pos>
	void add_int(const std::wstring& name, const std::string& var, int min, int max, int step, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;
		s_entries[s_entry_sz[pos]][pos].var = var;

		s_entries[s_entry_sz[pos]][pos].int_min = min;
		s_entries[s_entry_sz[pos]][pos].int_max = max;

		s_entries[s_entry_sz[pos]][pos].int_step = step;

		s_entries[s_entry_sz[pos]][pos].state = UI_INT_STATE;

		s_entries[s_entry_sz[pos]][pos].space = space;

		++s_entry_sz[pos];
	}

	template <int pos>
	void add_float(const std::wstring& name, const std::string& var, float min, float max, float step, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;
		s_entries[s_entry_sz[pos]][pos].var = var;

		s_entries[s_entry_sz[pos]][pos].float_min = min;
		s_entries[s_entry_sz[pos]][pos].float_max = max;

		s_entries[s_entry_sz[pos]][pos].float_step = step;

		s_entries[s_entry_sz[pos]][pos].state = UI_FLOAT_STATE;

		s_entries[s_entry_sz[pos]][pos].space = space;

		++s_entry_sz[pos];
	}

	template <int pos>
	void add_item(const std::wstring& name, const std::string& var, std::vector<std::wstring> items, c_color item_text_col, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;
		s_entries[s_entry_sz[pos]][pos].var = var;

		s_entries[s_entry_sz[pos]][pos].int_min = 0;
		s_entries[s_entry_sz[pos]][pos].int_max = items.size() - 1;

		s_entries[s_entry_sz[pos]][pos].int_step = 1;

		s_entries[s_entry_sz[pos]][pos].state = UI_ITEM_STATE;
		s_entries[s_entry_sz[pos]][pos].items = items;
		s_entries[s_entry_sz[pos]][pos].item_text_col = item_text_col;

		s_entries[s_entry_sz[pos]][pos].space = space;

		++s_entry_sz[pos];
	}

	template <int pos>
	void add_key(const std::wstring& name, const std::string& var, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;
		s_entries[s_entry_sz[pos]][pos].var = var;

		s_entries[s_entry_sz[pos]][pos].state = UI_KEY_STATE;

		s_entries[s_entry_sz[pos]][pos].space = space;

		++s_entry_sz[pos];
	}

	template <int pos>
	void add_function(const std::wstring& name, std::function<void()> fn, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;

		s_entries[s_entry_sz[pos]][pos].state = UI_FUNCTION_STATE;
		s_entries[s_entry_sz[pos]][pos].fn = fn;

		s_entries[s_entry_sz[pos]][pos].space = space;

		++s_entry_sz[pos];
	}

	template <int pos>
	void add_tab(const std::wstring& name, std::function<void()> fn, bool space = false) {
		s_entries[s_entry_sz[pos]][pos].name = name;

		s_entries[s_entry_sz[pos]][pos].state = UI_TAB_STATE;

		s_entries[s_entry_sz[pos]][pos].space = space;

		if (s_entry_pos[pos] == s_entry_sz[pos])
			fn();

		++s_entry_sz[pos];
	}
	
	void clear();

private:
	bool m_opened,
	     m_sopened[maxUISubs]{};

	int m_entry_pos,
		m_entry_sz,
		s_entry_pos[maxUISubs]{},
		s_entry_sz[maxUISubs]{};

	float m_current_animation_progress,
		  m_target_animation_progress;

	m_entry m_entry[MAX_MENU_ENTRIES]{};
	s_entry s_entries[MAX_SUB_ENTRIES][maxUISubs]{};

	c_color   m_colors                [maxUIColors]{};
	sprite_t* m_sprites = new sprite_t[maxUISprites]{};
};

inline std::shared_ptr<c_ui> g_ui = c_ui::make_shared();