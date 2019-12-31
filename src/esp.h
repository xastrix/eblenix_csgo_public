#pragma once

#include "sdk.h"

#define MAX_CLIENTS 65
#define MAX_PLAYER_RENDERING_DISTANCE 1499.0f

struct esp {
	void run();
	void on_create_move(i_user_cmd* cmd);
	void on_round_start();
private:
	bool can_draw_player(int index, float& anim, c_base_player* entity);
	void player_rendering(c_base_player* entity);

	vec3  m_stored_pos[MAX_CLIENTS]{};
	float m_anim_progress[MAX_CLIENTS]{};
	bool  m_has_seen[MAX_CLIENTS]{};
	float m_alpha[MAX_CLIENTS]{};

	int   m_armor_min{ 10 };

	void update_position(int index, const vec3& pos);
	void reset_position(int index);
};

inline esp g_esp;