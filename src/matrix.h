#pragma once

#include "vec.h"
#include "vec3.h"

struct matrix_t {
	matrix_t() = default;

	matrix_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23
	);

	float (&get_matrix())[3][4];
	float* operator[](int i);

private:
	float matrix[3][4];
};