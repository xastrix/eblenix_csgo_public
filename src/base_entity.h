#pragma once

#include <cstdint>

#include "client_class.h"
#include "weapon_info.h"
#include "model.h"
#include "user_cmd.h"
#include "matrix.h"
#include "vec2.h"
#include "vec3.h"

enum move_type {
	movetype_none,
	movetype_isometric,
	movetype_walk,
	movetype_step,
	movetype_fly,
	movetype_flygravity,
	movetype_vphysics,
	movetype_push,
	movetype_noclip,
	movetype_ladder,
	movetype_observer,
	movetype_custom,
	movetype_last = movetype_custom,
};

enum item_definition_data {
	weapon_none = 0,
	weapon_deagle,
	weapon_elite,
	weapon_fiveseven,
	weapon_glock,
	weapon_ak47 = 7,
	weapon_aug,
	weapon_awp,
	weapon_famas,
	weapon_g3sg1,
	weapon_galilar = 13,
	weapon_m249,
	weapon_m4a1 = 16,
	weapon_mac10,
	weapon_p90 = 19,
	weapon_mp5sd = 23,
	weapon_ump45,
	weapon_xm1014,
	weapon_bizon,
	weapon_mag7,
	weapon_negev,
	weapon_sawedoff,
	weapon_tec9,
	weapon_taser,
	weapon_hkp2000,
	weapon_mp7,
	weapon_mp9,
	weapon_nova,
	weapon_p250,
	weapon_shield,
	weapon_scar20,
	weapon_sg556,
	weapon_ssg08,
	weapon_knifegg,
	weapon_knife,
	weapon_flashbang,
	weapon_hegrenade,
	weapon_smokegrenade,
	weapon_molotov,
	weapon_decoy,
	weapon_incgrenade,
	weapon_c4,
	weapon_healthshot = 57,
	weapon_knife_t = 59,
	weapon_m4a1_silencer,
	weapon_usp_silencer,
	weapon_cz75a = 63,
	weapon_revolver,
	weapon_tagrenade = 68,
	weapon_fists,
	weapon_breachcharge,
	weapon_tablet = 72,
	weapon_melee = 74,
	weapon_axe,
	weapon_hammer,
	weapon_spanner = 78,
	weapon_knife_ghost = 80,
	weapon_firebomb,
	weapon_diversion,
	weapon_fraggrenade,
	weapon_snowball,
	weapon_bumpmine,
	weapon_bayonet = 500,
	weapon_knife_css = 503,
	weapon_knife_flip = 505,
	weapon_knife_gut,
	weapon_knife_karambit,
	weapon_knife_m9_bayonet,
	weapon_knife_tactical,
	weapon_knife_falchion = 512,
	weapon_knife_survival_bowie = 514,
	weapon_knife_butterfly,
	weapon_knife_push,
	weapon_knife_ursus = 519,
	weapon_knife_gypsy_jackknife,
	weapon_knife_stiletto = 522,
	weapon_knife_widowmaker,
};

enum team_nums {
	teamT = 2,
	teamCT,
};

enum bomb_site_nums {
	bombA,
	bombB,
};

class c_base_entity {
public:
	void*                 get_renderable();
	void*                 get_networkable();
	c_client_class*       get_client_class();
	bool                  get_dormant();
	int                   get_team_num();
	bool                  setup_bones(matrix_t* out, int max_bones, int mask, float time);
	model_t*              get_model();
	vec3                  get_absolute_origin();
	vec3                  get_vec_origin();
	int                   index();
	bool&                 get_spotted();
	int                   draw_model(int flags, uint8_t alpha);
};

class c_base_weapon : public c_base_entity {
public:
	float                 next_primary_attack();
	int                   clip1_count();
	int                   primary_reserve_ammo_acount();
	short                 item_definition_index();
	weapon_info_t*        get_weapon_data();
};

class c_base_player : public c_base_entity {
public:
	int                   get_health();
	vec3                  get_velocity();
	vec3                  get_view_offset();
	vec3                  get_eye_pos();
	int                   get_tick_base();
	int                   get_flags();
	bool                  is_life_state();
	int                   get_move_type();
	c_base_weapon*        get_active_weapon();
	bool                  can_see_entity(const vec3& pos);
	vec3                  get_bone_position(int bone_id);
	vec3                  get_hitbox_position(int hitbox_id);
	bool                  is_moving();
	bool                  is_in_air();
	bool                  has_gun_game_immunity();
	int                   hitbox_set();
	vec3                  aim_punch_angle();
	bool                  is_scoped();
	float                 get_flash_duration();
	bool                  is_flashed();
	int                   get_armor_value();
	vec3                  get_eye_angles();
	bool                  has_helmet();
	bool                  has_defuser();
	bool                  has_c4();
	int                   get_cash();
	bool                  is_defusing();
	unsigned int*         get_my_weapons();
	float                 get_duck_amount();
	unsigned long         get_observer_target();
};

class c_base_attributable_item : public c_base_entity {
public:
	short                 item_definition_index();
};

class c_base_plantedc4 : public c_base_entity {
public:
	bool                  bomb_ticking();
	float                 defuse_count_down();
	int                   bomb_defuser();
	int                   bomb_site();
	bool                  bomb_defused();
	float                 c4_blow();
	bool                  is_bomb_planted();
	float                 get_defuse_length();
};

struct renderable_info_t {
	c_base_entity*       entity;
	std::byte            u[18];
	uint16_t             flags;
	uint16_t             flags2;
};