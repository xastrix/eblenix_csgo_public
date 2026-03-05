#include "helpers.h"

#include "globals.h"
#include "interfaces.h"
#include "signatures.h"
#include "math.h"

using line_goes_through_smoke = bool(__cdecl*)(vec3, vec3);

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
	return weapon->item_definition_index() == weapon_revolver;
}

bool Helpers::is_taser(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_taser;
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
	return weapon->item_definition_index() == weapon_c4;
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

bool Helpers::get_bbox(c_base_player* entity, box& in, const _bbox_types type)
{
	vec3 top, down, s[2]{};

	switch (type) {
	case BT_STATIC: {
		down = entity->get_vec_origin();
		top = down + vec3(0, 0, 72);
		break;
	}
	case BT_DYNAMIC: {
		vec3 air, adjust;

		adjust = vec3(0, 0, -18) * entity->get_duck_amount();
		air = vec3(0, 0, (!(entity->get_flags() & fl_onground) && (entity->get_move_type() != movetype_ladder)) ? 10 : 0);

		down = entity->get_vec_origin() + air;
		top = down + vec3(0, 0, 72) + adjust;
		break;
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
	static auto line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke>(SIG(S_LINE_GOES_THROUGH_SMOKE));

	if (!line_goes_through_smoke_fn)
		return false;

	return line_goes_through_smoke_fn(start_pos, end_pos);
}

bool Helpers::is_chat_opened()
{
	static void* _this = *reinterpret_cast<DWORD**>(SIG(S_HUD_ELEMENT));
	if (!_this)
		return nullptr;

	static auto fn
		= reinterpret_cast<DWORD(__thiscall*)(void*, const char*)>(SIG(S_FIND_HUD_ELEMENT));

	auto ccsgo_hudchat = fn(_this, "CCSGO_HudChat");
	return read<bool>(ccsgo_hudchat + 13);
}

void Helpers::modulate_world_brightness(std::vector<std::pair<std::string, float>> models)
{
	for (auto i = g_cs->m_mat_system->first_material();
		i != g_cs->m_mat_system->invalid_material_handle();
		i = g_cs->m_mat_system->next_material(i)) {

		auto material = g_cs->m_mat_system->get_material(i);

		if (!material)
			continue;

		for (int j = 0; j < models.size(); j++)
		{
			if (std::string{ material->get_texture_group_name() }.find(models[j].first) != std::string::npos)
				material->color_modulate(models[j].second, models[j].second, models[j].second);
		}
	}
}

int Helpers::get_c4_server_time()
{
	return g_cs->m_cvar->get_convar("mp_c4timer")->get_int();
}

float Helpers::get_viewmodel_fov()
{
	return g_cs->m_cvar->get_convar("viewmodel_fov")->get_float();
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

	return "?";
}

std::string Helpers::get_weapon_type_by_index(const int index, const _weapon_esp_types type)
{
	std::vector<std::pair<std::string, int>> weapons = {
		{ type == WE_TEXT ? "Knife"      : "[", weapon_knife_t },
		{ type == WE_TEXT ? "Bayonet"    : "1", weapon_bayonet },
		{ type == WE_TEXT ? "M9"         : "5", weapon_knife_m9_bayonet },
		{ type == WE_TEXT ? "Karambit"   : "4", weapon_knife_karambit },
		{ type == WE_TEXT ? "Gut"        : "3", weapon_knife_gut },
		{ type == WE_TEXT ? "Flip"       : "2", weapon_knife_flip },
		{ type == WE_TEXT ? "Falchion"   : "7", weapon_knife_falchion },
		{ type == WE_TEXT ? "Butterfly"  : "8", weapon_knife_butterfly },
		{ type == WE_TEXT ? "Deagle"     : "A", weapon_deagle },
		{ type == WE_TEXT ? "AUG"        : "U", weapon_aug },
		{ type == WE_TEXT ? "G3SG1"      : "X", weapon_g3sg1 },
		{ type == WE_TEXT ? "Mac10"      : "K", weapon_mac10 },
		{ type == WE_TEXT ? "P90"        : "P", weapon_p90 },
		{ type == WE_TEXT ? "SSG08"      : "a", weapon_ssg08 },
		{ type == WE_TEXT ? "SCAR20"     : "Y", weapon_scar20 },
		{ type == WE_TEXT ? "UMP45"      : "L", weapon_ump45 },
		{ type == WE_TEXT ? "Elite"      : "B", weapon_elite },
		{ type == WE_TEXT ? "Famas"      : "R", weapon_famas },
		{ type == WE_TEXT ? "Five-Seven" : "C", weapon_fiveseven },
		{ type == WE_TEXT ? "Galil"      : "Q", weapon_galilar },
		{ type == WE_TEXT ? "M4"         : "S", weapon_m4a1 },
		{ type == WE_TEXT ? "M4-S"       : "T", weapon_m4a1_silencer },
		{ type == WE_TEXT ? "P250"       : "F", weapon_p250 },
		{ type == WE_TEXT ? "M249"       : "g", weapon_m249 },
		{ type == WE_TEXT ? "XM1014"     : "b", weapon_xm1014 },
		{ type == WE_TEXT ? "Glock"      : "D", weapon_glock },
		{ type == WE_TEXT ? "USP-S"      : "G", weapon_usp_silencer },
		{ type == WE_TEXT ? "P2000"      : "E", weapon_hkp2000 },
		{ type == WE_TEXT ? "AK47"       : "W", weapon_ak47 },
		{ type == WE_TEXT ? "AWP"        : "Z", weapon_awp },
		{ type == WE_TEXT ? "Bizon"      : "M", weapon_bizon },
		{ type == WE_TEXT ? "MAG7"       : "d", weapon_mag7 },
		{ type == WE_TEXT ? "Negev"      : "f", weapon_negev },
		{ type == WE_TEXT ? "Sawedoff"   : "c", weapon_sawedoff },
		{ type == WE_TEXT ? "TEC9"       : "H", weapon_tec9 },
		{ type == WE_TEXT ? "Taser"      : "h", weapon_taser },
		{ type == WE_TEXT ? "Nova"       : "e", weapon_nova },
		{ type == WE_TEXT ? "CZ75"       : "I", weapon_cz75a },
		{ type == WE_TEXT ? "SG556"      : "V", weapon_sg556 },
		{ type == WE_TEXT ? "R8"         : "J", weapon_revolver },
		{ type == WE_TEXT ? "MP7"        : "N", weapon_mp7 },
		{ type == WE_TEXT ? "MP9"        : "O", weapon_mp9 },
		{ type == WE_TEXT ? "MP5"        : "L", weapon_mp5sd },
		{ type == WE_TEXT ? "C4"         : "o", weapon_c4 },
		{ type == WE_TEXT ? "Frag"       : "j", weapon_fraggrenade },
		{ type == WE_TEXT ? "Smoke"      : "k", weapon_smokegrenade },
		{ type == WE_TEXT ? "Molotov"    : "l", weapon_molotov },
		{ type == WE_TEXT ? "Inc"        : "n", weapon_incgrenade },
		{ type == WE_TEXT ? "Flash"      : "i", weapon_flashbang },
		{ type == WE_TEXT ? "Decoy"      : "m", weapon_decoy },
	};

	for (const auto& ss : weapons)
	{
		if (ss.second == index)
			return ss.first;
	}

	return weapons[0].first;
}

#include <codecvt>
std::wstring Helpers::stws(const std::string& string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> str{};
	return str.from_bytes(string);
}

std::string Helpers::xor_encrypt_decrypt(const std::string& data, const std::string& key)
{
	std::string result = data;

	for (int i = 0; i < data.size(); i++) {
		result[i] = data[i] ^ key[i % key.size()];
	}

	return result;
}

std::string Helpers::remove_chars_from_string(std::string string, const std::string& chars)
{
	size_t pos = string.find(chars);

	if (pos != std::string::npos) {
		string.erase(pos, chars.length());
	}

	return string;
}

_wfm_stat Helpers::wait_for_module(int module_index, int ms)
{
	return wait_for_module(module_index, maxModules, ms);
}

_wfm_stat Helpers::wait_for_module(int module_index, int fallback_module_index, int ms)
{
	const auto timeout_ms{ 50000 };
	auto waited_ms{ 0 };

	while (!GetModuleHandleA(GLOBAL(module_list[module_index]).c_str()))
	{
		if (waited_ms >= timeout_ms)
			return WM_TIMEOUT;

		if (fallback_module_index != maxModules)
			GLOBAL(module_list[module_index]) = GLOBAL(module_list[fallback_module_index]);

		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		waited_ms += ms;
	}

	return WM_OK;
}