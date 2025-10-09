#pragma once

#include "sdk.h"

struct esp {
	void instance();
	void on_create_move(i_user_cmd* cmd);
	void on_do_post_screen_effects();
	void on_scene_end();
	void on_round_start();
private:
	bool can_draw_player(int index, float& anim, c_base_player* entity);
	void player_rendering(c_base_player* entity);
	void update_position(int index, const vec3& pos);
	void reset_position(int index);

	vec3  m_stored_pos[65]{};
	float m_anim_progress[65]{};
	bool  m_has_seen[65]{};
	float m_alpha[65]{};

	int   m_armor_min{ 10 };
};

extern esp g_esp;