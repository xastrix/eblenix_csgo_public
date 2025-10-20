#pragma once

#include "player_info.h"
#include "vec3.h"

#define ENGINE_INTERFACE_VERSION "VEngineClient014"

class c_engine_client {
public:
	int get_local_player();
	int get_player_for_user_id(int user_id);
	void get_player_info(int index, player_info_t* info);
	bool is_in_game();
	bool is_connected();
	bool is_playing();
	bool is_console_visible();
	const char* get_product_version_string();
	void set_view_angles(vec3& angles);
	vec3 get_view_angles();
	void* get_bsp_query();
	void exec_cmd(const char* cmd);
};