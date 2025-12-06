#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <optional>

#include "vars_def.h"

using var_type = std::variant<bool, int, float>;
using vars_t = std::vector<std::pair<std::string, var_type>>;

struct vars
{
	void init()
	{
		set(V_AIMBOT_ENABLED, false);
		set(V_AIMBOT_SILENT, false);
		set(V_AIMBOT_AUTOPISTOL, false);
		set(V_AIMBOT_VISIBLE_CHECK, true);
		set(V_AIMBOT_SMOKE_CHECK, false);
		set(V_AIMBOT_FLASH_CHECK, false);
		set(V_AIMBOT_TEAMMATE_CHECK, false);
		set(V_AIMBOT_JUMP_CHECK, false);
		set(V_AIMBOT_SCOPE_CHECK, false);
		set(V_AIMBOT_ALL_WEAPONS, false);
		set(V_AIMBOT_ALL_WEAPONS_TYPE, 0);
		set(V_AIMBOT_ALL_WEAPONS_BONE, 0);
		set(V_AIMBOT_ALL_WEAPONS_FOV, 0.0f);
		set(V_AIMBOT_ALL_WEAPONS_SMOOTH, 1.0f);
		set(V_AIMBOT_TYPE_PISTOL, 0);
		set(V_AIMBOT_TYPE_RIFLE, 0);
		set(V_AIMBOT_TYPE_SNIPER, 0);
		set(V_AIMBOT_TYPE_HEAVY, 0);
		set(V_AIMBOT_TYPE_SMG, 0);
		set(V_AIMBOT_BONE_PISTOL, 0);
		set(V_AIMBOT_BONE_RIFLE, 0);
		set(V_AIMBOT_BONE_SNIPER, 0);
		set(V_AIMBOT_BONE_HEAVY, 0);
		set(V_AIMBOT_BONE_SMG, 0);
		set(V_AIMBOT_FOV_PISTOL, 0.0f);
		set(V_AIMBOT_SMOOTH_PISTOL, 1.0f);
		set(V_AIMBOT_FOV_RIFLE, 0.0f);
		set(V_AIMBOT_SMOOTH_RIFLE, 1.0f);
		set(V_AIMBOT_FOV_SNIPER, 0.0f);
		set(V_AIMBOT_SMOOTH_SNIPER, 1.0f);
		set(V_AIMBOT_FOV_HEAVY, 0.0f);
		set(V_AIMBOT_SMOOTH_HEAVY, 1.0f);
		set(V_AIMBOT_FOV_SMG, 0.0f);
		set(V_AIMBOT_SMOOTH_SMG, 1.0f);

		set(V_TRIGGERBOT_ENABLED, false);
		set(V_TRIGGERBOT_FLASH_CHECK, false);
		set(V_TRIGGERBOT_SMOKE_CHECK, false);
		set(V_TRIGGERBOT_TEAMMATE_CHECK, false);
		set(V_TRIGGERBOT_JUMP_CHECK, false);
		set(V_TRIGGERBOT_SCOPE_CHECK, false);
		set(V_TRIGGERBOT_HITBOX_HEAD, true);
		set(V_TRIGGERBOT_HITBOX_BODY, true);
		set(V_TRIGGERBOT_HITBOX_ARMS, true);
		set(V_TRIGGERBOT_HITBOX_LEGS, true);

		set(V_KNIFEBOT_ENABLED, false);
		set(V_KNIFEBOT_FLASH_CHECK, false);
		set(V_KNIFEBOT_SMOKE_CHECK, false);
		set(V_KNIFEBOT_TEAMMATE_CHECK, false);
		set(V_KNIFEBOT_DISTANCE, 65.0f);

		set(V_ESP_ENABLED, false);
		set(V_ESP_TEAM, false);
		set(V_ESP_VISIBLE_ONLY, false);
		set(V_ESP_WALKING_ONLY, false);
		set(V_ESP_RENDER_TYPE, 1);
		set(V_ESP_NAME_ENABLED, false);
		set(V_ESP_NAME_TYPE, 0);
		set(V_ESP_NAME_COL_R, 255);
		set(V_ESP_NAME_COL_G, 255);
		set(V_ESP_NAME_COL_B, 255);
		set(V_ESP_BOX_ENABLED, false);
		set(V_ESP_BOX_TYPE, 0);
		set(V_ESP_BOX_COL_R, 255);
		set(V_ESP_BOX_COL_G, 255);
		set(V_ESP_BOX_COL_B, 255);
		set(V_ESP_HEALTH_ENABLED, false);
		set(V_ESP_HEALTH_TYPE, 0);
		set(V_ESP_HEALTH_BATTERY, true);
		set(V_ESP_HEALTH_COL_R, 255);
		set(V_ESP_HEALTH_COL_G, 255);
		set(V_ESP_HEALTH_COL_B, 255);
		set(V_ESP_WEAPON_ENABLED, false);
		set(V_ESP_WEAPON_TYPE, 0);
		set(V_ESP_WEAPON_COL_R, 255);
		set(V_ESP_WEAPON_COL_G, 255);
		set(V_ESP_WEAPON_COL_B, 255);
		set(V_ESP_ARMOR_ENABLED, false);
		set(V_ESP_ARMOR_COL_R, 255);
		set(V_ESP_ARMOR_COL_G, 255);
		set(V_ESP_ARMOR_COL_B, 255);
		set(V_ESP_SKELETON_ENABLED, false);
		set(V_ESP_SKELETON_TYPE, 0);
		set(V_ESP_SKELETON_COL_R, 255);
		set(V_ESP_SKELETON_COL_G, 255);
		set(V_ESP_SKELETON_COL_B, 255);
		set(V_ESP_BARREL_ENABLED, false);
		set(V_ESP_BARREL_COL_R, 255);
		set(V_ESP_BARREL_COL_G, 255);
		set(V_ESP_BARREL_COL_B, 255);
		set(V_ESP_CROSSHAIR_ENABLED, false);
		set(V_ESP_CROSSHAIR_FOV, 1.0f);
		set(V_ESP_CROSSHAIR_COL_R, 255);
		set(V_ESP_CROSSHAIR_COL_G, 255);
		set(V_ESP_CROSSHAIR_COL_B, 255);
		set(V_ESP_CROSSHAIR_COL_ON_TARGET_R, 255);
		set(V_ESP_CROSSHAIR_COL_ON_TARGET_G, 60);
		set(V_ESP_CROSSHAIR_COL_ON_TARGET_B, 60);
		set(V_ESP_SNAP_LINES_ENABLED, false);
		set(V_ESP_SNAP_LINES_COL_R, 255);
		set(V_ESP_SNAP_LINES_COL_G, 255);
		set(V_ESP_SNAP_LINES_COL_B, 255);
		set(V_ESP_FLAGS_ENABLED, false);
		set(V_ESP_FLAGS_HK, false);
		set(V_ESP_FLAGS_HEALTH, false);
		set(V_ESP_FLAGS_MONEY, false);
		set(V_ESP_FLAGS_AMMO, false);
		set(V_ESP_FLAGS_FLASHED, false);
		set(V_ESP_FLAGS_DEFUSING, false);
		set(V_ESP_FLAGS_DISTANCE, false);
		set(V_ESP_FLAGS_SCOPED, false);
		set(V_ESP_FLAGS_COL_R, 255);
		set(V_ESP_FLAGS_COL_G, 255);
		set(V_ESP_FLAGS_COL_B, 255);
		set(V_ESP_FLAGS_ITEMS_ENABLED, false);
		set(V_ESP_FLAGS_ITEMS_COL_R, 255);
		set(V_ESP_FLAGS_ITEMS_COL_G, 255);
		set(V_ESP_FLAGS_ITEMS_COL_B, 255);

		set(V_VISUALS_ENABLED, true);
		set(V_VISUALS_INTERFACE_STATUS, true);
		set(V_VISUALS_INTERFACE_SPECTATORS, false);
		set(V_VISUALS_WORLD_PROJECTLINES_ENABLED, false);
		set(V_VISUALS_WORLD_PROJECTLINES_DISTANCE, false);
		set(V_VISUALS_WORLD_PROJECTLINES_TYPE, 0);
		set(V_VISUALS_WORLD_PROJECTLINES_COL_R, 255);
		set(V_VISUALS_WORLD_PROJECTLINES_COL_G, 255);
		set(V_VISUALS_WORLD_PROJECTLINES_COL_B, 255);
		set(V_VISUALS_WORLD_ITEMS_ENABLED, false);
		set(V_VISUALS_WORLD_ITEMS_DISTANCE, false);
		set(V_VISUALS_WORLD_ITEMS_COL_R, 255);
		set(V_VISUALS_WORLD_ITEMS_COL_G, 255);
		set(V_VISUALS_WORLD_ITEMS_COL_B, 255);
		set(V_VISUALS_WORLD_WEAPONS_ENABLED, false);
		set(V_VISUALS_WORLD_WEAPONS_DISTANCE, false);
		set(V_VISUALS_WORLD_WEAPONS_AMMO_BAR, false);
		set(V_VISUALS_WORLD_WEAPONS_TYPE, 0);
		set(V_VISUALS_WORLD_WEAPONS_COL_R, 255);
		set(V_VISUALS_WORLD_WEAPONS_COL_G, 255);
		set(V_VISUALS_WORLD_WEAPONS_COL_B, 255);
		set(V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_R, 255);
		set(V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_G, 255);
		set(V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_B, 255);
		set(V_VISUALS_WORLD_C4_ENABLED, false);
		set(V_VISUALS_WORLD_C4_DAMAGE_INDICATOR, false);
		set(V_VISUALS_WORLD_C4_TIME_BAR, false);
		set(V_VISUALS_WORLD_C4_DEFUSE_BAR, false);
		set(V_VISUALS_WORLD_C4_TYPE, 0);
		set(V_VISUALS_WORLD_C4_COL_R, 255);
		set(V_VISUALS_WORLD_C4_COL_G, 255);
		set(V_VISUALS_WORLD_C4_COL_B, 255);
		set(V_VISUALS_WORLD_C4_COL_TIME_BAR_R, 210);
		set(V_VISUALS_WORLD_C4_COL_TIME_BAR_G, 50);
		set(V_VISUALS_WORLD_C4_COL_TIME_BAR_B, 0);
		set(V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_R, 140);
		set(V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_G, 230);
		set(V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_B, 0);
		set(V_VISUALS_WORLD_NIGHTMODE_ENABLED, false);
		set(V_VISUALS_WORLD_NIGHTMODE_WORLD_BRIGHTNESS, 0.30f);
		set(V_VISUALS_WORLD_NIGHTMODE_SKY_BRIGHTNESS, 0.30f);
		set(V_VISUALS_WORLD_NIGHTMODE_MODEL_BRIGHTNESS, 0.70f);
		set(V_VISUALS_WORLD_NIGHTMODE_DECAL_BRIGHTNESS, 1.0f);
		set(V_VISUALS_WORLD_NIGHTMODE_OTHER_BRIGHTNESS, 0.70f);

		set(V_VISUALS_REMOVALS_SMOKE, false);
		set(V_VISUALS_REMOVALS_FLASH, false);
		set(V_VISUALS_REMOVALS_SCOPE, false);
		set(V_VISUALS_REMOVALS_SLEEVES, false);
		set(V_VISUALS_REMOVALS_HANDS, false);

		set(V_GLOW_ENABLED, false);
		set(V_GLOW_VISIBLE_ONLY, false);
		set(V_GLOW_WALKING_ONLY, false);
		set(V_GLOW_TEAM, false);
		set(V_GLOW_HEALTH_BASED, false);
		set(V_GLOW_C4, false);
		set(V_GLOW_ENEMY_COL_R, 255);
		set(V_GLOW_ENEMY_COL_G, 255);
		set(V_GLOW_ENEMY_COL_B, 255);
		set(V_GLOW_ENEMY_COL_A, 255);
		set(V_GLOW_TEAM_COL_R, 255);
		set(V_GLOW_TEAM_COL_G, 255);
		set(V_GLOW_TEAM_COL_B, 255);
		set(V_GLOW_TEAM_COL_A, 255);
		set(V_GLOW_C4_COL_R, 255);
		set(V_GLOW_C4_COL_G, 255);
		set(V_GLOW_C4_COL_B, 255);
		set(V_GLOW_C4_COL_A, 255);

		set(V_CHAMS_ENABLED, false);
		set(V_CHAMS_WALKING_ONLY, false);
		set(V_CHAMS_VISIBLE_ONLY, false);
		set(V_CHAMS_TEAM, false);
		set(V_CHAMS_HEALTH_BASED, false);
		set(V_CHAMS_TYPE, 0);
		set(V_CHAMS_COL_R, 255);
		set(V_CHAMS_COL_G, 255);
		set(V_CHAMS_COL_B, 255);

		set(V_MISC_VISUAL_ENGINE_RADAR, false);
		set(V_MISC_VISUAL_INVENTORY_UNLOCK, false);
		set(V_MISC_VISUAL_REVEAL_RANKS, false);
		set(V_MISC_VISUAL_THIRDPERSON, false);
		set(V_MISC_VISUAL_THIRDPERSON_FOV, 80.0f);
		set(V_MISC_VISUAL_ASPECT_RATIO, false);
		set(V_MISC_VISUAL_ASPECT_RATIO_VALUE, 18.0f);
		set(V_MISC_VISUAL_VIEWMODEL_FOV, 54.0f);
		set(V_MISC_MOVEMENT_BUNNYHOP, false);
		set(V_MISC_MOVEMENT_INFINITE_DUCK, false);
		set(V_MISC_MOVEMENT_AUTO_STRAFE, false);
		set(V_MISC_EVENT_LOGS_PLAYER_HURT, false);
		set(V_MISC_EVENT_LOGS_PLAYER_PURCHASE, false);

		set(V_UI_POS_X, 80);
		set(V_UI_POS_Y, 80);
		set(V_UI_COL_R, 95);
		set(V_UI_COL_G, 115);
		set(V_UI_COL_B, 150);
		set(V_UI_COL_A, 130);
		set(V_UI_MOUSE_WHEEL_NAVIGATION, true);

		set(V_KEYS_ON_TOGGLE_AIMBOT, 0);
		set(V_KEYS_ON_TOGGLE_TRIGGERBOT, 0);
		set(V_KEYS_ON_TOGGLE_THIRDPERSON, 0);
		set(V_KEYS_ON_TOGGLE_PANIC, 0);
		set(V_KEYS_ON_TOGGLE_UI, 0);
		set(V_KEYS_ON_HOLD_BLOCKBOT, 0);
	}
	
	void reset() {
		init();
	}

	void set(const std::string& key, const var_type& value)
	{
		for (auto& v : m_vars)
		{
			if (v.first == key)
			{
				if (v.second != value)
					v.second = std::move(value);

				return;
			}
		}

		m_vars.emplace_back(std::move(key), std::move(value));
	}

	template <typename T>
	std::optional<T> get_as(const std::string& key)
	{
		for (const auto& v : m_vars)
		{
			if (v.first == key)
			{
				std::optional<var_type> opt{ v.second };

				if (opt.has_value()) {
					if (const auto val = std::get_if<T>(&opt.value())) {
						return *val;
					}
				}
			}
		}

		return std::nullopt;
	}

	vars_t get_vars() {
		return m_vars;
	}

	void undo() {
		m_vars.clear();
	}
private:
	vars_t m_vars{};
};

inline vars g_vars;