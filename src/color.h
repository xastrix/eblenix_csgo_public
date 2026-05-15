#pragma once

#include <string>
#include <array>
#include <d3dx9.h>

#include "vars.h"

class c_color {
public:
	c_color(int r = 255, int g = 255, int b = 255, int a = 255)
		: m_color{ r, g, b, a } {}

	explicit c_color(const std::string& var_name, int alpha = 255)
		: m_color{
			get_var_value(var_name + R_COL_P),
			get_var_value(var_name + G_COL_P),
			get_var_value(var_name + B_COL_P),
			alpha,
	} {}

	c_color(int alpha, const std::string& var_name)
		: m_color{
			alpha,
			get_var_value(var_name + R_COL_P),
			get_var_value(var_name + G_COL_P),
			get_var_value(var_name + B_COL_P),
	} {}

	D3DCOLOR get() const {
		return D3DCOLOR_RGBA(m_color[0], m_color[1], m_color[2], m_color[3]);
	}

	D3DCOLOR get_revert() const {
		return D3DCOLOR_ARGB(m_color[0], m_color[3], m_color[2], m_color[1]);
	}

	static c_color calc_health_color(int hp, int alpha = 255) {
		if (hp <= 0 || hp >= 100)
			return c_color(99, std::min(255, hp * 225 / 100), 0, alpha);

		if (hp >= 55) {
			if (hp >= 75) {
				if (hp >= 85) {
					if (hp >= 95)
						return c_color(136, 205, 80, alpha);

					if (hp >= 90)
						return c_color(128, 194, 75, alpha);

					return c_color(128, 194, 75, alpha);
				}

				if (hp >= 80)
					return c_color(170, 194, 75, alpha);

				return c_color(170, 194, 75, alpha);
			}

			if (hp >= 65) {
				if (hp >= 70)
					return c_color(190, 194, 75, alpha);

				return c_color(190, 194, 75, alpha);
			}

			if (hp >= 60)
				return c_color(190, 173, 75, alpha);

			return c_color(190, 173, 75, alpha);
		}
		else {
			if (hp >= 25) {
				if (hp >= 40) {
					if (hp >= 50)
						return c_color(194, 131, 75, alpha);

					if (hp >= 45)
						return c_color(194, 131, 75, alpha);

					return c_color(194, 120, 75, alpha);
				}

				if (hp >= 35)
					return c_color(194, 120, 75, alpha);

				if (hp >= 30)
					return c_color(194, 95, 75, alpha);

				return c_color(194, 95, 75, alpha);
			}

			if (hp >= 10) {
				if (hp >= 20)
					return c_color(194, 78, 75, alpha);

				if (hp >= 15)
					return c_color(194, 78, 75, alpha);

				return c_color(174, 64, 64, alpha);
			}

			if (hp >= 5)
				return c_color(174, 64, 64, alpha);

			return c_color(174, 59, 59, alpha);
		}
	}

	const std::array<int, 4>& get_arr() const {
		return m_color;
	}

private:
	int get_var_value(const std::string& key) const {
		auto opt = g_var->get_as<int>(key);

		if (!opt.has_value())
			return 0;

		return opt.value();
	}

private:
	std::array<int, 4> m_color;
};