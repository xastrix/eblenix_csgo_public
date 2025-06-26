#pragma once

#include <limits>
#include <algorithm>

class vec3 {
public:
	vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}

	void init(float _x, float _y, float _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	void clamp() {
		x = std::clamp(x, -89.0f, 89.0f);
		y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
		z = std::clamp(z, -50.0f, 50.0f);
	}

	float distance_to(const vec3& other) {
		vec3 delta{};

		delta.x = x - other.x;
		delta.y = y - other.y;
		delta.z = z - other.z;

		return delta.length();
	}

	void normalize() {
		x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
		y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
		z = 0.0f;
	}

	float length() {
		return std::sqrt(length_sqr());
	}

	float length_2d() {
		return std::sqrtf(x * x + y * y);
	}

	float length_sqr() {
		return { x * x + y * y + z * z };
	}

	float length_2d_sqr() {
		return { x * x + y * y };
	}

	float dot(const vec3 other) {
		return { x * other.x + y * other.y + z * other.z };
	}

	float dot(const float* other) {
		const vec3& a = *this;
		return { a.x * other[0] + a.y * other[1] + a.z * other[2] };
	}

	vec3& operator+=(const vec3& v) {
		x += v.x;
		y += v.y;
		z += v.z;

		return *this;
	}

	vec3& operator-=(const vec3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;

		return *this;
	}

	vec3& operator*=(float v) {
		x *= v;
		y *= v;
		z *= v;

		return *this;
	}

	vec3 operator+(const vec3& v) {
		return vec3{ x + v.x, y + v.y, z + v.z };
	}

	vec3 operator-(const vec3& v) {
		return vec3{ x - v.x, y - v.y, z - v.z };
	}

	auto operator-(const vec3& other) const -> vec3 {
		vec3 vec{ *this };

		vec.x -= other.x;
		vec.y -= other.y;
		vec.z -= other.z;

		return vec;
	}

	vec3 operator*(float fl) const {
		return vec3{ x * fl, y * fl, z * fl };
	}

	vec3 operator*(const vec3& v) const {
		return vec3{ x * v.x, y * v.y, z * v.z };
	}

	vec3& operator/=(float fl) {
		x /= fl;
		y /= fl;
		z /= fl;

		return *this;
	}

	vec3 operator/(float other) const {
		vec3 vec{};

		vec.x = x / other;
		vec.y = y / other;
		vec.z = z / other;

		return vec;
	}

	vec3 operator+(const vec3& v) const {
		return vec3{ x + v.x, y + v.y, z + v.z };
	}

	float& operator[](int i) {
		return ((float*)this)[i];
	}

	float operator[](int i) const {
		return ((float*)this)[i];
	}

	float x, y, z;
};

class __declspec(align(16)) vec_aligned : public vec3 {
public:
	vec_aligned& operator=(const vec3& oth) {
		init(oth.x, oth.y, oth.z);
		return *this;
	}

	float w;
};