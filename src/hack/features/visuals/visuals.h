#pragma once

#include "../../../csgo/sdk/sdk.h"

#include <vector>

struct visuals {
	void instance();
private:
	void draw_status(const char* title, int x_offset);
	void draw_spectators();
	void draw_status_bomb_info(c_base_plantedc4* entity, const float explode_time);
	void draw_projectiles(c_base_entity* entity);
	void draw_entity_objects(c_base_entity* entity, std::vector<std::pair<std::string, class_ids>> objects);
	void draw_dropped_weapons(c_base_entity* entity);
	void draw_planted_bomb(c_base_plantedc4* entity, const float explode_time);
};

extern visuals g_visuals;