#pragma once

#include <windows.h>
#include <vector>

struct font_resource_t {
	int m_index{};
	unsigned char* m_data{};
	unsigned int m_data_len{};
};

struct font_manager {
	void init(const std::vector<font_resource_t>& fonts);
	void undo(const std::vector<font_resource_t>& fonts);

private:
	HANDLE m_handles[4]{};
	DWORD  m_num{};
};

inline font_manager g_fonts;