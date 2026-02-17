#include "color.h"

color_t color_t::calc_health_color(int hp, int alpha)
{
	auto range_health_col = [](std::vector<range_clr_t> ranges, int hp) -> color_t
	{
		for (const auto& range : ranges)
		{
			if (hp >= range.m_min_health && hp <= range.m_max_health)
				return range.m_color;
		}

		return color_t(99, std::min(255, hp * 225 / 100), 0, 255);
	};

	std::vector<range_clr_t> health_ranges =
	{
		{ 100, 95, color_t(136, 205, 80, alpha) },

		{ 95, 90, color_t(128, 194, 75, alpha) },
		{ 90, 85, color_t(128, 194, 75, alpha) },
		{ 85, 80, color_t(170, 194, 75, alpha) },
		{ 80, 75, color_t(170, 194, 75, alpha) },

		{ 75, 70, color_t(190, 194, 75, alpha) },
		{ 70, 65, color_t(190, 194, 75, alpha) },

		{ 65, 60, color_t(190, 173, 75, alpha) },
		{ 60, 55, color_t(190, 173, 75, alpha) },

		{ 55, 50, color_t(194, 131, 75, alpha) },
		{ 50, 45, color_t(194, 131, 75, alpha) },

		{ 45, 40, color_t(194, 120, 75, alpha) },
		{ 40, 35, color_t(194, 120, 75, alpha) },
		{ 35, 30, color_t(194, 95, 75, alpha) },
		{ 30, 25, color_t(194, 95, 75, alpha) },

		{ 25, 20, color_t(194, 78, 75, alpha) },
		{ 20, 15, color_t(194, 78, 75, alpha) },

		{ 15, 10, color_t(174, 64, 64, alpha) },
		{ 10, 5, color_t(174, 64, 64, alpha) },

		{ 5, 0, color_t(174, 59, 59, alpha) },
	};

	return range_health_col(health_ranges, hp);
}