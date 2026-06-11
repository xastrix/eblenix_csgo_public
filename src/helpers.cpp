#include "helpers.h"

#include "globals.h"
#include "interfaces.h"
#include "signatures.h"
#include "math.h"
#include "fnv.h"

#include <psapi.h>
#include <unordered_map>
#include <codecvt>

using line_goes_through_smoke_t = bool(__cdecl*)(vec3, vec3);
using netvar_key_value_map = std::unordered_map<uint32_t, uintptr_t>;
using netvar_table_map = std::unordered_map<uint32_t, netvar_key_value_map>;

static std::pair<std::string, std::string> parse_netvar_string(const std::string& netvar);
static void add_props_for_table(
	netvar_table_map& table_map,
	const uint32_t table_name_hash,
	const std::string& table_name,
	recv_table* table,
	const size_t child_offset = 0
);
static void initialize_props(netvar_table_map& table_map);

bool Helpers::is_pistol(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_glock ||
		weapon->item_definition_index() == weapon_elite ||
		weapon->item_definition_index() == weapon_p250 ||
		weapon->item_definition_index() == weapon_tec9 ||
		weapon->item_definition_index() == weapon_cz75a ||
		weapon->item_definition_index() == weapon_deagle ||
		weapon->item_definition_index() == weapon_revolver ||
		weapon->item_definition_index() == weapon_usp_silencer ||
		weapon->item_definition_index() == weapon_hkp2000 ||
		weapon->item_definition_index() == weapon_fiveseven;
}

bool Helpers::is_sniper(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_awp ||
		weapon->item_definition_index() == weapon_ssg08 ||
		weapon->item_definition_index() == weapon_g3sg1 ||
		weapon->item_definition_index() == weapon_scar20;
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
	return weapon->item_definition_index() == weapon_mac10 ||
		weapon->item_definition_index() == weapon_mp7 ||
		weapon->item_definition_index() == weapon_ump45 ||
		weapon->item_definition_index() == weapon_p90 ||
		weapon->item_definition_index() == weapon_bizon ||
		weapon->item_definition_index() == weapon_mp9 ||
		weapon->item_definition_index() == weapon_mp5sd;
}

bool Helpers::is_heavy(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_nova ||
		weapon->item_definition_index() == weapon_xm1014 ||
		weapon->item_definition_index() == weapon_sawedoff ||
		weapon->item_definition_index() == weapon_m249 ||
		weapon->item_definition_index() == weapon_negev ||
		weapon->item_definition_index() == weapon_mag7;
}

bool Helpers::is_rifle(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_ak47 ||
		weapon->item_definition_index() == weapon_aug ||
		weapon->item_definition_index() == weapon_famas ||
		weapon->item_definition_index() == weapon_galilar ||
		weapon->item_definition_index() == weapon_m249 ||
		weapon->item_definition_index() == weapon_m4a1 ||
		weapon->item_definition_index() == weapon_m4a1_silencer ||
		weapon->item_definition_index() == weapon_negev ||
		weapon->item_definition_index() == weapon_sg556;
}

bool Helpers::is_grenade(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_fraggrenade ||
		weapon->item_definition_index() == weapon_hegrenade ||
		weapon->item_definition_index() == weapon_incgrenade ||
		weapon->item_definition_index() == weapon_smokegrenade ||
		weapon->item_definition_index() == weapon_tagrenade ||
		weapon->item_definition_index() == weapon_molotov ||
		weapon->item_definition_index() == weapon_flashbang ||
		weapon->item_definition_index() == weapon_decoy;
}

bool Helpers::is_c4(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_c4;
}

bool Helpers::is_knife(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_knife ||
		weapon->item_definition_index() == weapon_knifegg ||
		weapon->item_definition_index() == weapon_knife_butterfly ||
		weapon->item_definition_index() == weapon_knife_falchion ||
		weapon->item_definition_index() == weapon_knife_survival_bowie ||
		weapon->item_definition_index() == weapon_knife_flip ||
		weapon->item_definition_index() == weapon_knife_ghost ||
		weapon->item_definition_index() == weapon_knife_gut ||
		weapon->item_definition_index() == weapon_bayonet ||
		weapon->item_definition_index() == weapon_knife_gypsy_jackknife ||
		weapon->item_definition_index() == weapon_knife_karambit ||
		weapon->item_definition_index() == weapon_knife_m9_bayonet ||
		weapon->item_definition_index() == weapon_knife_push ||
		weapon->item_definition_index() == weapon_knife_stiletto ||
		weapon->item_definition_index() == weapon_knife_t ||
		weapon->item_definition_index() == weapon_knife_tactical ||
		weapon->item_definition_index() == weapon_knife_ursus ||
		weapon->item_definition_index() == weapon_knife_widowmaker;
}

