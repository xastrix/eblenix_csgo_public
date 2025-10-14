#include "vec3.h"

vec3::vec3(void)
{
	x = y = z = 0.0f;
}

vec3::vec3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

void vec3::init(float _x, float _y, float _z)
{
	x = _x; y = _y; z = _z;
}

void vec3::clamp(void)
{
	x = std::clamp(x, -89.0f, 89.0f);
	y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
	z = std::clamp(z, -50.0f, 50.0f);
}

vec3 vec3::clamped()
{
	vec3 clamped = *this;
	clamped.clamp();

	return clamped;
}

float vec3::distance_to(const vec3& other)
{
	vec3 delta;

	delta.x = x - other.x;
	delta.y = y - other.y;
	delta.z = z - other.z;

	return delta.length();
}

bool vec3::is_zero(void)
{
	return (x == 0 && y == 0 && z == 0);
}

void vec3::normalize()
{
	x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
	y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
	z = 0.0f;
}

vec3 vec3::normalized(void)
{
	vec3 vec(*this);
	vec.normalize();

	return vec;
}

float vec3::length(void)
{
	float root = 0.0f, sqsr = this->length_sqr();
	root = std::sqrt(sqsr);

	return root;
}

float vec3::length_sqr(void)
{
	auto sqr = [](float n) {
		return static_cast<float>(n * n);
	};

	return { sqr(x) + sqr(y) + sqr(z) };
}

float vec3::length_2d_sqr(void) const
{
	return { x * x + y * y };
}

float vec3::dot(const vec3 other)
{
	return { x * other.x + y * other.y + z * other.z };
}

float vec3::dot(const float* other)
{
	const vec3& a = *this;
	return(a.x * other[0] + a.y * other[1] + a.z * other[2]);
}

vec3& vec3::operator+=(const vec3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

vec3& vec3::operator-=(const vec3& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

vec3& vec3::operator*=(float v)
{
	x *= v; y *= v; z *= v;
	return *this;
}

vec3 vec3::operator+(const vec3& v)
{
	return vec3{ x + v.x, y + v.y, z + v.z };
}

vec3 vec3::operator-(const vec3& v)
{
	return vec3{ x - v.x, y - v.y, z - v.z };
}

vec3 vec3::operator*(float fl) const
{
	return vec3{ x * fl, y * fl, z * fl };
}

vec3 vec3::operator*(const vec3& v) const
{
	return vec3{ x * v.x, y * v.y, z * v.z };
}

vec3& vec3::operator/=(float fl)
{
	x /= fl;
	y /= fl;
	z /= fl;

	return *this;
}

vec3 vec3::operator-(const vec3& other) const
{
	auto buf = *this;

	buf.x -= other.x;
	buf.y -= other.y;
	buf.z -= other.z;

	return buf;
}

auto vec3::operator/(float other) const
{
	vec3 vec;

	vec.x = x / other;
	vec.y = y / other;
	vec.z = z / other;

	return vec;
}

float& vec3::operator[](int i)
{
	return ((float*)this)[i];
}

float vec3::operator[](int i) const
{
	return ((float*)this)[i];
}

float vec3::length_2d() const
{
	return std::sqrtf(x * x + y * y);
}

void vec3::crossproduct(vec3 v1, vec3 v2, vec3 cross_p) const
{
	cross_p.x = (v1.y * v2.z) - (v1.z * v2.y);
	cross_p.y = -((v1.x * v2.z) - (v1.z * v2.x));
	cross_p.z = (v1.x * v2.y) - (v1.y * v2.x);
}

vec3 vec3::cross(const vec3 & other) const
{
	vec3 res;
	crossproduct(*this, other, res);
	return res;
}