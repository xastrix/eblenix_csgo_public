#pragma once

#include "vec.h"
#include "vec3.h"

using matrix4x4_t = float[4][4];

class matrix3x4_t {
public:
	matrix3x4_t() = default;

	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23);
	matrix3x4_t(const vec3& x_axis, const vec3& y_axis, const vec3& z_axis, const vec3& vec_origin);

	void init(const vec3& x_axis, const vec3& y_axis, const vec3& z_axis, const vec3& vec_origin);
	void set_origin(const vec3& p);
	void invalidate();

	float* operator[](int i) {
		return matrix[i];
	}

	const float* operator[](int i) const {
		return matrix[i];
	}

	float* base() {
		return &matrix[0][0];
	}

	const float* base() const {
		return &matrix[0][0];
	}

	float matrix[3][4];
};