#pragma once

#include "../../csgo/sdk/sdk.h"

#define M_PI            3.14159265358979323846
#define M_RADPI         57.295779513082f
#define M_PI_F          ((float)M_PI)
#define RAD2DEG(x)      ((float)(x) * (float)(180.f / M_PI_F))
#define DEG2RAD(x)      ((float)(x) * (float)(M_PI_F / 180.f))
#define MAX_COORD_FLOAT (16384.0f)
#define MIN_COORD_FLOAT (-MAX_COORD_FLOAT)

struct math {
	vec3 calculate_angle(vec3& a, vec3& b);
	vec3 calculate_angle_alternative(vec3& a, vec3& b);
	vec3 calculate_angle(const vec3& source, const vec3& destination, const vec3& view_angles);
	void sin_cos(float r, float* s, float* c);
	void transform_vector(vec3& a, matrix3x4_t& b, vec3& out);
	void vector_angles(vec3& forward, vec3& angles);
	void angle_vectors(vec3& angles, vec3* forward, vec3* right, vec3* up);
	void angle_vectors(vec3& angles, vec3& forward);
	bool clamp_angles(vec3& angles);
	bool normalize_angles(vec3& angles);
	float distance_based_fov(float distance, vec3 angle, i_user_cmd* cmd);
	float get_fov(vec3 vangle, vec3 angle);
	bool w2s(const vec3& origin, vec3& screen);
	float get_damage_armor(float damage, const int armor_value);
	void find_position_rotation(float& x, float& y, const float& screen_width, const float& screen_height);
};

extern math g_math;