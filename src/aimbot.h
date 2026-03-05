#pragma once

#include "base_entity.h"
#include "engine_trace.h"

// ::shared_ptr
#include <memory>

struct auto_pistol_t {
	auto_pistol_t(const auto_pistol_t&) = delete;
	auto_pistol_t& operator=(const auto_pistol_t&) = delete;

	static auto_pistol_t& get_instance() {
		static auto_pistol_t i{};
		return i;
	}

	void think(user_cmd_t* cmd);

private:
	auto_pistol_t() = default;
};

struct triggerbot_t {
	triggerbot_t(const triggerbot_t&) = delete;
	triggerbot_t& operator=(const triggerbot_t&) = delete;

	static triggerbot_t& get_instance() {
		static triggerbot_t i{};
		return i;
	}

	void think(user_cmd_t* cmd);
	void clip_trace_to_players(const vec3& start, const vec3& end, uint32_t mask, trace_t* old_trace, c_base_entity* ent) const;

private:
	triggerbot_t() = default;

	void calc_trace_to_players(user_cmd_t* cmd, trace_filter* filter, vec3& src, vec3& dst, ray_t& ray, trace_t* trace);
	bool can_shoot(c_base_weapon* weapon, trace_t trace);
	void shoot(user_cmd_t* cmd, c_base_weapon* weapon, trace_t trace);
};

class c_aimbot {
public:
	void run(user_cmd_t* cmd);

	static std::shared_ptr<c_aimbot> make_shared() {
		return std::shared_ptr<c_aimbot>(new c_aimbot());
	}
};

inline std::shared_ptr<c_aimbot> g_aimbot = c_aimbot::make_shared();