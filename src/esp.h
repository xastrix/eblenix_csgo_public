#pragma once

#include "helpers.h"

#define MAX_PLAYER_RENDERING_DISTANCE 1499.0f
#define MAX_PLAYERS 65

struct health_anim_t {
	int   m_final_hp;
	float m_elapsed;
	bool  m_confirmed;
};

struct esp {
	void run();
	void on_round_start_e();
private:
	void calc_player_animation_progress(int index, float& anim, c_base_player* entity);
	void player_rendering(int index, c_base_player* entity, box bbox);
private:
	vec3  m_stored_pos[MAX_PLAYERS]{};
	float m_anim_progress[MAX_PLAYERS]{};
	health_anim_t m_health_anims[MAX_PLAYERS]{};
	bool  m_has_seen[MAX_PLAYERS]{};
	float m_alpha[MAX_PLAYERS]{};

	int   m_armor_min{ 10 };

	void update_position(int index, const vec3& pos);
	void reset_position(int index);
};

inline esp g_esp;