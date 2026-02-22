#include "vec3.h"
#include "vec_util.h"

// FLT_MAX, etc
#include <float.h>

// Used min, max
#include <algorithm>

void vec3::set(float x0, float y0, float z0)
{
	x = x0;
	y = y0;
	z = z0;
}

vec3& vec3::operator+=(const vec3& o)
{
	x += o.x;
	y += o.y;
	z += o.z;

	return *this;
}

vec3& vec3::operator-=(const vec3& o)
{
	x -= o.x;
	y -= o.y;
	z -= o.z;

	return *this;
}

vec3& vec3::operator*=(float s)
{
	x *= s;
	y *= s;
	z *= s;

	return *this;
}

void vec3::normalize()
{
	x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	z = 0.0f;
}

float vec3::distance_to(const vec3& o)
{
	vec3 delta;

	delta.x = x - o.x;
	delta.y = y - o.y;
	delta.z = z - o.z;

	return length(delta);
}

vec3& vec3::operator*=(const vec3& o)
{
	x *= o.x;
	y *= o.y;
	z *= o.z;

	return *this;
}

vec3& vec3::operator/=(const vec3& o)
{
	x /= o.x;
	y /= o.y;
	z /= o.z;

	return *this;
}

vec3& vec3::operator/=(float s)
{
	const float sinv = 1.0f / s;

	x *= sinv;
	y *= sinv;
	z *= sinv;

	return *this;
}

vec3 vec3::operator/(float s) const
{
	const float sinv = 1.f / s;
	return vec3(x * sinv, y * sinv, z * sinv);
}

vec3 vec3::operator*(const vec3& o) const
{
	return vec3(x*o.x, y*o.y, z*o.z);
}

vec3 vec3::operator/(const vec3& o) const
{
	return vec3(x / o.x, y / o.y, z / o.z);
}

vec3 vec3::operator+(const vec3& o) const
{
	return vec3(x + o.x, y + o.y, z + o.z);
}

vec3 vec3::operator-(const vec3& o) const
{
	return vec3(x - o.x, y - o.y, z - o.z);
}

vec3 vec3::operator-() const
{
	return vec3(-x, -y, -z);
}

vec3 vec3::operator*(float s) const
{
	return vec3(x*s, y*s, z*s);
}

bool vec3::operator==(const vec3& o) const
{
	return x == o.x && y == o.y && z == o.z;
}

bool vec3::operator!=(const vec3& o) const
{
	return !(*this == o);
}

bool vec3::operator!() const
{
	return is_zero();
}

vec3 operator*(float s, const vec3& v)
{
	return vec3(v.x*s, v.y*s, v.z*s);
}

float length(const vec3& v)
{
	float res = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return res;
}

float length_2d(const vec3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y);
}

float dot(const vec3& a, const vec3& b)
{
	float res = a.x * b.x + a.y * b.y + a.z * b.z;
	return res;
}

float dot(const vec3& a, const float* o)
{
	return { a.x * o[0] + a.y * o[1] + a.z * o[2] };
}

vec3 normalize(const vec3& v)
{
	float len = length(v);
	float invlen = 1.0f / len;

	vec3 res(v.x * invlen, v.y * invlen, v.z * invlen);

	return res;
}

vec3 max(const vec3& a, const vec3& b)
{
	return vec3(std::max(a[0], b[0]), std::max(a[1], b[1]), std::max(a[2], b[2]));
}

vec3 min(const vec3& a, const vec3& b)
{
	return vec3(std::min(a[0], b[0]), std::min(a[1], b[1]), std::min(a[2], b[2]));
}

vec3 abs(const vec3& v)
{
	return vec3(std::fabsf(v[0]), std::fabsf(v[1]), std::fabsf(v[2]));
}

vec3 mix(const vec3& x, const vec3& y, float a)
{
	return vecUtil::vecMix(x, y, a);
}

float distance(const vec3& p0, const vec3& p1)
{
	return length(p0 - p1);
}

vec3 clamp(const vec3& v, const vec3& min, const vec3& max)
{
	return vecUtil::vecClamp(v, min, max);
}

void clamp(vec3& v)
{
	v.x = std::clamp(v.x, -89.0f, 89.0f);
	v.y = std::clamp(std::remainder(v.y, 360.0f), -180.0f, 180.0f);
	v.z = std::clamp(v.z, -50.0f, 50.0f);
}

vec3 saturate(const vec3& v)
{
	return vecUtil::vecSaturate(v);
}

bool check(const vec3& v)
{
	return v.x <= FLT_MAX && v.x >= -FLT_MAX && v.y <= FLT_MAX && v.y >= -FLT_MAX && v.z <= FLT_MAX && v.z >= -FLT_MAX;
}

vec3 neg(const vec3& a)
{
	return -a;
}

vec3 add(const vec3& a, const vec3& b)
{
	return a + b;
}

vec3 sub(const vec3& a, const vec3& b)
{
	return a - b;
}

vec3 mul(const vec3& a, float b)
{
	return a * b;
}

vec3 mul(float b, const vec3& a)
{
	return a * b;
}

vec3 rotate_z(const vec3& v, float a)
{
	float s, c;
	vecUtil::sinCos(-a, &s, &c);

	return vec3(v.x * c + v.y * s, v.y * c - v.x * s, v.z);
}

vec3 rotate_y(const vec3& v, float a)
{
	float s, c;
	vecUtil::sinCos(a, &s, &c);

	return vec3(v.z * s + v.x * c, v.y, v.z * c - v.x * s);
}

vec3 rotate_x(const vec3& v, float a)
{
	float s, c;
	vecUtil::sinCos(a, &s, &c);

	return vec3(v.x, v.y * c - v.z * s, v.y * s + v.z * c);
}

vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

vec3 facenormal_ccw(const vec3& v0, const vec3& v1, const vec3& v2)
{
	vec3 a(v1.x - v0.x, v1.y - v0.y, v1.z - v0.z);
	vec3 b(v2.x - v0.x, v2.y - v0.y, v2.z - v0.z);

	vec3 res(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);

	res *= 1.0f / length(res);
	return res;
}

vec3 facenormal_cw(const vec3& v0, const vec3& v1, const vec3& v2)
{
	return facenormal_ccw(v0, v2, v1);
}

vec3 faceforward(const vec3& n, const vec3& i, const vec3& nref)
{
	return dot(nref, i) < 0.f ? n : -n;
}

vec3 reflect(const vec3& i, const vec3& n)
{
	return i - n * (2.f * dot(n, i));
}

vec3 refract(const vec3& i, const vec3& n, float eta)
{
	float ndoti = dot(n, i);

	float k = 1.0f - eta * eta*(1.0f - ndoti * ndoti);

	if (k < 0.0f)
		return vec3(0.f);
	else
		return i * eta - n * (eta*ndoti + sqrtf(k));
}

vec3& vec3::operator-=(float s)
{
	x -= s;
	y -= s;
	z -= s;

	return *this;
}

vec3& vec3::operator+=(float s)
{
	x += s;
	y += s;
	z += s;

	return *this;
}