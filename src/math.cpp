#include "math.h"

#include "globals.h"
#include "interfaces.h"
#include "signatures.h"
#include "renderer.h"

#include "vec_util.h"

static bool screen_transform(const vec3& in, vec3& out)
{
	static uintptr_t view_matrix{};

	view_matrix = reinterpret_cast<uintptr_t>(SIG(S_VIEW_MATRIX));
	view_matrix = *reinterpret_cast<uintptr_t*>(view_matrix) + 0xB0;

	auto& matrix = (*(matrix3x4_t*)view_matrix);

	out.x = matrix.m[0][0] * in.x + matrix.m[0][1] * in.y + matrix.m[0][2] * in.z + matrix.m[0][3];
	out.y = matrix.m[1][0] * in.x + matrix.m[1][1] * in.y + matrix.m[1][2] * in.z + matrix.m[1][3];
	out.z = 0.0f;

	auto w = matrix.m[3][0] * in.x + matrix.m[3][1] * in.y + matrix.m[3][2] * in.z + matrix.m[3][3];

	if (w < 0.001f)
		return false;

	out.x *= 1.0f / w;
	out.y *= 1.0f / w;

	return true;
}

void Math::transform_vector(vec3& a, matrix3x4_t& b, vec3& out)
{
	out.x = dot(a, b.m[0]) + b.m[0][3];
	out.y = dot(a, b.m[1]) + b.m[1][3];
	out.z = dot(a, b.m[2]) + b.m[2][3];
}

void Math::angle_vectors(const vec3& angles, vec3& forward)
{
	float sp, sy, cp, cy;

	vecUtil::sinCos(DEG2RAD(angles.y), &sy, &cy);
	vecUtil::sinCos(DEG2RAD(angles.x), &sp, &cp);

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
	angles.z = 0.0f;

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

bool Math::w2s(const vec3& origin, vec3& screen)
{
	if (!screen_transform(origin, screen))
		return false;

	vec2 screen_size = g_renderer->get_screen_size();

	screen.x = (screen_size.x / 2.0f) + (screen.x * screen_size.x) / 2.0f;
	screen.y = (screen_size.y / 2.0f) - (screen.y * screen_size.y) / 2.0f;

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

void Math::adjust_sidemove_for_yaw(const vec3& forward, const vec3& local_angle, const float best_speed, user_cmd_t* cmd)
{
	auto delta = (std::atan2(forward.y, forward.x) * 180.0f / M_PI) - local_angle.y;

	if (delta > 180)
		delta -= 360;

	else if (delta < -180)
		delta += 360;

	if (delta > 0.25)
		cmd->sidemove = -best_speed;

	else if (delta < -0.25)
		cmd->sidemove = best_speed;
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

void Math::vector_angles(const vec3& forward, vec3& angles)
{
	if (forward.x == 0.0f && forward.y == 0.0f)
	{
		angles.x = (forward.z > 0.0f) ? 270.0f : 90.0f;
		angles.y = 0.0f;
	}
	else {
		angles.x = std::atan2(-forward.z, length_2d(forward)) * -180 / static_cast<float>(M_PI);
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

float Math::distance_based_fov(const float distance, const vec3 angle, const user_cmd_t* cmd)
{
	vec3 aiming_at;
	angle_vectors(cmd->viewangles, aiming_at);
	aiming_at *= distance / 10;

	vec3 aim_at;
	angle_vectors(angle, aim_at);
	aim_at *= distance / 10;

	return aiming_at.distance_to(aim_at);
}