#pragma once

#include <string>
#include <array>
#include <d3dx9.h>

#include "vars.h"

class color_t {
public:
	color_t(int r = 255, int g = 255, int b = 255, int a = 255)
		: m_color{ r, g, b, a } {}

	explicit color_t(const std::string& var_name, int alpha = 255)
		: m_color{
			get_var_value(var_name + R_COL_P),
			get_var_value(var_name + G_COL_P),
			get_var_value(var_name + B_COL_P),
			alpha,
	} {}

	color_t(int alpha, const std::string& var_name)
		: m_color{
			alpha,
			get_var_value(var_name + R_COL_P),
			get_var_value(var_name + G_COL_P),
			get_var_value(var_name + B_COL_P),
	} {};

	D3DCOLOR get() const {
		return D3DCOLOR_RGBA(m_color[0], m_color[1], m_color[2], m_color[3]);
	}

	D3DCOLOR get_revert() const {
		return D3DCOLOR_ARGB(m_color[0], m_color[3], m_color[2], m_color[1]);
	}

	int get_alpha() const {
		return m_color[3];
	}

	const std::array<int, 4>& get_arr() const { return m_color; }

private:
	std::array<int, 4> m_color{};

	int get_var_value(const std::string& key) const {
		auto opt = g_vars.get_as<int>(key);

		if (!opt.has_value())
			return 0;

		return opt.value();
	}
};