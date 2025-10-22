#pragma once

#include "sdk.h"

enum _aim_state {
	as_none,
	as_all_weapons,
};

struct aimbot {
	void run(i_user_cmd* cmd);
private:
	void set_weapon_param(c_base_weapon* weapon);
	bool can_aiming(c_base_player* entity, c_base_weapon* weapon);
	void set_aim_angle(i_user_cmd* cmd, c_base_player* entity, const vec3& aim_punch);
private:
	float      m_aim_fov{};
	int        m_hitbox_id{};
	float      m_aim_smooth{};
	int        m_aim_type{};
	vec3       m_angle{};
	_aim_state m_state{};
};

inline aimbot g_aimbot;