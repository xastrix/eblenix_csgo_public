#pragma once

#include "base_entity.h"

#include <vector>
// ::shared_ptr
#include <memory>

class c_visuals {
public:
	void run();

	static std::shared_ptr<c_visuals> make_shared() {
		return std::shared_ptr<c_visuals>(new c_visuals());
	}

	void on_do_post_screen_effects();
	void on_scene_end();

private:
	void draw_projectiles(c_base_entity* entity);
	void draw_entity_objects(c_base_entity* entity, std::vector<std::pair<std::string, class_ids>> objects);
	void draw_dropped_weapons(c_base_entity* entity);
	void draw_planted_bomb(c_base_plantedc4* entity, const float explode_time);
};

inline std::shared_ptr<c_visuals> g_visuals = c_visuals::make_shared();