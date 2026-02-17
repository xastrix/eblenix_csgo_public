#include "vec2.h"
#include "vec_util.h"

// FLT_MAX, etc
#include <float.h>

// Used min, max
#include <algorithm>

vec2::vec2(float v)
{
	y = x = v;
}

void vec2::set(float x0, float y0)
{
	x = x0;
	y = y0;
}

vec2& vec2::operator+=(const vec2& o)
{
	x += o.x;
	y += o.y;

	return *this;
}

vec2& vec2::operator-=(const vec2& o)
{
	x -= o.x;
	y -= o.y;

	return *this;
}

void vec2::normalize()
{
	float len = length(*this);
	*this *= 1.0f / len;
}

vec2& vec2::operator*=(float s)
{
	x *= s;
	y *= s;

	return *this;
}

vec2& vec2::operator/=(float s)
{
	const float sinv = 1.f / s;

	x *= sinv;
	y *= sinv;

	return *this;
}

vec2 vec2::operator/(float s) const
{
	const float sinv = 1.0f / s;

	return vec2(x * sinv, y * sinv);
}

vec2& vec2::operator*=(const vec2& o)
{
	x *= o.x;
	y *= o.y;

	return *this;
}

vec2& vec2::operator/=(const vec2& o)
{
	x /= o.x;
	y /= o.y;

	return *this;
}

vec2 vec2::operator*(const vec2& o) const
{
	return vec2(x*o.x, y*o.y);
}

vec2 vec2::operator/(const vec2& o) const
{
	return vec2(x / o.x, y / o.y);
}

vec2 vec2::operator+(const vec2& o) const
{
	return vec2(x + o.x, y + o.y);
}

vec2 vec2::operator-(const vec2& o) const
{
	return vec2(x - o.x, y - o.y);
}

vec2 vec2::operator-() const
{
	return vec2(-x, -y);
}

vec2 vec2::operator*(float s) const
{
	return vec2(x*s, y*s);
}

bool vec2::operator==(const vec2& o) const
{
	return x == o.x && y == o.y;
}

bool vec2::operator!=(const vec2& o) const
{
	return !(*this == o);
}

float length(const vec2& v)
{
	float res = sqrtf(v.x * v.x + v.y * v.y);
	return res;
}

float dot(const vec2& a, const vec2& b)
{
	float res = a.x * b.x + a.y * b.y;
	return res;
}

vec2 normalize(const vec2& v)
{
	float len = length(v);
	float invlen = 1.f / len;

	vec2 res(v.x * invlen, v.y * invlen);

	return res;
}

vec2 operator*(float s, const vec2& v)
{
	return vec2(v.x * s, v.y * s);
}

vec2 max(const vec2& a, const vec2& b)
{
	return vec2(std::max(a[0], b[0]), std::max(a[1], b[1]));
}

vec2 min(const vec2& a, const vec2& b)
{
	return vec2(std::min(a[0], b[0]), std::min(a[1], b[1]));
}

vec2 abs(const vec2& v)
{
	return vec2(std::fabsf(v[0]), std::fabsf(v[1]));
}

vec2 mix(const vec2& x, const vec2& y, float a)
{
	return vecUtil::vecMix(x, y, a);
}

float distance(const vec2& p0, const vec2& p1)
{
	return length(p0 - p1);
}

vec2 clamp(const vec2& v, const vec2& min, const vec2& max)
{
	return vecUtil::vecClamp(v, min, max);
}

vec2 saturate(const vec2& v)
{
	return vecUtil::vecSaturate(v);
}

bool check(const vec2& v)
{
	return v.x <= FLT_MAX && v.x >= -FLT_MAX && v.y <= FLT_MAX && v.y >= -FLT_MAX;
}

vec2 neg(const vec2& a)
{
	return -a;
}

vec2 add(const vec2& a, const vec2& b)
{
	return a + b;
}

vec2 sub(const vec2& a, const vec2& b)
{
	return a - b;
}

vec2 mul(const vec2& a, float b)
{
	return a * b;
}

vec2 mul(float b, const vec2& a)
{
	return a * b;
}

vec2 rotate(const vec2& v, float a)
{
	float s, c;
	vecUtil::sinCos(-a, &s, &c);

	return vec2(v.x * c + v.y * s, v.y * c - v.x * s);
}

vec2& vec2::operator-=(float s)
{
	x -= s;
	y -= s;

	return *this;
}

vec2& vec2::operator+=(float s)
{
	x += s;
	y += s;

	return *this;
}