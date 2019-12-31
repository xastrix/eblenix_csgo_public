#include "font_manager.h"
#include "fonts.hpp"

#include <vector>

static std::vector<font_res_t> font_res_list = {
	{ AstriumwepRes, astriumwep_ttf, ASTRIUMWEP_TTF_SZ },
};

void font_manager::init()
{
	for (const auto& font : font_res_list)
	{
		m_handles[font.m_index] = AddFontMemResourceEx(font.m_data, font.m_data_len, NULL, &m_num);
	}
}

void font_manager::undo()
{
	for (const auto& font : font_res_list)
	{
		if (m_handles[font.m_index]) {
			RemoveFontMemResourceEx(m_handles[font.m_index]);
			m_handles[font.m_index] = NULL;
		}
	}

	m_num = 0;
}