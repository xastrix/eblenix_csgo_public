#pragma once

// Used sinf, cosf
#include <cmath>

namespace vecUtil
{
	template <class T>
	inline T vecMix(const T& x, const T& y, float a)
	{
		return x + (y - x) * a;
	}

	template <class T>
	inline T vecClamp(const T& v, const T& min, const T& max)
	{
		T v1;

		for (int i = 0; i < T::SIZE; ++i)
			v1[i] = v[i] > max[i] ? max[i] : (v[i] < min[i] ? min[i] : v[i]);

		return v1;
	}

	template <class T>
	inline T vecSaturate(const T& v)
	{
		T v1;

		for (int i = 0; i < T::SIZE; ++i)
			v1[i] = v[i] > 1.0f ? 1.0f : (v[i] < 0.0f ? 0.0f : v[i]);

		return v1;
	}

	inline void sinCos(float a, float* sina, float* cosa)
	{
		*sina = std::sinf(a);
		*cosa = std::cosf(a);
	}
}