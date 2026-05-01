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

private:
	bool can_aim(c_base_player* entity, c_base_weapon* weapon);
	void set_weapon_param(c_base_weapon* weapon);

private:
	float m_fov{},
		  m_smooth{},
		  m_rcs_x{},
		  m_rcs_y{};
	int   m_hitbox_id{},
		  m_type{};
	vec3  m_angle{};
};

inline std::shared_ptr<c_aimbot> g_aimbot = c_aimbot::make_shared();