#pragma once

#include "base_entity.h"

// ::shared_ptr
#include <memory>

class c_aimbot {
public:
	void run(user_cmd_t* cmd);

	static std::shared_ptr<c_aimbot> make_shared() {
		return std::shared_ptr<c_aimbot>(new c_aimbot());
	}

	void do_triggerbot(user_cmd_t* cmd);
	void do_knifebot(user_cmd_t* cmd);

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

inline std::shared_ptr<c_aimbot> g_aimbot = c_aimbot::make_shared();