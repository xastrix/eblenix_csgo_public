#pragma once

#include "renderer.h"

#include <functional>

#define UI_START_POSITION_X 120
#define UI_START_POSITION_Y 30

class c_element {
public:
	virtual void think() = 0;
	virtual void draw() = 0;

	virtual c_element* get_parent() {
		return m_parent;
	}

	virtual void set_parent(c_element* parent) {
		m_parent = parent;
	}

	virtual bool is_child(c_element* child) {
		for (auto elem : m_childs) {
			if (elem == child)
				return true;
		}
		return false;
	}

	virtual void add_child(c_element* child) {
		m_childs.push_back(child);
		child->set_parent(this);
	}

	virtual void set_label(const std::string& label) {
		m_label = label;
	}

	virtual std::string get_label() {
		return m_label;
	}

	virtual void set_size(const c_vec2& size) {
		m_size = size;
	}

	virtual c_vec2 get_size() {
		return m_size;
	}

	virtual void set_position(const c_vec2& position) {
		m_pos = position;
	}

	virtual c_vec2& get_position() {
		return m_pos;
	}

	virtual c_vec2 get_child_draw_position() {
		if (m_parent == nullptr)
			return m_pos;

		return m_parent->get_child_draw_position() + m_pos;
	}

protected:
	c_vec2 m_pos;
	c_vec2 m_size;
	std::string m_label;
	std::vector<c_element*> m_childs;
	c_element* m_parent;
};

class c_game : public c_element {
public:
	c_game(const std::string& label) {
		m_label = label;
	}

	void think() override {
		for (auto child : m_childs) {
			child->think();
		}
	}

	void draw() override {
		for (auto child : m_childs)
			child->draw();
	}

	void add_child(c_element* child) override {
		c_element::add_child(child);

		m_cursor_pos.y += child->get_size().y + 16;

		if (m_cursor_pos.y - 16 >= (m_parent->get_size().y - 32)) {
			m_cursor_pos.x += child->get_size().x + 16;
			m_cursor_pos.y = child->get_size().y + 16;
		}

		child->set_position(m_cursor_pos - c_vec2(0, child->get_size().y + 16));
	}

	c_vec2 get_child_draw_position() override {
		return c_element::get_child_draw_position();
	}

	c_vec2 get_cursor_pos() {
		return m_cursor_pos;
	}

	void set_cursor_pos(const c_vec2& pos) {
		m_cursor_pos = pos;
	}

private:
	c_vec2 m_cursor_pos;
};

class c_group : public c_element {
public:
	c_group(const std::string& label, const c_vec2& size, bool dont_render = false) {
		m_label = label;
		m_size = size;
		m_dont_render = dont_render;
	}

	void think() override {
		c_vec2 draw_position(m_parent->get_child_draw_position() + m_pos);

		for (auto child : m_childs) {
			child->think();
		}
	}

	void draw() override {
		c_vec2 draw_position(m_parent->get_child_draw_position() + m_pos);

		c_vec2 rect_min(draw_position);
		c_vec2 rect_max(m_size);

		if (!m_dont_render) {
			g_renderer.rect_fill_cornered(rect_min, rect_max, 20.0f, c_color(17, 17, 17));
			g_renderer.rect_cornered(rect_min, rect_max, 20.0f, c_color(35, 35, 35));

			g_renderer.rect_fill(rect_min + c_vec2(0, 32), c_vec2(rect_max.x, 1), c_color(35, 35, 35));

			g_font.draw_string(m_label, rect_min.x + 12, rect_min.y + 12,
				g_font[Verdana12px], TEXT_NONE, c_color(200, 200, 200));
		}

		for (auto child : m_childs)
			child->draw();
	}

	void add_child(c_element* child) override {
		c_element::add_child(child);

		m_cursor_pos.y += child->get_size().y + 16;
		child->set_position(m_cursor_pos - c_vec2(0, child->get_size().y + 16));
	}

	c_vec2 get_child_draw_position() override {
		return m_parent->get_child_draw_position() + m_pos + c_vec2(16, 45);
	}

	c_vec2 get_cursor_pos() {
		return m_cursor_pos;
	}

	void set_cursor_pos(const c_vec2& pos) {
		m_cursor_pos = pos;
	}

	void push_cursor_pos(const c_vec2& pos) {
		m_cursor_pos += pos;
	}

	void pop_cursor_pos(const c_vec2& pos) {
		m_cursor_pos -= pos;
	}

private:
	c_vec2 m_cursor_pos;
	bool m_dont_render;
};

class c_button : public c_element {
public:
	c_button(const std::string& label, std::function<void()> callback) {
		m_label = label;
		m_size = c_vec2(0, 17);
		m_callback = callback;
		m_holding = false;
	}

	void think() override;
	void draw() override;

private:
	std::function<void()> m_callback;
	bool m_holding;
};

class c_label : public c_element {
public:
	c_label(const std::string& label) {
		m_label = label;
	}

	void think() override {}
	void draw() override {
		c_vec2 draw_position = m_parent->get_child_draw_position() + m_pos;

		c_vec2 label_min(draw_position.x + m_size.x, draw_position.y);
		g_font.draw_string(m_label, label_min.x, label_min.y,
			g_font[Verdana13px], TEXT_OUTLINE, c_color(200, 200, 200));
	};
};

class c_ui_mem_pool {
public:
	template <typename T, typename... Args>
	static T* alloc(Args&&... args) {
		if (m_index >= MAX_ELEMENTS)
			return nullptr;

		void* memory_address = m_pool[m_index];
		T* object = ::new (memory_address) T(std::forward<Args>(args)...);

		m_history[m_index] = object;
		m_index++;

		return object;
	}

	static void clear() {
		for (size_t i = 0; i < m_index; i++) {
			if (m_history[i]) {
				m_history[i]->~c_element();
				m_history[i] = nullptr;
			}
		}

		m_index = 0;
	}

private:
	static constexpr size_t  MAX_ELEMENTS = 512;
	static constexpr size_t  MAX_ELEMENT_SIZE = 256;

	inline static uint8_t    m_pool[MAX_ELEMENTS][MAX_ELEMENT_SIZE];
	inline static c_element* m_history[MAX_ELEMENTS];
	inline static size_t     m_index = 0;
};

class c_ui : public c_element {
public:
	void think() override;
	void draw() override;

	c_vec2 get_child_draw_position() override;

	void poll_input(HWND hwnd);

	bool is_key_down(int key);
	bool is_key_pressed(int key);
	bool is_key_released(int key);

	c_vec2 get_mouse_pos();
	c_vec2 get_prev_mouse_pos();

	bool in_bounds(const c_vec2 pos, const c_vec2 size);
	bool is_hovered(const c_vec2 min, const c_vec2 max);

	c_game* set_game(const std::string& label);
	c_group* set_group(c_game* game, const std::string& label, const c_vec2& size, std::function<void(c_group*)> items, bool dont_render = false);

private:
	c_vec2 m_prev_mouse_pos;
	c_vec2 m_mouse_pos;

	bool m_prev_key_state[256];
	bool m_key_state[256];

	std::vector<c_game*> m_games;
	c_game* m_active_game;
};

inline c_ui g_ui;