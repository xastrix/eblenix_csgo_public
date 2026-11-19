#pragma once

#include "vec3.h"
#include "vec4.h"

using matrix3x3_t = float[3][3];

struct matrix3x4_t
{
	matrix3x4_t() = default;

	matrix3x4_t(const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13,
		const float m20, const float m21, const float m22, const float m23)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;
	}

	matrix3x4_t(const vec3& x, const vec3& y, const vec3& z, const vec3& origin)
	{
		init(x, y, z, origin);
	}

	inline void init(const vec3& forward, const vec3& left, const vec3& up, const vec3& origin)
	{
		set_forward(forward);
		set_left(left);
		set_up(up);
		set_origin(origin);
	}

	inline void set_forward(const vec3& forward)
	{
		m[0][0] = forward.x;
		m[1][0] = forward.y;
		m[2][0] = forward.z;
	}

	inline void set_left(const vec3& left)
	{
		m[0][1] = left.x;
		m[1][1] = left.y;
		m[2][1] = left.z;
	}

	inline void set_up(const vec3& up)
	{
		m[0][2] = up.x;
		m[1][2] = up.y;
		m[2][2] = up.z;
	}

	inline void set_origin(const vec3& origin)
	{
		m[0][3] = origin.x;
		m[1][3] = origin.y;
		m[2][3] = origin.z;
	}

	float m[3][4]{};
};

struct matrix4x4_t
{
	matrix4x4_t() = default;

	matrix4x4_t(const float m00, const float m01, const float m02, const float m03,
		const float m10, const float m11, const float m12, const float m13, const float m20,
		const float m21, const float m22, const float m23, const float m30,
		const float m31, const float m32, const float m33)
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;

		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;

		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;

		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}

	matrix4x4_t(const matrix3x4_t& from, const vec4& additional_column = {})
	{
		m[0][0] = from.m[0][0];
		m[0][1] = from.m[0][1];
		m[0][2] = from.m[0][2];
		m[0][3] = from.m[0][3];

		m[1][0] = from.m[1][0];
		m[1][1] = from.m[1][1];
		m[1][2] = from.m[1][2];
		m[1][3] = from.m[1][3];

		m[2][0] = from.m[2][0];
		m[2][1] = from.m[2][1];
		m[2][2] = from.m[2][2];
		m[2][3] = from.m[2][3];


		m[3][0] = additional_column.x;
		m[3][1] = additional_column.y;
		m[3][2] = additional_column.z;
		m[3][3] = additional_column.w;
	}

	const matrix3x4_t& as3x4() const
	{
		return *reinterpret_cast<const matrix3x4_t*>(this);
	}

	matrix3x4_t& as3x4()
	{
		return *reinterpret_cast<matrix3x4_t*>(this);
	}

	float m[4][4]{};
};