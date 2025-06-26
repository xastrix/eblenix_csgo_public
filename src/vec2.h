#pragma once

#include "vec3.h"

class vec2 {
public:
	vec2(float x = 0.0f, float y = 0.0f) : x(x), y(y) {}
	vec2(const vec3& vec) { x = vec.x; y = vec.y; }

	vec2 operator*(const float n) const {
		return { x * n, y * n };
	}

	vec2 operator+(const vec2& v) const {
		return { x + v.x, y + v.y };
	}

	vec2 operator-(const vec2& v) const {
		return { x - v.x, y - v.y };
	}

	void operator+=(const vec2& v) {
		x += v.x;
		y += v.y;
	}

	void operator-=(const vec2& v) {
		x -= v.x;
		y -= v.y;
	}

	bool operator==(const vec2& v) const {
		return (v.x == x && v.y == y);
	}

	bool operator!=(const vec2& v) const {
		return (v.x != x || v.y != y);
	}

	float length() {
		return std::sqrt((x * x) + (y * y));
	}

	float x, y;
};