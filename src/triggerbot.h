#pragma once

#include "base_entity.h"
#include "engine_trace.h"

// ::shared_ptr
#include <memory>

class c_triggerbot {
public:
	void run(user_cmd_t* cmd);
	void clip_trace_to_players(const vec3& start, const vec3& end, uint32_t mask, trace_t* old_trace, c_base_entity* ent) const;

	static std::shared_ptr<c_triggerbot> make_shared() {
		return std::shared_ptr<c_triggerbot>(new c_triggerbot());
	}

private:
	void calc_trace_to_players(user_cmd_t* cmd, trace_filter* filter, vec3& src, vec3& dst, ray_t& ray, trace_t* trace);
	bool can_shoot(c_base_weapon* weapon, trace_t trace);
	void shoot(user_cmd_t* cmd, c_base_weapon* weapon, trace_t trace);
};

inline std::shared_ptr<c_triggerbot> g_triggerbot = c_triggerbot::make_shared();