bool Helpers::is_non_aim(c_base_weapon* weapon)
{
	return weapon->item_definition_index() == weapon_fists ||
		weapon->item_definition_index() == weapon_healthshot ||
		weapon->item_definition_index() == weapon_shield ||
		weapon->item_definition_index() == weapon_taser ||
		weapon->item_definition_index() == weapon_tablet ||
		weapon->item_definition_index() == weapon_hammer ||
		weapon->item_definition_index() == weapon_spanner ||
		weapon->item_definition_index() == weapon_bumpmine ||
		weapon->item_definition_index() == weapon_axe;
}

int Helpers::get_nearest_bone(c_base_player* entity, user_cmd_t* cmd)
{
	auto aim_bone = 0;
	auto best_distance = 360.0f;

	matrix3x4_t matrix[max_studio_bones];

	if (!entity->setup_bones(matrix, max_studio_bones, bone_used_by_hitbox, 0.0f))
		return -1;

	const auto studio_model = g_cs->m_model_info->get_studio_model(entity->get_model());

	if (!studio_model)
		return -1;

	const auto hitbox_set = studio_model->hitbox_set(entity->hitbox_set());

	if (!hitbox_set)
		return -1;

	for (int i = 0; i < hitbox_set->m_hitbox_count; i++)
	{
		if (i >= hitbox_max)
			continue;

		const auto hitbox = hitbox_set->hitbox(i);

		if (!hitbox)
			continue;

		const auto angle = Math::calculate_angle(g_cs->get_local()->get_eye_pos(), vec3(matrix[hitbox->m_bone].m[0][3], matrix[hitbox->m_bone].m[1][3], matrix[hitbox->m_bone].m[2][3]), cmd->m_viewangles);
		const auto this_distance = std::hypotf(angle.x, angle.y);

		if (best_distance > this_distance) {
			best_distance = this_distance;
			aim_bone = hitbox->m_bone;
			continue;
		}
	}

	return aim_bone;
}

