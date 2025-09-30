#pragma once

#include "../../csgo/sdk/sdk.h"

#include <string>

enum bbox_type {
	bb_static,
	bb_dynamic,
};

enum weapon_esp_type {
	we_text,
	we_icon,
};

struct box {
	int x, y, w, h;
	box() = default;
	box(int x, int y, int w, int h) {
		this->x = x; this->y = y;
		this->w = w; this->h = h;
	}
};

namespace Helpers
{
	int get_nearest_bone(c_base_player* entity, i_user_cmd* cmd);
	int find_target_entity(i_user_cmd* cmd, float& fov, vec3& angle);
	bool is_weapon_switching(c_base_weapon* weapon);
	bool is_pistol(c_base_weapon* weapon);
	bool is_sniper(c_base_weapon* weapon);
	bool is_auto(c_base_weapon* weapon);
	bool is_revolver(c_base_weapon* weapon);
	bool is_taser(c_base_weapon* weapon);
	bool is_smg(c_base_weapon* weapon);
	bool is_heavy(c_base_weapon* weapon);
	bool is_rifle(c_base_weapon* weapon);
	bool is_grenade(c_base_weapon* weapon);
	bool is_c4(c_base_weapon* weapon);
	bool is_knife(c_base_weapon* weapon);
	bool is_non_aim(c_base_weapon* weapon);
	bool get_bbox(c_base_player* entity, box& in, const bbox_type type);
	bool is_behind_smoke(vec3 start_pos, vec3 end_pos);
	int get_c4_server_time();
	float get_viewmodel_fov();
	std::string hitgroup_name(const int index);
	std::string get_weapon_type_by_index(const int index, const weapon_esp_type type);
	std::wstring stws(const std::string& string);
	std::string xor_encrypt_decrypt(const std::string& data, const std::string& key);
	std::string remove_chars_from_string(std::string string, const std::string& chars);
}