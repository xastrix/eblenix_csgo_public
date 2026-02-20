#pragma once

#include "base_entity.h"

#include <vector>

class c_visuals {
public:
	void run();
	void on_do_post_screen_effects();
	void on_scene_end();

private:
	void draw_projectiles(c_base_entity* entity);
	void draw_entity_objects(c_base_entity* entity, std::vector<std::pair<std::string, class_ids>> objects);
	void draw_dropped_weapons(c_base_entity* entity);
	void draw_planted_bomb(c_base_plantedc4* entity, const float explode_time);
};

inline c_visuals g_visuals;