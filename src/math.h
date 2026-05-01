#pragma once

#include "vec3.h"
#include "matrix.h"

#include "i_user_cmd.h"

#define M_PI       3.14159265358979323846
#define M_RADPI    57.295779513082f

#define M_PI_F     static_cast<float>(M_PI)

#define RAD2DEG(x) static_cast<float>(x) * static_cast<float>(180.f / M_PI_F)
#define DEG2RAD(x) static_cast<float>(x) * static_cast<float>(M_PI_F / 180.f)

namespace Math
{
	void transform_vector(vec3& a, matrix3x4_t& b, vec3& out);
	void angle_vectors(const vec3& angles, vec3& forward);
	bool clamp_angles(vec3& angles);
	bool normalize_angles(vec3& angles);
	bool w2s(const vec3& origin, vec3& screen);
	float get_damage_armor(float damage, const int armor_value);
	void find_position_rotation(float& x, float& y, const float screen_width, const float screen_height);
	void adjust_sidemove_for_yaw(const vec3& forward, const vec3& local_angle, const float best_speed, user_cmd_t* cmd);
	vec3 calculate_angle(const vec3& source, const vec3& destination, const vec3& view_angles);
	void vector_angles(const vec3& forward, vec3& angles);
	float distance_based_fov(const float distance, const vec3 angle, const user_cmd_t* cmd);
}