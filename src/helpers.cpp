#include "helpers.h"

#include "interfaces.h"
#include "signatures.h"
#include "math.h"

#include <vector>
#include <codecvt>

using line_goes_through_smoke = bool(__cdecl*)(vec3, vec3);

int Helpers::get_nearest_bone(c_base_player* entity, i_user_cmd* cmd)
{
	auto aim_bone = 0;
	auto best_distance = 360.0f;

	matrix3x4_t matrix[max_studio_bones];

	if (!entity->setup_bones(matrix, max_studio_bones, bone_used_by_hitbox, 0.0f))
		return -1;

	const auto studio_model = g_csgo.m_model_info->get_studio_model(entity->get_model());

	if (!studio_model)
		return -1;

	const auto hitbox_set = studio_model->hitbox_set(entity->hitbox_set());

	if (!hitbox_set)
		return -1;

	for (int i = 0; i < hitbox_set->hitbox_count; i++)
	{
		if (i >= hitbox_max)
			continue;

		const auto hitbox = hitbox_set->hitbox(i);

		if (!hitbox)
			continue;

		const auto angle = Math::calculate_angle(g_csgo.get_local()->get_eye_pos(), vec3(matrix[hitbox->bone].m[0][3], matrix[hitbox->bone].m[1][3], matrix[hitbox->bone].m[2][3]), cmd->viewangles);
		const auto this_distance = std::hypotf(angle.x, angle.y);

		if (best_distance > this_distance) {
			best_distance = this_distance;
			aim_bone = hitbox->bone;
			continue;
		}
	}

	return aim_bone;
}

int Helpers::find_target_entity(i_user_cmd* cmd, const float fov, vec3& angle)
{
	auto best_fov = fov;
	auto best_target = 0;

	for (int i = 1; i <= g_csgo.m_globals->max_clients; i++) {
		auto entity = reinterpret_cast<c_base_player*>(g_csgo.m_entity_list->get_client_entity(i));

		auto entity_bone_pos = entity->get_bone_position(get_nearest_bone(entity, cmd));
		auto local_eye_pos = g_csgo.get_local()->get_eye_pos();

		if (!entity || entity == g_csgo.get_local() || entity->get_dormant() || !entity->is_life_state() || entity->has_gun_game_immunity())
			continue;

		angle = Math::calculate_angle(local_eye_pos, entity_bone_pos, cmd->viewangles);

		const auto distance = Math::distance_based_fov(local_eye_pos.distance_to(entity_bone_pos),
			Math::calculate_angle_alternative(local_eye_pos, entity_bone_pos), cmd);

		if (distance < best_fov) {
			best_fov = distance;
			best_target = i;
		}
	}

	return best_target;
}

bool Helpers::is_weapon_switching(c_base_weapon* weapon)
{
	if (is_sniper(weapon) && !is_auto(weapon))
		return false;

	static int last_weap = 0;
	static bool weapon_switched = false;

	if (weapon->item_definition_index() != last_weap)
	{
		weapon_switched = true;
		last_weap = weapon->item_definition_index();
	}

	const float server_time = g_csgo.get_local()->get_tick_base() * g_csgo.m_globals->interval_per_tick;
	const auto next_attach_time = weapon->next_primary_attack();

	if (server_time - next_attach_time >= 1.f && weapon_switched)
		weapon_switched = false;

	return weapon_switched;
}

