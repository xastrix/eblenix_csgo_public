#include "matrix.h"

matrix3x4_t::matrix3x4_t(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23)
{
	matrix[0][0] = m00;
	matrix[0][1] = m01;
	matrix[0][2] = m02;
	matrix[0][3] = m03;

	matrix[1][0] = m10;
	matrix[1][1] = m11;
	matrix[1][2] = m12;
	matrix[1][3] = m13;

	matrix[2][0] = m20;
	matrix[2][1] = m21;
	matrix[2][2] = m22;
	matrix[2][3] = m23;
}

matrix3x4_t::matrix3x4_t(const vec3& x_axis, const vec3& y_axis, const vec3& z_axis, const vec3& vec_origin)
{
	init(x_axis, y_axis, z_axis, vec_origin);
}

void matrix3x4_t::init(const vec3& x_axis, const vec3& y_axis, const vec3& z_axis, const vec3& vec_origin)
{
	matrix[0][0] = x_axis.x;
	matrix[0][1] = y_axis.x;
	matrix[0][2] = z_axis.x;

	matrix[0][3] = vec_origin.x;

	matrix[1][0] = x_axis.y;
	matrix[1][1] = y_axis.y;
	matrix[1][2] = z_axis.y;

	matrix[1][3] = vec_origin.y;

	matrix[2][0] = x_axis.z;
	matrix[2][1] = y_axis.z;
	matrix[2][2] = z_axis.z;

	matrix[2][3] = vec_origin.z;
}

void matrix3x4_t::set_origin(const vec3& p)
{
	matrix[0][3] = p.x;
	matrix[1][3] = p.y;
	matrix[2][3] = p.z;
}

void matrix3x4_t::invalidate()
{
	for (auto &i : matrix) {
		for (float &j : i)
			j = std::numeric_limits< float >::infinity();
	}
}