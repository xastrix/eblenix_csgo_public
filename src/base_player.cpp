#include "base_entity.h"

#include "signatures.h"
#include "netvar_manager.h"
#include "interfaces.h"
#include "math.h"
#include "helpers.h"

int c_base_player::get_health()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_iHealth"));
}

vec3 c_base_player::get_velocity()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_vecVelocity[0]"));
}

vec3 c_base_player::get_view_offset()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_vecViewOffset[0]"));
}

vec3 c_base_player::get_eye_pos()
{
	return get_vec_origin() + get_view_offset();
}

int c_base_player::get_tick_base()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_nTickBase"));
}

int c_base_player::get_flags()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_fFlags"));
}

bool c_base_player::is_life_state()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_lifeState")) == 0;
}

int c_base_player::get_move_type()
{
	static int type = netvar_manager::get_address("DT_BaseEntity::m_nRenderMode") + 1;
	return Helpers::read<int>(uintptr_t(this) + type);
}

c_base_weapon* c_base_player::get_active_weapon()
{
	auto weapon = Helpers::read<uintptr_t>(uintptr_t(this)
		+ netvar_manager::get_address("DT_CSPlayer::m_hActiveWeapon")) & 0xFFF;

	if (!weapon)
		return nullptr;

	return g_cs->m_entity_list->get_client_entity<c_base_weapon*>(weapon);
}

bool c_base_player::can_see_entity(c_base_entity* entity, const vec3& pos)
{
	trace_t      tr;
	ray_t        ray;
	trace_filter filter;
	vec3         src = get_eye_pos(), dir = (pos - src);

	filter.m_fp = this;

	normalize(dir);
	ray.init(src, pos);

	g_cs->m_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return tr.m_entity == entity || tr.m_fraction > 0.97f;
}

vec3 c_base_player::get_bone_position(int bone_id)
{
	matrix3x4_t bone_matrix[max_studio_bones];

	if (!setup_bones(bone_matrix, max_studio_bones, bone_used_by_hitbox, 0.0f))
		return vec3{ 0, 0, 0 };

	return vec3{ bone_matrix[bone_id].m[0][3], bone_matrix[bone_id].m[1][3], bone_matrix[bone_id].m[2][3] };
}

vec3 c_base_player::get_hitbox_position(int hitbox_id)
{
	vec3        min{};
	vec3        max{};
	matrix3x4_t bone_matrix[max_studio_bones];

	if (!setup_bones(bone_matrix, max_studio_bones, bone_used_by_hitbox, 0.0f))
		return vec3{ 0, 0, 0 };

	const auto studio_model = g_cs->m_model_info->get_studio_model(get_model());

	if (!studio_model)
		return vec3{ 0, 0, 0 };

	const auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

	if (!hitbox)
		return vec3{ 0, 0, 0 };

	Math::transform_vector(hitbox->m_mins, bone_matrix[hitbox->m_bone], min);
	Math::transform_vector(hitbox->m_maxs, bone_matrix[hitbox->m_bone], max);

	return vec3{ (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f };
}

bool c_base_player::is_moving()
{
	return length(get_velocity()) > 0.1f;
}

bool c_base_player::has_gun_game_immunity()
{
	return Helpers::read<bool>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bGunGameImmunity"));
}

int c_base_player::hitbox_set()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_nHitboxSet"));
}

vec3 c_base_player::aim_punch_angle()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_aimPunchAngle"));
}

bool c_base_player::is_scoped()
{
	return Helpers::read<bool>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bIsScoped"));
}

float c_base_player::get_flash_duration()
{
	return Helpers::read<float>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_flFlashDuration") - 0xC);
}

bool c_base_player::is_flashed()
{
	if (!this)
		return false;

	return get_flash_duration() > 0.0f;
}

int c_base_player::get_armor_value()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_ArmorValue"));
}

vec3 c_base_player::get_eye_angles()
{
	return Helpers::read<vec3>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_angEyeAngles"));
}

bool c_base_player::has_helmet()
{
	return Helpers::read<bool>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bHasHelmet"));
}

bool c_base_player::has_defuser()
{
	return Helpers::read<bool>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bHasDefuser"));
}

bool c_base_player::has_c4()
{
	if (!this)
		return false;

	return reinterpret_cast<bool(__thiscall*)(void*)>(SIG(S_HAS_C4))(this);
}

int c_base_player::get_cash()
{
	return Helpers::read<int>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_iAccount"));
}

bool c_base_player::is_defusing()
{
	return Helpers::read<bool>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bIsDefusing"));
}

float c_base_player::get_duck_amount()
{
	return Helpers::read<float>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_flDuckAmount"));
}

unsigned long c_base_player::get_observer_target()
{
	return Helpers::read<unsigned long>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_hObserverTarget"));
}