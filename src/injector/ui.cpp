#include "ui.h"

void c_ui::poll_input(HWND hwnd)
{
	static bool is_dragging = false;
	static c_vec2 drag_pos = c_vec2(0, 0);

	m_prev_mouse_pos = m_mouse_pos;

	for (int i = 0; i < 256; i++) {
		m_prev_key_state[i] = m_key_state[i];
		m_key_state[i] = GetAsyncKeyState(i);
	}

	POINT p;

	GetCursorPos(&p);
	ScreenToClient(hwnd, &p);

	m_mouse_pos = c_vec2(p.x, p.y);

	RECT window_rect;
	GetWindowRect(hwnd, &window_rect);

	if (is_key_down(VK_LBUTTON)) {
		POINT screen_mouse = { static_cast<LONG>(m_mouse_pos.x), static_cast<LONG>(m_mouse_pos.y) };
		ClientToScreen(hwnd, &screen_mouse);

		if (is_key_pressed(VK_LBUTTON)) {
			bool hit_drag_zone = true;

			if (hit_drag_zone) {
				is_dragging = true;
				drag_pos.x = screen_mouse.x - window_rect.left;
				drag_pos.y = screen_mouse.y - window_rect.top;
			}
		}

		if (is_dragging) {
			int new_x = screen_mouse.x - static_cast<int>(drag_pos.x);
			int new_y = screen_mouse.y - static_cast<int>(drag_pos.y);
			int width = window_rect.right - window_rect.left;
			int height = window_rect.bottom - window_rect.top;

			SetWindowPos(hwnd, nullptr, new_x, new_y, width, height, SWP_NOZORDER | SWP_NOSIZE);
		}
	}
	else
		is_dragging = false;
}

bool c_ui::is_key_down(int key)
{
	return m_key_state[key];
}

bool c_ui::is_key_pressed(int key)
{
	return !m_prev_key_state[key] && m_key_state[key];
}

bool c_ui::is_key_released(int key)
{
	return m_prev_key_state[key] && !m_key_state[key];
}

c_vec2 c_ui::get_mouse_pos()
{
	return m_mouse_pos;
}

c_vec2 c_ui::get_prev_mouse_pos()
{
	return m_prev_mouse_pos;
}

bool c_ui::in_bounds(const c_vec2 pos, const c_vec2 size)
{
	return (m_mouse_pos.x > pos.x && m_mouse_pos.y > pos.y &&
		m_mouse_pos.x < pos.x + size.x && m_mouse_pos.y < pos.y + size.y);
}

bool c_ui::is_hovered(const c_vec2 min, const c_vec2 max)
{
	return (m_mouse_pos.x >= min.x && m_mouse_pos.y >= min.y &&
		m_mouse_pos.x <= max.x && m_mouse_pos.y <= max.y);
}

c_game* c_ui::set_game(const std::string& label)
{
	auto game = c_ui_mem_pool::alloc<c_game>(label);

	game->set_parent(this);

	m_games.push_back(game);
	m_active_game = game;

	return game;
}

c_group* c_ui::set_group(c_game* game, const std::string& label, const c_vec2& size, std::function<void(c_group*)> items, bool dont_render)
{
	auto group = c_ui_mem_pool::alloc<c_group>(label, size, dont_render);

	items(group);
	game->add_child(group);

	return group;
}

void c_ui::think()
{
	c_vec2 drag_min(m_pos);
	c_vec2 drag_max(m_size.x, 40);

	for (int i = 0; i < m_games.size(); i++)
	{
		auto game = m_games[i];

		c_vec2 rect_min(m_pos + c_vec2(5, UI_START_POSITION_Y + (i * 35)));
		c_vec2 rect_max(85, 30);

		if (is_key_pressed(VK_LBUTTON) && is_hovered(rect_min, rect_min + rect_max))
			m_active_game = game;
	}

	m_active_game->think();
}

void c_ui::draw()
{
	g_renderer.rect_fill_cornered(m_pos, c_vec2(140, m_size.y), 20.0f, c_color(10, 10, 10));
	g_renderer.rect_fill_cornered(m_pos + c_vec2(100, 0), m_size - c_vec2(100, 0), 20.0f, c_color(15, 15, 15));

	for (int i = 0; i < m_games.size(); i++)
	{
		auto game = m_games[i];
		auto game_label = game->get_label();
		auto game_label_height = g_font.get_text_height(game_label, g_font[Verdana13px]);

		c_vec2 rect_min(m_pos + c_vec2(5, UI_START_POSITION_Y + (i * 35)));
		c_vec2 rect_max(85, 30);

		bool hovered = is_hovered(rect_min, rect_min + rect_max);
		bool actived = m_active_game == game;

		if (actived)
		{
			g_renderer.rect_fill_cornered(rect_min, rect_max, 5.0f, c_color(60, 60, 60, 55));
			g_renderer.rect_fill_cornered(rect_min + c_vec2(rect_max.x - 3, 6), c_vec2(3, 3 + game_label_height), 15.0f,
				(is_key_down(VK_LBUTTON) && hovered) ? c_color(66, 84, 114) : c_color(68, 90, 129));
		}
		else
		{
			if (hovered)
				g_renderer.rect_fill_cornered(rect_min, rect_max, 5.0f, c_color(60, 60, 60, 55));
		}

		g_font.draw_string(game_label, rect_min.x + 9, rect_min.y + 7, g_font[Verdana13px], TEXT_NONE,
			actived ? c_color(66, 84, 114) : c_color(220, 220, 220));
	}

	m_active_game->draw();
}

c_vec2 c_ui::get_child_draw_position()
{
	return m_pos + c_vec2(UI_START_POSITION_X, UI_START_POSITION_Y);
}