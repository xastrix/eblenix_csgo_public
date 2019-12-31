#include "matrix.h"

matrix_t::matrix_t(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23
)
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

float (&matrix_t::get_matrix())[3][4]
{
	return matrix;
}

float* matrix_t::operator[](int i)
{
	return matrix[i];
}