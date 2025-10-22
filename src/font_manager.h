#pragma once

#include <windows.h>

enum _font_resources {
	AstriumwepRes,
	maxFontResources,
};

struct font_res_t {
	int m_index{};
	unsigned char* m_data{};
	unsigned int m_data_len{};
};

struct font_manager {
	void init();
	void undo();
private:
	HANDLE m_handles[maxFontResources]{};
	DWORD  m_num{};
};

inline font_manager g_fonts;