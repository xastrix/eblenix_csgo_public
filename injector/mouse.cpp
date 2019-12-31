#include "interface.h"

void mouse::on_m1_down()
{
	m_state[M1_BUTTON] = true;
}

void mouse::on_m1_up()
{
	m_state[M1_BUTTON] = false;
}

bool mouse::is_button_held(const mouseButtons index)
{
	return m_state[index] && m_prev_state[index];
}

void mouse::reset_states()
{
	memcpy(&m_prev_state, &m_state, (sizeof(bool) * maxMouseButtons));
}

void mouse::set_mouse_pos(int x, int y)
{
	m_mouse_pos_x = x;
	m_mouse_pos_y = y;
}

int mouse::get_mouse_pos_x()
{
	return m_mouse_pos_x;
}

int mouse::get_mouse_pos_y()
{
	return m_mouse_pos_y;
}