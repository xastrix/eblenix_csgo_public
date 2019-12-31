#include "math.h"

#include "globals.h"
#include "interfaces.h"
#include "signatures.h"

static bool screen_transform(const vec3& in, vec3& out)
{
	static uintptr_t view_matrix = 0;

	if (!view_matrix) {
		view_matrix = (uintptr_t)g_sig.s_view_matrix;
		view_matrix = *reinterpret_cast<uintptr_t*>(view_matrix + 0x3) + 0xB0;
	}

	auto matrix = (*(matrix_t*)view_matrix).get_matrix();

	out.x = matrix[0][0] * in.x + matrix[0][1] * in.y + matrix[0][2] * in.z + matrix[0][3];
	out.y = matrix[1][0] * in.x + matrix[1][1] * in.y + matrix[1][2] * in.z + matrix[1][3];
	out.z = 0.0f;

	auto w = matrix[3][0] * in.x + matrix[3][1] * in.y + matrix[3][2] * in.z + matrix[3][3];

	if (w < 0.001f)
		return false;

	auto invw = 1.0f / w;

	out.x *= invw;
	out.y *= invw;

	return true;
}

vec3 Math::calculate_angle(const vec3& source, const vec3& destination, const vec3& view_angles)
{
	vec3 delta = source - destination;
	vec3 angles;

	angles.x = RAD2DEG(std::atanf(delta.z / std::hypotf(delta.x, delta.y))) - view_angles.x;
	angles.y = RAD2DEG(std::atanf(delta.y / delta.x)) - view_angles.y;
	angles.z = 0.0f;

	if (delta.x >= 0.0)
		angles.y += 180.0f;

	return angles;
}

vec3 Math::calculate_angle_alternative(const vec3& a, const vec3& b)
{
	vec3 angles;
	vec3 delta = a - b;

	vector_angles(delta, angles);

	return angles;
}

void Math::sin_cos(float r, float* s, float* c)
{
	*s = std::sin(r);
	*c = std::cos(r);
}

void Math::transform_vector(vec3& a, matrix_t& b, vec3& out)
{
	auto matrix = b.get_matrix();

	out.x = a.dot(matrix[0]) + matrix[0][3];
	out.y = a.dot(matrix[1]) + matrix[1][3];
	out.z = a.dot(matrix[2]) + matrix[2][3];
}

void Math::vector_angles(const vec3& forward, vec3& angles)
{
	if ((forward.y == 0.0f && forward.x == 0.0f))
	{
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
		angles.y = 0.0f;
	}
	else
	{
		angles.x = std::atan2(-forward.z, vec2(forward).length()) * -180 / static_cast<float>(M_PI);
		angles.y = std::atan2(forward.y, forward.x) * 180 / static_cast<float>(M_PI);

		if (angles.y > 90)
			angles.y -= 180;

		else if (angles.y < 90)
			angles.y += 180;

		else if (angles.y == 90)
			angles.y = 0;
	}

	angles.z = 0.0f;
}

void Math::angle_vectors(const vec3& angles, vec3& forward)
{
	float sp, sy, cp, cy;

	sin_cos(DEG2RAD(angles.y), &sy, &cy);
	sin_cos(DEG2RAD(angles.x), &sp, &cp);

	forward.x = cp * cy;
	forward.y = cp * sy;
	forward.z = -sp;
}

bool Math::clamp_angles(vec3& angles)
{
	if (!(std::isfinite(angles.x) && std::isfinite(angles.y) && std::isfinite(angles.z)))
		return false;

	angles.x = std::clamp(angles.x, -89.f, 89.f);
	angles.y = std::clamp(angles.y, -180.f, 180.f);
	angles.z = 0.f;

	return true;
}

bool Math::normalize_angles(vec3& angles)
{
	if (!(std::isfinite(angles.x) && std::isfinite(angles.y) && std::isfinite(angles.z)))
		return false;

	angles.x = std::remainder(angles.x, 360.f);
	angles.y = std::remainder(angles.y, 360.f);

	return true;
}

float Math::distance_based_fov(const float distance, const vec3 angle, const i_user_cmd* cmd)
{
	vec3 aiming_at;
	angle_vectors(cmd->viewangles, aiming_at);
	aiming_at *= distance / 10;

	vec3 aim_at;
	angle_vectors(angle, aim_at);
	aim_at *= distance / 10;

	return aiming_at.distance_to(aim_at);
}

float Math::get_fov(const vec3 vangle, const vec3 angle)
{
	vec3 a0, a1;

	angle_vectors(vangle, a0);
	angle_vectors(angle, a1);

	return RAD2DEG(std::acos((a0.x * a1.x + a0.y * a1.y + a0.z * a1.z) /
		(a0.x * a0.x + a0.y * a0.y + a0.z * a0.z)));
}

bool Math::w2s(const vec3& origin, vec3& screen)
{
	if (!screen_transform(origin, screen))
		return false;

	screen.x = (GLOBAL(screen_width) / 2.0f) + (screen.x * GLOBAL(screen_width)) / 2.0f;
	screen.y = (GLOBAL(screen_height) / 2.0f) - (screen.y * GLOBAL(screen_height)) / 2.0f;

	return true;
}

float Math::get_damage_armor(float damage, const int armor_value)
{
	const auto armor_ratio = 0.5f;
	const auto armor_bonus = 0.5f;

	if (armor_value > 0)
	{
		float new_armor = damage * armor_ratio;
		float armor = (damage - new_armor) * armor_bonus;

		if (armor > static_cast<float>(armor_value)) {
			armor = static_cast<float>(armor_value) * (1.f / armor_bonus);
			new_armor = damage - armor;
		}

		damage = new_armor;
	}

	return damage;
}

void Math::find_position_rotation(float& x, float& y, const float screen_width, const float screen_height)
{
	const vec2  delta = vec2{ screen_width / 2 - x, screen_height / 2 - y };
	const float hypot = std::hypot(delta.x, delta.y);
	const float rotation = RAD2DEG(std::acos(delta.x / hypot));

	x += std::sin(rotation);
	y += std::cos(rotation);
}

void Math::adjust_sidemove_for_yaw(const vec3& forward, const vec3& local_angle, const float best_speed, i_user_cmd* cmd)
{
	auto delta = (std::atan2(forward.y, forward.x) * 180.0f / M_PI) - local_angle.y;

	if (delta > 180) {
		delta -= 360;
	}
	else if (delta < -180) {
		delta += 360;
	}

	if (delta > 0.25) {
		cmd->sidemove = -best_speed;
	}
	else if (delta < -0.25) {
		cmd->sidemove = best_speed;
	}
}