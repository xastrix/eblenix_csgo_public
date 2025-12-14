#include "font_manager.h"

void font_manager::init(const std::vector<font_resource_t>& fonts)
{
	for (const auto& font : fonts)
	{
		m_handles[font.m_index] = AddFontMemResourceEx(font.m_data, font.m_data_len, NULL, &m_num);
	}
}

void font_manager::undo(const std::vector<font_resource_t>& fonts)
{
	for (const auto& font : fonts)
	{
		if (m_handles[font.m_index]) {
			RemoveFontMemResourceEx(m_handles[font.m_index]);
			m_handles[font.m_index] = NULL;
		}
	}

	m_num = 0;
}