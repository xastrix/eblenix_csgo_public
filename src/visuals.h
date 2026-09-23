#pragma once

#include "base_entity.h"
#include "helpers.h"

#include <vector>

#define MAX_PLAYER_RENDERING_DISTANCE 1499.0f
#define MAX_PLAYERS                   64

#define ARMOR_MIN_VAL                 10

struct begin_animation_t {
	int   final_val{};
	float elapsed{};
	bool  confirmed{};
};

struct player_snd_t {
	bool  visible{};
	float expire_time{};
};

struct player_esp_t {
	player_esp_t(const player_esp_t&) = delete;
	player_esp_t& operator=(const player_esp_t&) = delete;

	static player_esp_t& get_instance() {
		static player_esp_t i{};
		return i;
	}

	void think();
	void trace_player_sound(int ent_index);

	void reset_positions();

private:
	player_esp_t() = default;

	void calc_player_animation_progress(int index, float& anim, c_base_player* entity);
	void player_rendering(int index, c_base_player* entity, box bbox);
	void update_position(int index, const vec3& pos);
	void reset_position(int index);

	vec3              m_stored_pos[MAX_PLAYERS]{};
	float             m_anim_progress[MAX_PLAYERS]{};
	begin_animation_t m_begin_anims[MAX_PLAYERS]{};
	bool              m_has_seen[MAX_PLAYERS]{};
	float             m_alpha[MAX_PLAYERS]{};
	player_snd_t      m_player_sounds[MAX_PLAYERS]{};
};

struct world_esp_t {
	world_esp_t(const world_esp_t&) = delete;
	world_esp_t& operator=(const world_esp_t&) = delete;

	static world_esp_t& get_instance() {
		static world_esp_t i{};
		return i;
	}

	void think();
	void on_do_post_screen_effects();
	void on_scene_end();

private:
	world_esp_t() = default;

	void draw_projectiles(c_base_entity* entity);
	void draw_entity_objects(c_base_entity* entity, std::vector<std::pair<std::string, _class_ids>> objects);
	void draw_dropped_weapons(c_base_entity* entity);
	void draw_planted_bomb(c_base_plantedc4* entity, const float explode_time);
};

class c_visuals {
public:
	void run();
};

inline c_visuals g_visuals;