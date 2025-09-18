#pragma once

#include "ui_render.h"

enum _ui_tabs {
	/* Tabs */
	AIMBOT_TAB,
	TRIGGERBOT_TAB,
	ESP_TAB,
	VISUALS_TAB,
	ESSENTIALS_TAB,
	GLOW_TAB,
	CHAMS_TAB,
	KEYS_TAB,
	LOAD_SETTINGS_TAB,
	SAVE_SETTINGS_TAB,

	/* Sub Tabs */
	AIMBOT_WEAPONS_TAB = 9,
	AIMBOT_KNIFE_TAB = 10,

	ESP_NAME_TAB = 5,
	ESP_BOX_TAB = 6,
	ESP_HEALTH_TAB = 7,
	ESP_WEAPON_TAB = 8,
	ESP_SKELETON_TAB = 9,
	ESP_ARMOR_TAB = 10,
	ESP_BARREL_TAB = 11,
	ESP_CROSSHAIR_TAB = 12,
	ESP_OFFSCREEN_LINES_TAB = 13,
	ESP_FLAGS_TAB = 14,

	VISUALS_INTERFACE_TAB = 1,
	VISUALS_WORLD_TAB = 2,
	VISUALS_REMOVALS_TAB = 3,

	ESSENTIALS_VISUAL_TAB = 0,
	ESSENTIALS_MOVEMENT_TAB = 1,
	ESSENTIALS_EVENT_LOGS_TAB = 2,
	ESSENTIALS_MENU_TAB = 3,

	GLOW_COLOR_TAB = 5,

	KEYS_ON_TOGGLE_TAB = 0,

	/* Sub Sub Tabs */
	AIMBOT_WEAPONS_PISTOL_TAB = 1,
	AIMBOT_WEAPONS_RIFLE_TAB = 2,
	AIMBOT_WEAPONS_SNIPER_TAB = 3,
	AIMBOT_WEAPONS_HEAVY_TAB = 4,
	AIMBOT_WEAPONS_SMG_TAB = 5,

	ESP_FLAGS_INVENTORY_ITEMS_TAB = 12,

	VISUALS_WORLD_ITEMS_TAB = 0,
	VISUALS_WORLD_GRENADES_TAB = 1,
	VISUALS_WORLD_WEAPONS_TAB = 2,
	VISUALS_WORLD_BOMB_TAB = 3,

	GLOW_COLOR_ENEMY_TAB = 0,
	GLOW_COLOR_TEAM_TAB = 1,
	GLOW_COLOR_C4_TAB = 2,

	KEYS_ON_TOGGLE_AIM_TAB = 0,
	KEYS_ON_TOGGLE_MISC_TAB = 1,
	KEYS_ON_TOGGLE_OTHER_TAB = 2,
};

struct ui : public ui_render {
	void instance();
	void init_sprites(IDirect3DDevice9* device);

	void on_reset_sprites() {
		for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
			if (i == UI_SPRITE_NONE) continue;
			get_sprite(static_cast<_ui_sprite_list>(i)).on_reset();
		}
	}

	void on_reset_end_sprites() {
		for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
			if (i == UI_SPRITE_NONE) continue;
			get_sprite(static_cast<_ui_sprite_list>(i)).on_reset_end();
		}
	}
private:
	void set_colors();
	void setup();
	void pop();
};

extern ui g_ui;