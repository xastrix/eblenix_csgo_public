#pragma once

#include "vec.h"

class vec3 {
public:
	float x, y, z;

	vec3();
	vec3(float, float, float);
	~vec3() {};

	void init(float ix, float iy, float iz);
	void clamp();
	vec3 normalized();
	float distance_to(const vec3& other);
	void normalize();
	float length();
	float length_sqr();
	float length_2d_sqr() const;
	float dot(const vec3 other);
	float dot(const float* other);
	vec3& operator+=(const vec3& v);
	vec3& operator-=(const vec3& v);
	vec3& operator*=(float v);
	vec3 operator+(const vec3& v);
	vec3 operator-(const vec3& v);
	vec3 operator*(float fl) const;
	vec3 operator*(const vec3& v) const;
	vec3& operator/=(float fl);
	vec3 operator-(const vec3& other) const;
	auto operator/(float other) const;
	float& operator[](int i);
	float operator[](int i) const;
	float length_2d() const;
	void crossproduct(vec3 v1, vec3 v2, vec3 cross_p) const;
	vec3 cross(const vec3& other) const;
};

class __declspec(align(16)) vector_aligned : public vec3 {
public:
	vector_aligned& operator=(const vec3& oth) {
		init(oth.x, oth.y, oth.z);
		return *this;
	}

	float w;
};

inline vec3 operator*(float lhs, const vec3& rhs) {
	return vec3(rhs.x * lhs, rhs.x * lhs, rhs.x * lhs);
}