bool Helpers::is_pistol(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_glock, weapon_elite, weapon_p250, weapon_tec9, weapon_cz75a, weapon_deagle,
		weapon_revolver, weapon_usp_silencer, weapon_hkp2000, weapon_fiveseven
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_sniper(c_base_weapon* weapon)
{
	static const std::vector<int> v = { weapon_awp, weapon_ssg08, weapon_g3sg1, weapon_scar20 };
	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_auto(c_base_weapon* weapon)
{
	static const std::vector<int> v = { weapon_g3sg1, weapon_scar20 };
	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_revolver(c_base_weapon* weapon)
{
	static const std::vector<int> v = { weapon_revolver };
	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_taser(c_base_weapon* weapon)
{
	static const std::vector<int> v = { weapon_taser };
	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_smg(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_mac10, weapon_mp7, weapon_ump45, weapon_p90, weapon_bizon,
		weapon_mp9, weapon_mp5sd
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_heavy(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_nova, weapon_xm1014, weapon_sawedoff, weapon_m249,
		weapon_negev, weapon_mag7
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_rifle(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_ak47, weapon_aug, weapon_famas, weapon_galilar, weapon_m249, weapon_m4a1,
		weapon_m4a1_silencer, weapon_negev, weapon_sg556
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_grenade(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_fraggrenade, weapon_hegrenade, weapon_incgrenade,
		weapon_smokegrenade, weapon_tagrenade, weapon_molotov, weapon_flashbang, weapon_decoy
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_c4(c_base_weapon* weapon)
{
	static const std::vector<int> v = { weapon_c4 };
	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_knife(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_knife, weapon_knifegg,
		weapon_knife_butterfly, weapon_knife_falchion,
		weapon_knife_survival_bowie, weapon_knife_flip,
		weapon_knife_ghost, weapon_knife_gut, weapon_bayonet,
		weapon_knife_gypsy_jackknife, weapon_knife_karambit,
		weapon_knife_m9_bayonet, weapon_knife_push, weapon_knife_stiletto,
		weapon_knife_t, weapon_knife_tactical, weapon_knife_ursus, weapon_knife_widowmaker
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::is_non_aim(c_base_weapon* weapon)
{
	static const std::vector<int> v = {
		weapon_fists, weapon_healthshot, weapon_shield,
		weapon_taser, weapon_tablet, weapon_hammer,
		weapon_spanner, weapon_bumpmine, weapon_axe
	};

	return (std::find(v.begin(), v.end(), weapon->item_definition_index()) != v.end());
}

bool Helpers::get_bbox(c_base_player* entity, box& in, const bbox_type type)
{
	vec3 top, down, s[2]{};

	switch (type) {
	case bb_static: {
		down = entity->get_vec_origin();
		top = down + vec3(0, 0, 72);
		break;
	}
	case bb_dynamic: {
		vec3 air, adjust;

		adjust = vec3(0, 0, -18) * entity->get_duck_amount();
		air = vec3(0, 0, (entity->is_in_air() && (entity->get_move_type() != movetype_ladder)) ? 10 : 0);

		down = entity->get_vec_origin() + air;
		top = down + vec3(0, 0, 72) + adjust;
	}
	}

	if (!Math::w2s(top, s[1]) || !Math::w2s(down, s[0]))
		return false;

	vec3 delta{ s[1] - s[0] };

	in.h = std::fabsf(delta.y);
	in.w = in.h / 2.0f;

	in.x = s[1].x - (in.w / 2);
	in.y = s[1].y;

	return true;
}

bool Helpers::is_behind_smoke(const vec3 start_pos, const vec3 end_pos)
{
	static auto line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(g_sig.s_line_goes_through_smoke);

	if (!line_goes_through_smoke_fn)
		return false;

	return line_goes_through_smoke_fn(start_pos, end_pos);
}

int Helpers::get_c4_server_time()
{
	return g_csgo.m_cvar->get_convar("mp_c4timer")->get_int();
}

float Helpers::get_viewmodel_fov()
{
	return g_csgo.m_cvar->get_convar("viewmodel_fov")->get_float();
}

std::string Helpers::hitgroup_name(const int index)
{
	std::vector<std::pair<std::string, int>> hitgroups = {
		{ "head",      hitgroup_head },
		{ "left leg",  hitgroup_leftleg },
		{ "right leg", hitgroup_rightleg },
		{ "stomach",   hitgroup_stomach },
		{ "left arm",  hitgroup_leftarm },
		{ "right arm", hitgroup_rightarm },
		{ "body",      hitgroup_chest },
	};

	for (const auto& ss : hitgroups)
	{
		if (ss.second == index)
			return ss.first;
	}

	return "hitgroup_unknown";
}

std::string Helpers::get_weapon_type_by_index(const int index, const weapon_esp_type type)
{
	std::vector<std::pair<std::string, int>> weapons = {
		{ type == we_text ? "Knife"      : "[", weapon_knife_t },
		{ type == we_text ? "Bayonet"    : "1", weapon_bayonet },
		{ type == we_text ? "M9"         : "5", weapon_knife_m9_bayonet },
		{ type == we_text ? "Karambit"   : "4", weapon_knife_karambit },
		{ type == we_text ? "Gut"        : "3", weapon_knife_gut },
		{ type == we_text ? "Flip"       : "2", weapon_knife_flip },
		{ type == we_text ? "Falchion"   : "7", weapon_knife_falchion },
		{ type == we_text ? "Butterfly"  : "8", weapon_knife_butterfly },
		{ type == we_text ? "Deagle"     : "A", weapon_deagle },
		{ type == we_text ? "AUG"        : "U", weapon_aug },
		{ type == we_text ? "G3SG1"      : "X", weapon_g3sg1 },
		{ type == we_text ? "Mac10"      : "K", weapon_mac10 },
		{ type == we_text ? "P90"        : "P", weapon_p90 },
		{ type == we_text ? "SSG08"      : "a", weapon_ssg08 },
		{ type == we_text ? "SCAR20"     : "Y", weapon_scar20 },
		{ type == we_text ? "UMP45"      : "L", weapon_ump45 },
		{ type == we_text ? "Elite"      : "B", weapon_elite },
		{ type == we_text ? "Famas"      : "R", weapon_famas },
		{ type == we_text ? "Five-Seven" : "C", weapon_fiveseven },
		{ type == we_text ? "Galil"      : "Q", weapon_galilar },
		{ type == we_text ? "M4"         : "S", weapon_m4a1 },
		{ type == we_text ? "M4-S"       : "T", weapon_m4a1_silencer },
		{ type == we_text ? "P250"       : "F", weapon_p250 },
		{ type == we_text ? "M249"       : "g", weapon_m249 },
		{ type == we_text ? "XM1014"     : "b", weapon_xm1014 },
		{ type == we_text ? "Glock"      : "D", weapon_glock },
		{ type == we_text ? "USP-S"      : "G", weapon_usp_silencer },
		{ type == we_text ? "P2000"      : "E", weapon_hkp2000 },
		{ type == we_text ? "AK47"       : "W", weapon_ak47 },
		{ type == we_text ? "AWP"        : "Z", weapon_awp },
		{ type == we_text ? "Bizon"      : "M", weapon_bizon },
		{ type == we_text ? "MAG7"       : "d", weapon_mag7 },
		{ type == we_text ? "Negev"      : "f", weapon_negev },
		{ type == we_text ? "Sawedoff"   : "c", weapon_sawedoff },
		{ type == we_text ? "TEC9"       : "H", weapon_tec9 },
		{ type == we_text ? "Taser"      : "h", weapon_taser },
		{ type == we_text ? "Nova"       : "e", weapon_nova },
		{ type == we_text ? "CZ75"       : "I", weapon_cz75a },
		{ type == we_text ? "SG556"      : "V", weapon_sg556 },
		{ type == we_text ? "R8"         : "J", weapon_revolver },
		{ type == we_text ? "MP7"        : "N", weapon_mp7 },
		{ type == we_text ? "MP9"        : "O", weapon_mp9 },
		{ type == we_text ? "MP5"        : "L", weapon_mp5sd },
		{ type == we_text ? "C4"         : "o", weapon_c4 },
		{ type == we_text ? "Frag"       : "j", weapon_fraggrenade },
		{ type == we_text ? "Smoke"      : "k", weapon_smokegrenade },
		{ type == we_text ? "Molotov"    : "l", weapon_molotov },
		{ type == we_text ? "Inc"        : "n", weapon_incgrenade },
		{ type == we_text ? "Flash"      : "i", weapon_flashbang },
		{ type == we_text ? "Decoy"      : "m", weapon_decoy },
	};

	for (const auto& ss : weapons)
	{
		if (ss.second == index)
			return ss.first;
	}

	return weapons[0].first;
}

std::wstring Helpers::stws(const std::string& string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> str{};
	return str.from_bytes(string);
}

std::string Helpers::xor_encrypt_decrypt(const std::string& data, const std::string& key)
{
	std::string result{ data };

	for (int i = 0; i < data.size(); i++) {
		result[i] = data[i] ^ key[i % key.size()];
	}

	return result;
}

std::string Helpers::remove_chars_from_string(std::string string, const std::string& chars)
{
	size_t pos{ string.find(chars) };

	if (pos != std::string::npos) {
		string.erase(pos, chars.length());
	}

	return string;
}