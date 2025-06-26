#pragma once

#include "sdk.h"

struct aimbot {
	void run(user_cmd_t* cmd);
private:
	void set_weapon_param(c_base_weapon* weapon);
	bool can_aiming(c_base_player* entity, c_base_weapon* weapon);
	void set_aim_angle(user_cmd_t* cmd, c_base_player* entity, const vec3& aim_punch);
	void do_autopistol(user_cmd_t* cmd, c_base_weapon* weapon);
private:
	float m_aim_fov{};
	int   m_hitbox_id{};
	float m_aim_smooth{};
	int   m_aim_type{};
	vec3  m_angle{};
};

inline aimbot g_aimbot;