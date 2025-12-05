#include "base_entity.h"

#include "signatures.h"
#include "netvar_manager.h"
#include "interfaces.h"
#include "math.h"
#include "helpers.h"

int c_base_player::get_health()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_iHealth"));
}

vec3 c_base_player::get_velocity()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_vecVelocity[0]"));
}

vec3 c_base_player::get_view_offset()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_vecViewOffset[0]"));
}

vec3 c_base_player::get_eye_pos()
{
	return get_vec_origin() + get_view_offset();
}

int c_base_player::get_tick_base()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_nTickBase"));
}

int c_base_player::get_flags()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_fFlags"));
}

bool c_base_player::is_life_state()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_lifeState")) == 0;
}

int c_base_player::get_move_type()
{
	static int type = netvar_manager::get_address("DT_BaseEntity::m_nRenderMode") + 1;
	return Helpers::read<int>(reinterpret_cast<uintptr_t>(this) + type);
}

c_base_weapon* c_base_player::get_active_weapon()
{
	auto weapon = Helpers::read<uintptr_t>(reinterpret_cast<uintptr_t>(this)
		+ netvar_manager::get_address("DT_CSPlayer::m_hActiveWeapon")) & 0xFFF;

	if (!weapon)
		return nullptr;

	return reinterpret_cast<c_base_weapon*>(g_csgo.m_entity_list->get_client_entity(weapon));
}

bool c_base_player::can_see_entity(const vec3& pos)
{
	trace_t      tr{};
	ray_t        ray{};
	trace_filter filter{};

	filter.skip = this;

	ray.init(get_eye_pos(), pos);
	g_csgo.m_trace->trace_ray(ray, MASK_SHOT | CONTENTS_GRATE, &filter, &tr);

	return (tr.entity == this || tr.flFraction > 0.97f);
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
	matrix3x4_t bone_matrix[max_studio_bones];

	if (!setup_bones(bone_matrix, max_studio_bones, bone_used_by_hitbox, 0.0f))
		return vec3{ 0, 0, 0 };

	const auto studio_model = g_csgo.m_model_info->get_studio_model(get_model());

	if (!studio_model)
		return vec3{ 0, 0, 0 };

	const auto hitbox = studio_model->hitbox_set(0)->hitbox(hitbox_id);

	if (!hitbox)
		return vec3{ 0, 0, 0 };

	vec3 min{};
	vec3 max{};

	Math::transform_vector(hitbox->mins, bone_matrix[hitbox->bone], min);
	Math::transform_vector(hitbox->maxs, bone_matrix[hitbox->bone], max);

	return vec3{ (min.x + max.x) * 0.5f, (min.y + max.y) * 0.5f, (min.z + max.z) * 0.5f };
}

bool c_base_player::is_moving()
{
	return (get_velocity().length() > 0.1f);
}

bool c_base_player::is_in_air()
{
	if (!this)
		return false;

	return !(get_flags() & fl_onground);
}

bool c_base_player::has_gun_game_immunity()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bGunGameImmunity"));
}

int c_base_player::hitbox_set()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_nHitboxSet"));
}

vec3 c_base_player::aim_punch_angle()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_aimPunchAngle"));
}

bool c_base_player::is_scoped()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bIsScoped"));
}

float c_base_player::get_flash_duration()
{
	return *reinterpret_cast<float*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_flFlashDuration"));
}

bool c_base_player::is_flashed()
{
	if (!this)
		return false;

	return (get_flash_duration() > 0.0f);
}

int c_base_player::get_armor_value()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_ArmorValue"));
}

vec3 c_base_player::get_eye_angles()
{
	return *reinterpret_cast<vec3*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_angEyeAngles"));
}

bool c_base_player::has_helmet()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bHasHelmet"));
}

bool c_base_player::has_defuser()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bHasDefuser"));
}

bool c_base_player::has_c4()
{
	return reinterpret_cast<bool(__thiscall*)(void*)>(g_sig.s_has_c4)(this);
}

int c_base_player::get_cash()
{
	return *reinterpret_cast<int*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_iAccount"));
}

bool c_base_player::is_defusing()
{
	return *reinterpret_cast<bool*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_bIsDefusing"));
}

unsigned int* c_base_player::get_my_weapons()
{
	return (unsigned int*)((uintptr_t)this +
		netvar_manager::get_address("DT_CSPlayer::m_hMyWeapons"));
}

float c_base_player::get_duck_amount()
{
	return *reinterpret_cast<float*>(uintptr_t(this) +
		netvar_manager::get_address("DT_CSPlayer::m_flDuckAmount"));
}

unsigned long c_base_player::get_observer_target()
{
	return *reinterpret_cast<unsigned long*>(uintptr_t(this) +
		netvar_manager::get_address("DT_BasePlayer::m_hObserverTarget"));
}