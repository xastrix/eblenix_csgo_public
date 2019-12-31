#pragma once

#include "vec3.h"

class vec2 {
public:
	float x, y;

	vec2();
	vec2(float _x, float _y);
	vec2(vec3 vec);

	float length();

	vec2 operator*(const float n) const;
	vec2 operator+(const vec2& v) const;
	vec2 operator-(const vec2 & v) const;
	void operator+=(const vec2 & v);
	void operator-=(const vec2 & v);
	bool operator==(const vec2 & v) const;
	bool operator!=(const vec2 & v) const;
};