int Helpers::find_target_entity(user_cmd_t* cmd, const float fov, vec3& angle)
{
	auto best_fov = fov;
	auto best_target = 0;

	for (int i = 1; i <= g_cs->m_globals->max_clients; i++)
	{
		auto entity = g_cs->m_entity_list->get_client_entity<c_base_player*>(i);

		auto entity_bone_pos = entity->get_bone_position(get_nearest_bone(entity, cmd));
		auto local_eye_pos = g_cs->get_local()->get_eye_pos();

		if (!entity || entity == g_cs->get_local() || entity->get_dormant() || !entity->is_life_state() || entity->has_gun_game_immunity())
			continue;

		angle = Math::calculate_angle(local_eye_pos, entity_bone_pos, cmd->m_viewangles);

		vec3 angles{};
		Math::vector_angles(local_eye_pos - entity_bone_pos, angles);

		const auto distance = Math::distance_based_fov(local_eye_pos.distance_to(entity_bone_pos), angles, cmd);

		if (distance < best_fov) {
			best_fov = distance;
			best_target = i;
		}
	}

	return best_target;
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

bool Helpers::is_behind_smoke(const vec3 start, const vec3 end)
{
	static auto line_goes_through_smoke_fn = reinterpret_cast<line_goes_through_smoke_t>(SIG(S_LINE_GOES_THROUGH_SMOKE));

	if (!line_goes_through_smoke_fn)
		return false;

	return line_goes_through_smoke_fn(start, end);
}

bool Helpers::is_chat_opened()
{
	auto ccsgo_hudchat = g_cs->find_hud_element("CCSGO_HudChat");
	if (!ccsgo_hudchat)
		return false;

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

std::wstring Helpers::stws(const std::string& string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> str{};
	return str.from_bytes(string);
}

std::string Helpers::stutf8(const std::wstring& string)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> str{};
	return str.to_bytes(string);
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

std::vector<std::string> Helpers::parse_json_object(const std::string& in)
{
	std::vector<std::string> result;

	size_t pos{};
	while ((pos = in.find('"', pos)) != std::string::npos)
	{
		size_t begin_pos = pos + 1;
		size_t end_pos = begin_pos;

		while (end_pos < in.size()) {
			end_pos = in.find('"', end_pos);

			if (end_pos == std::string::npos)
				break;

			if (end_pos > 0 && in[end_pos - 1] == '\\') {
				end_pos++;
				continue;
			}

			break;
		}

		if (end_pos == std::string::npos)
			break;

		result.push_back(in.substr(begin_pos, end_pos - begin_pos));
		pos = end_pos + 1;
	}

	return result;
}

void Helpers::console_printf_with_prefix(const char* prefix, const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);

	va_list ap_copy;
	va_copy(ap_copy, ap);
	int needed = std::vsnprintf(nullptr, 0, fmt, ap_copy);
	va_end(ap_copy);

	std::vector<char> buf(static_cast<size_t>(needed) + 1);
	std::vsnprintf(buf.data(), buf.size(), fmt, ap);
	va_end(ap);

	g_cs->m_cvar->console_color_printf(c_color(255, V_UI_COL).get_revert(), "%s ", prefix);
	g_cs->m_cvar->console_printf("%s\n", buf.data());
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

std::string Helpers::get_current_time()
{
	std::time_t now     = std::time(nullptr);
	std::tm* local_time = std::localtime(&now);

	char buf[16];
	sprintf(buf, "%02d:%02d:%02d",
		local_time->tm_hour, local_time->tm_min, local_time->tm_sec);

	return buf;
}

module_t Helpers::get_module(const std::string& name)
{
	module_t ret;
	MODULEINFO mod_info{ 0 };

	auto mod = GetModuleHandleA(name.empty() ? nullptr : name.c_str());
	if (!mod)
		return ret;

	ret.m_base_address = reinterpret_cast<uintptr_t>(mod);
	
	char path[MAX_PATH];
	if (GetModuleFileNameA(mod, path, MAX_PATH)) {
		std::string full_path = path;

		size_t last_slash = full_path.find_last_of("\\/");

		if (last_slash != std::string::npos)
			ret.m_name = full_path.substr(last_slash + 1);

		else
			ret.m_name = full_path;
	}
	else
		ret.m_name = name;

	if (GetModuleInformation(GetCurrentProcess(), mod, &mod_info, sizeof(mod_info)))
		ret.m_size = static_cast<size_t>(mod_info.SizeOfImage);

	return ret;
}

uintptr_t Helpers::get_netvar(const std::string& netvar)
{
	static netvar_table_map map{};

	if (map.empty())
		initialize_props(map);

	const auto[table, prop] = parse_netvar_string(netvar);

	if (map.find(fnv::hash(table.c_str())) == map.end())
		return 0;

	auto& table_map = map.at(fnv::hash(table.c_str()));

	if (table_map.find(fnv::hash(prop.c_str())) == table_map.end())
		return 0;

	return table_map.at(fnv::hash(prop.c_str()));
}

static std::pair<std::string, std::string> parse_netvar_string(const std::string& netvar)
{
	std::string cols = "::";

	size_t pos = netvar.find(cols);
	if (pos == std::string::npos)
		return {};

	return { netvar.substr(0, pos), netvar.substr(pos + cols.size()) };
}

static void add_props_for_table(
	netvar_table_map& table_map,
	const uint32_t table_name_hash,
	const std::string& table_name,
	recv_table* table,
	const size_t child_offset
) {
	for (int i = 0; i < table->m_props_count; i++) {
		auto& prop = table->m_props[i];

		auto name = std::string{ prop.m_prop_name };

		if (prop.m_data_table && prop.m_elements_count > 0) {
			if (name.substr(0, 1) == "0")
				continue;

			add_props_for_table(table_map, table_name_hash, table_name,
				prop.m_data_table, prop.m_offset + child_offset);
		}

		if (name.substr(0, 1) != "m")
			continue;

		const auto name_hash = fnv::hash(prop.m_prop_name);
		const auto offset = static_cast<uintptr_t>(prop.m_offset) + child_offset;

		table_map[table_name_hash][name_hash] = offset;
	}
}

static void initialize_props(netvar_table_map& table_map)
{
	for (auto c = g_cs->m_client->get_client_classes(); c; c = c->m_next_ptr)
	{
		const auto table = c->m_recvtable_ptr;
		const auto table_name = table->m_table_name;
		const auto table_name_hash = fnv::hash(table_name);

		if (table == nullptr)
			continue;

		add_props_for_table(table_map, table_name_hash, table_name, table);
	}
}