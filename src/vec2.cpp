#include "vec2.h"

vec2::vec2()
{
	x = 0; y = 0;
}

vec2::vec2(float _x, float _y)
{
	x = _x; y = _y;
}

vec2::vec2(vec3 vec)
{
	x = vec.x; y = vec.y;
}

float vec2::length()
{
	return sqrt((x * x) + (y * y));
}

vec2 vec2::operator*(const float n) const
{
	return vec2(x * n, y * n);
}

vec2 vec2::operator+(const vec2& v) const
{
	return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator-(const vec2 & v) const
{
	return vec2(x - v.x, y - v.y);
}

void vec2::operator+=(const vec2 & v)
{
	x += v.x;
	y += v.y;
}

void vec2::operator-=(const vec2 & v)
{
	x -= v.x;
	y -= v.y;
}

bool vec2::operator==(const vec2 & v) const
{
	return (v.x == x && v.y == y);
}

bool vec2::operator!=(const vec2 & v) const
{
	return (v.x != x || v.y != y);
}