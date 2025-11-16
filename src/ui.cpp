#include "ui.h"

#include "globals.h"
#include "config_manager.h"
#include "input_manager.h"

#include "sprites.hpp"

void ui::init(IDirect3DDevice9* device)
{
	m_sprites[UI_SPRITE_LOGO].init(device, m_ui_logotype, sizeof(m_ui_logotype), 178, 33);
}

void ui::run()
{
	if (!m_opened)
		return;

	setup();

	for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
		if (i == UI_SPRITE_NONE)
			continue;

		m_sprites[i].begin();
	}

	draw(g_vars.get_as<int>(V_UI_POS_X).value(), g_vars.get_as<int>(V_UI_POS_Y).value());

	for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
		if (i == UI_SPRITE_NONE)
			continue;

		m_sprites[i].end();
	}
}

void ui::on_reset_sprites()
{
	for (int i = UI_SPRITE_NONE; i < maxUISprites; i++)
	{
		if (i == UI_SPRITE_NONE)
			continue;

		m_sprites[i].on_reset();
	}
}

void ui::on_reset_end_sprites()
{
	for (int i = UI_SPRITE_NONE; i < maxUISprites; i++)
	{
		if (i == UI_SPRITE_NONE)
			continue;

		m_sprites[i].on_reset_end();
	}
}

void ui::setup()
{
	clear();

	add(L"Aim");
	add(L"Trigger");
	add(L"Esp");
	add(L"Visuals");
	add(L"Essentials");
	add(L"Glow");
	add(L"Chams");
	add(L"Keys", true);

	add(L"Load Settings", false);
	add(L"Save Settings", false);

	switch (m_entry_position) {
	case AIMBOT_TAB: {
		add_bool(L"Enable", V_AIMBOT_ENABLED, true);

		add_bool(L"Silent", V_AIMBOT_SILENT);
		add_bool(L"Auto Pistol", V_AIMBOT_AUTOPISTOL, true);

		add_bool(L"Visible", V_AIMBOT_VISIBLE_CHECK);
		add_bool(L"Team", V_AIMBOT_TEAMMATE_CHECK);
		add_bool(L"Smoke", V_AIMBOT_SMOKE_CHECK);
		add_bool(L"Flash", V_AIMBOT_FLASH_CHECK);
		add_bool(L"Scope", V_AIMBOT_SCOPE_CHECK);
		add_bool(L"Jump", V_AIMBOT_JUMP_CHECK, true);

		add_tab(L"Weapons");
		add_tab(L"Knifes");

		switch (s_entry_position) {
		case AIMBOT_WEAPONS_TAB: {
			add_sub_bool(L"All", V_AIMBOT_ALL_WEAPONS, true);

			std::vector<std::wstring> m_hitboxes{ L"Head", L"Neck", L"Chest", L"Stomach", L"Pelvis" };
			std::vector<std::wstring> m_types{ L"Hitbox", L"Nearest" };

			switch (g_vars.get_as<bool>(V_AIMBOT_ALL_WEAPONS).value()) {
			case true: {
				add_sub_item(L"Type", V_AIMBOT_ALL_WEAPONS_TYPE, m_types, true);

				if (g_vars.get_as<int>(V_AIMBOT_ALL_WEAPONS_TYPE).value() == 0)
					add_sub_item(L"Hitbox", V_AIMBOT_ALL_WEAPONS_BONE, m_hitboxes, true);

				add_sub_float(L"Fov", V_AIMBOT_ALL_WEAPONS_FOV, 0.0f, 180.0f, 0.5f);
				add_sub_float(L"Smooth", V_AIMBOT_ALL_WEAPONS_SMOOTH, 1.0f, 10.0f, 0.1f);
				break;
			}
			case false: {
				add_sub_tab(L"Pistols");
				add_sub_tab(L"Rifles");
				add_sub_tab(L"Snipers");
				add_sub_tab(L"Heavies");
				add_sub_tab(L"Smgs");

				switch (ss_entry_position) {
				case AIMBOT_WEAPONS_PISTOL_TAB: {
					add_sub_sub_item(L"Type", V_AIMBOT_TYPE_PISTOL, m_types, true);

					if (g_vars.get_as<int>(V_AIMBOT_TYPE_PISTOL).value() == 0)
						add_sub_sub_item(L"Hitbox", V_AIMBOT_BONE_PISTOL, m_hitboxes, true);

					add_sub_sub_float(L"Fov", V_AIMBOT_FOV_PISTOL, 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", V_AIMBOT_SMOOTH_PISTOL, 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_RIFLE_TAB: {
					add_sub_sub_item(L"Type", V_AIMBOT_TYPE_RIFLE, m_types, true);

					if (g_vars.get_as<int>(V_AIMBOT_TYPE_RIFLE).value() == 0)
						add_sub_sub_item(L"Hitbox", V_AIMBOT_BONE_RIFLE, m_hitboxes, true);

					add_sub_sub_float(L"Fov", V_AIMBOT_FOV_RIFLE, 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", V_AIMBOT_SMOOTH_RIFLE, 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_SNIPER_TAB: {
					add_sub_sub_item(L"Type", V_AIMBOT_TYPE_SNIPER, m_types, true);

					if (g_vars.get_as<int>(V_AIMBOT_TYPE_SNIPER).value() == 0)
						add_sub_sub_item(L"Hitbox", V_AIMBOT_BONE_SNIPER, m_hitboxes, true);

					add_sub_sub_float(L"Fov", V_AIMBOT_FOV_SNIPER, 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", V_AIMBOT_SMOOTH_SNIPER, 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_HEAVY_TAB: {
					add_sub_sub_item(L"Type", V_AIMBOT_TYPE_HEAVY, m_types, true);

					if (g_vars.get_as<int>(V_AIMBOT_TYPE_HEAVY).value() == 0)
						add_sub_sub_item(L"Hitbox", V_AIMBOT_BONE_HEAVY, m_hitboxes, true);

					add_sub_sub_float(L"Fov", V_AIMBOT_FOV_HEAVY, 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", V_AIMBOT_SMOOTH_HEAVY, 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_SMG_TAB: {
					add_sub_sub_item(L"Type", V_AIMBOT_TYPE_SMG, m_types, true);

					if (g_vars.get_as<int>(V_AIMBOT_TYPE_SMG).value() == 0)
						add_sub_sub_item(L"Hitbox", V_AIMBOT_BONE_SMG, m_hitboxes, true);

					add_sub_sub_float(L"Fov", V_AIMBOT_FOV_SMG, 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", V_AIMBOT_SMOOTH_SMG, 1.0f, 10.0f, 0.1f);
					break;
				}
				}
				break;
			}
			}
			break;
		}
		case AIMBOT_KNIFE_TAB: {
			add_sub_bool(L"Enable", V_KNIFEBOT_ENABLED, true);

			add_sub_bool(L"Team", V_KNIFEBOT_TEAMMATE_CHECK);
			add_sub_bool(L"Smoke", V_KNIFEBOT_SMOKE_CHECK);
			add_sub_bool(L"Flash", V_KNIFEBOT_FLASH_CHECK, true);

			add_sub_float(L"Distance", V_KNIFEBOT_DISTANCE, 40.0f, 80.0f, 0.5f);
			break;
		}
		}
		break;
	}
	case TRIGGERBOT_TAB: {
		add_bool(L"Enable", V_TRIGGERBOT_ENABLED, true);

		add_bool(L"Team", V_TRIGGERBOT_TEAMMATE_CHECK);
		add_bool(L"Smoke", V_TRIGGERBOT_SMOKE_CHECK);
		add_bool(L"Flash", V_TRIGGERBOT_FLASH_CHECK);
		add_bool(L"Scope", V_TRIGGERBOT_SCOPE_CHECK);
		add_bool(L"Jump", V_TRIGGERBOT_JUMP_CHECK, true);

		add_bool(L"Head", V_TRIGGERBOT_HITBOX_HEAD);
		add_bool(L"Body", V_TRIGGERBOT_HITBOX_HEAD);
		add_bool(L"Arms", V_TRIGGERBOT_HITBOX_ARMS);
		add_bool(L"Legs", V_TRIGGERBOT_HITBOX_LEGS);
		break;
	}
	case ESP_TAB: {
		add_bool(L"Enable", V_ESP_ENABLED, true);

		add_bool(L"Team", V_ESP_TEAM);
		add_bool(L"Visible", V_ESP_VISIBLE_ONLY);
		add_bool(L"Walking", V_ESP_WALKING_ONLY, true);

		add_item(L"Render Type", V_ESP_RENDER_TYPE, { L"Static", L"Dynamic" }, true);

		add_tab(L"Name");
		add_tab(L"Box");
		add_tab(L"Health");
		add_tab(L"Weapon");
		add_tab(L"Skeleton");
		add_tab(L"Armor");
		add_tab(L"Barrel");
		add_tab(L"Crosshair");
		add_tab(L"Snap Lines", true);

		add_tab(L"Flags");

		switch (s_entry_position) {
		case ESP_NAME_TAB: {
			add_sub_bool(L"Enable", V_ESP_NAME_ENABLED, true);

			add_sub_item(L"Name Type", V_ESP_NAME_TYPE, { L"Default", L"Lower", L"Upper" }, true);

			add_sub_int(L"Name R", V_ESP_NAME_COL_R, 0, 255, 1);
			add_sub_int(L"Name G", V_ESP_NAME_COL_G, 0, 255, 1);
			add_sub_int(L"Name B", V_ESP_NAME_COL_B, 0, 255, 1);
			break;
		}
		case ESP_BOX_TAB: {
			add_sub_bool(L"Enable", V_ESP_BOX_ENABLED, true);

			add_sub_item(L"Box Type", V_ESP_BOX_TYPE,
				{ L"Default", L"Default (Background)", L"Corner", L"Corner (Background)" }, true);

			add_sub_int(L"Box R", V_ESP_BOX_COL_R, 0, 255, 1);
			add_sub_int(L"Box G", V_ESP_BOX_COL_G, 0, 255, 1);
			add_sub_int(L"Box B", V_ESP_BOX_COL_B, 0, 255, 1);
			break;
		}
		case ESP_HEALTH_TAB: {
			add_sub_bool(L"Enable", V_ESP_HEALTH_ENABLED, true);

			add_sub_item(L"Health Type", V_ESP_HEALTH_TYPE, { L"Default", L"Health Based" }, true);

			switch (g_vars.get_as<int>(V_ESP_HEALTH_TYPE).value()) {
			case 0: {
				add_sub_int(L"Health R", V_ESP_HEALTH_COL_R, 0, 255, 1);
				add_sub_int(L"Health G", V_ESP_HEALTH_COL_G, 0, 255, 1);
				add_sub_int(L"Health B", V_ESP_HEALTH_COL_B, 0, 255, 1, true);
				break;
			}
			}

			add_sub_bool(L"Health Battery", V_ESP_HEALTH_BATTERY);
			break;
		}
		case ESP_WEAPON_TAB: {
			add_sub_bool(L"Enable", V_ESP_WEAPON_ENABLED, true);

			add_sub_item(L"Weapon Type", V_ESP_WEAPON_TYPE, { L"Text", L"Icon" }, true);

			add_sub_int(L"Weapon R", V_ESP_WEAPON_COL_R, 0, 255, 1);
			add_sub_int(L"Weapon G", V_ESP_WEAPON_COL_G, 0, 255, 1);
			add_sub_int(L"Weapon B", V_ESP_WEAPON_COL_B, 0, 255, 1);
			break;
		}
		case ESP_SKELETON_TAB: {
			add_sub_bool(L"Enable", V_ESP_SKELETON_ENABLED, true);

			add_sub_item(L"Skeleton Type", V_ESP_SKELETON_TYPE, { L"Default", L"Health Based" }, true);

			add_sub_int(L"Skeleton R", V_ESP_SKELETON_COL_R, 0, 255, 1);
			add_sub_int(L"Skeleton G", V_ESP_SKELETON_COL_G, 0, 255, 1);
			add_sub_int(L"Skeleton B", V_ESP_SKELETON_COL_B, 0, 255, 1);
			break;
		}
		case ESP_ARMOR_TAB: {
			add_sub_bool(L"Enable", V_ESP_ARMOR_ENABLED, true);

			add_sub_int(L"Armor R", V_ESP_ARMOR_COL_R, 0, 255, 1);
			add_sub_int(L"Armor G", V_ESP_ARMOR_COL_G, 0, 255, 1);
			add_sub_int(L"Armor B", V_ESP_ARMOR_COL_B, 0, 255, 1);
			break;
		}
		case ESP_BARREL_TAB: {
			add_sub_bool(L"Enable", V_ESP_BARREL_ENABLED, true);

			add_sub_int(L"Barrel R", V_ESP_BARREL_COL_R, 0, 255, 1);
			add_sub_int(L"Barrel G", V_ESP_BARREL_COL_G, 0, 255, 1);
			add_sub_int(L"Barrel B", V_ESP_BARREL_COL_B, 0, 255, 1);
			break;
		}
		case ESP_CROSSHAIR_TAB: {
			add_sub_bool(L"Enable", V_ESP_CROSSHAIR_ENABLED, true);

			add_sub_float(L"Crosshair Fov", V_ESP_CROSSHAIR_FOV, 1.0f, 2.0f, 0.1f, true);

			add_sub_int(L"Crosshair No Target R", V_ESP_CROSSHAIR_COL_R, 0, 255, 1);
			add_sub_int(L"Crosshair No Target G", V_ESP_CROSSHAIR_COL_G, 0, 255, 1);
			add_sub_int(L"Crosshair No Target B", V_ESP_CROSSHAIR_COL_B, 0, 255, 1, true);

			add_sub_int(L"Crosshair On Target R", V_ESP_CROSSHAIR_COL_ON_TARGET_R, 0, 255, 1);
			add_sub_int(L"Crosshair On Target G", V_ESP_CROSSHAIR_COL_ON_TARGET_G, 0, 255, 1);
			add_sub_int(L"Crosshair On Target B", V_ESP_CROSSHAIR_COL_ON_TARGET_B, 0, 255, 1);
			break;
		}
		case ESP_SNAP_LINES_TAB: {
			add_sub_bool(L"Enable", V_ESP_SNAP_LINES_ENABLED, true);

			add_sub_int(L"Snap Lines R", V_ESP_SNAP_LINES_COL_R, 0, 255, 1);
			add_sub_int(L"Snap Lines G", V_ESP_SNAP_LINES_COL_G, 0, 255, 1);
			add_sub_int(L"Snap Lines B", V_ESP_SNAP_LINES_COL_B, 0, 255, 1);
			break;
		}
		case ESP_FLAGS_TAB: {
			add_sub_bool(L"Enable", V_ESP_FLAGS_ENABLED, true);

			add_sub_bool(L"HK", V_ESP_FLAGS_HK);
			add_sub_bool(L"Health", V_ESP_FLAGS_HEALTH);
			add_sub_bool(L"Money", V_ESP_FLAGS_MONEY);
			add_sub_bool(L"Ammo", V_ESP_FLAGS_AMMO);
			add_sub_bool(L"Scoped", V_ESP_FLAGS_SCOPED);
			add_sub_bool(L"Flashed", V_ESP_FLAGS_FLASHED);
			add_sub_bool(L"Defusing", V_ESP_FLAGS_DEFUSING);
			add_sub_bool(L"Distance", V_ESP_FLAGS_DISTANCE, true);

			add_sub_int(L"Flags R", V_ESP_FLAGS_COL_R, 0, 255, 1);
			add_sub_int(L"Flags G", V_ESP_FLAGS_COL_G, 0, 255, 1);
			add_sub_int(L"Flags B", V_ESP_FLAGS_COL_B, 0, 255, 1, true);

			add_sub_tab(L"Inventory Items");

			switch (ss_entry_position) {
			case ESP_FLAGS_INVENTORY_ITEMS_TAB: {
				add_sub_sub_bool(L"Enable", V_ESP_FLAGS_ITEMS_ENABLED, true);

				add_sub_sub_int(L"Inventory Items R", V_ESP_FLAGS_ITEMS_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Inventory Items G", V_ESP_FLAGS_ITEMS_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Inventory Items B", V_ESP_FLAGS_ITEMS_COL_B, 0, 255, 1);
				break;
			}
			}
			break;
		}
		}
		break;
	}
	case VISUALS_TAB: {
		add_bool(L"Enable", V_VISUALS_ENABLED, true);

		add_tab(L"Interface");
		add_tab(L"World");
		add_tab(L"Removals");

		switch (s_entry_position) {
		case VISUALS_INTERFACE_TAB: {
			add_sub_bool(L"Draw Status", V_VISUALS_INTERFACE_STATUS);
			add_sub_bool(L"Draw Spectators", V_VISUALS_INTERFACE_SPECTATORS);
			break;
		}
		case VISUALS_WORLD_TAB: {
			add_sub_tab(L"Items");
			add_sub_tab(L"Grenades (In use)");
			add_sub_tab(L"Weapons");
			add_sub_tab(L"Bomb (Planted)");

			switch (ss_entry_position) {
			case VISUALS_WORLD_ITEMS_TAB: {
				add_sub_sub_bool(L"Enable", V_VISUALS_WORLD_ITEMS_ENABLED, true);

				add_sub_sub_bool(L"Items Distance", V_VISUALS_WORLD_ITEMS_DISTANCE, true);

				add_sub_sub_int(L"Items R", V_VISUALS_WORLD_ITEMS_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Items G", V_VISUALS_WORLD_ITEMS_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Items B", V_VISUALS_WORLD_ITEMS_COL_B, 0, 255, 1);
				break;
			}
			case VISUALS_WORLD_GRENADES_TAB: {
				add_sub_sub_bool(L"Enable", V_VISUALS_WORLD_PROJECTLINES_ENABLED, true);

				add_sub_sub_item(L"Grenades Type", V_VISUALS_WORLD_PROJECTLINES_TYPE, { L"Text", L"Icon" }, true);

				add_sub_sub_bool(L"Grenades Distance", V_VISUALS_WORLD_PROJECTLINES_DISTANCE, true);

				add_sub_sub_int(L"Grenades R", V_VISUALS_WORLD_PROJECTLINES_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Grenades G", V_VISUALS_WORLD_PROJECTLINES_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Grenades B", V_VISUALS_WORLD_PROJECTLINES_COL_B, 0, 255, 1);
				break;
			}
			case VISUALS_WORLD_WEAPONS_TAB: {
				add_sub_sub_bool(L"Enable", V_VISUALS_WORLD_WEAPONS_ENABLED, true);

				add_sub_sub_item(L"Weapons Type", V_VISUALS_WORLD_WEAPONS_TYPE, { L"Text", L"Icon" }, true);

				add_sub_sub_bool(L"Weapons Distance", V_VISUALS_WORLD_WEAPONS_DISTANCE);
				add_sub_sub_bool(L"Weapons Ammo Bar", V_VISUALS_WORLD_WEAPONS_AMMO_BAR, true);

				add_sub_sub_int(L"Weapons R", V_VISUALS_WORLD_WEAPONS_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Weapons G", V_VISUALS_WORLD_WEAPONS_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Weapons B", V_VISUALS_WORLD_WEAPONS_COL_B, 0, 255, 1, true);

				add_sub_sub_int(L"Weapons Ammo Bar R", V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Weapons Ammo Bar G", V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Weapons Ammo Bar B", V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_B, 0, 255, 1);
				break;
			}
			case VISUALS_WORLD_BOMB_TAB: {
				add_sub_sub_bool(L"Enable", V_VISUALS_WORLD_C4_ENABLED, true);

				add_sub_sub_item(L"Bomb Type", V_VISUALS_WORLD_C4_TYPE, { L"Text", L"Icon" }, true);

				add_sub_sub_bool(L"Bomb Time Bar", V_VISUALS_WORLD_C4_TIME_BAR);
				add_sub_sub_bool(L"Bomb Defuse Bar", V_VISUALS_WORLD_C4_DEFUSE_BAR);
				add_sub_sub_bool(L"Bomb Damage Indicator", V_VISUALS_WORLD_C4_DAMAGE_INDICATOR, true);

				add_sub_sub_int(L"Bomb R", V_VISUALS_WORLD_C4_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Bomb G", V_VISUALS_WORLD_C4_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Bomb B", V_VISUALS_WORLD_C4_COL_B, 0, 255, 1, true);

				add_sub_sub_int(L"Bomb Time Bar R", V_VISUALS_WORLD_C4_COL_TIME_BAR_R, 0, 255, 1);
				add_sub_sub_int(L"Bomb Time Bar G", V_VISUALS_WORLD_C4_COL_TIME_BAR_G, 0, 255, 1);
				add_sub_sub_int(L"Bomb Time Bar B", V_VISUALS_WORLD_C4_COL_TIME_BAR_B, 0, 255, 1, true);

				add_sub_sub_int(L"Bomb Defuse Bar R", V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_R, 0, 255, 1);
				add_sub_sub_int(L"Bomb Defuse Bar G", V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_G, 0, 255, 1);
				add_sub_sub_int(L"Bomb Defuse Bar B", V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_B, 0, 255, 1);
				break;
			}
			}
			break;
		}
		case VISUALS_REMOVALS_TAB: {
			add_sub_bool(L"Smoke", V_VISUALS_REMOVALS_SMOKE);
			add_sub_bool(L"Flash", V_VISUALS_REMOVALS_FLASH);
			add_sub_bool(L"Hands", V_VISUALS_REMOVALS_HANDS);
			add_sub_bool(L"Sleeves", V_VISUALS_REMOVALS_SLEEVES);
			add_sub_bool(L"Scope", V_VISUALS_REMOVALS_SCOPE);
			break;
		}
		}
		break;
	}
	case ESSENTIALS_TAB: {
		add_tab(L"Visual");
		add_tab(L"Movement");
		add_tab(L"Event Logs");
		add_tab(L"Menu", true);

		switch (s_entry_position) {
		case ESSENTIALS_VISUAL_TAB: {
			add_sub_bool(L"Radar", V_MISC_VISUAL_ENGINE_RADAR);
			add_sub_bool(L"Inventory Unlock", V_MISC_VISUAL_INVENTORY_UNLOCK);
			add_sub_bool(L"Reveal Ranks", V_MISC_VISUAL_REVEAL_RANKS, true);

			add_sub_float(L"Viewmodel Fov", V_MISC_VISUAL_VIEWMODEL_FOV, 54.0f, 130.0f, 1.0f, true);

			add_sub_bool(L"Aspect Ratio", V_MISC_VISUAL_ASPECT_RATIO);
			add_sub_float(L"Aspect Ratio Value", V_MISC_VISUAL_ASPECT_RATIO_VALUE, 1.0f, 3.0f, 0.01f, true);

			add_sub_bool(L"Thirdperson", V_MISC_VISUAL_THIRDPERSON);
			add_sub_float(L"Thirdperson Fov", V_MISC_VISUAL_THIRDPERSON_FOV, 20.0f, 90.0f, 1.0f);
			break;
		}
		case ESSENTIALS_MOVEMENT_TAB: {
			add_sub_bool(L"Bunnyhop", V_MISC_MOVEMENT_BUNNYHOP);
			add_sub_bool(L"Infinite Duck", V_MISC_MOVEMENT_INFINITE_DUCK);
			add_sub_bool(L"Autostrafe", V_MISC_MOVEMENT_AUTO_STRAFE);
			break;
		}
		case ESSENTIALS_EVENT_LOGS_TAB: {
			add_sub_bool(L"Player Hurt", V_MISC_EVENT_LOGS_PLAYER_HURT);
			add_sub_bool(L"Player Purchase", V_MISC_EVENT_LOGS_PLAYER_PURCHASE);
			break;
		}
		case ESSENTIALS_MENU_TAB: {
			add_sub_bool(L"Mouse Wheel Navigation", V_UI_MOUSE_WHEEL_NAVIGATION, true);

			add_sub_int(L"Position X", V_UI_POS_X, 0, 500, 1);
			add_sub_int(L"Position Y", V_UI_POS_Y, 0, 700, 1, true);

			add_sub_int(L"Menu R", V_UI_COL_R, 0, 255, 1);
			add_sub_int(L"Menu G", V_UI_COL_G, 0, 255, 1);
			add_sub_int(L"Menu B", V_UI_COL_B, 0, 255, 1, true);

			add_sub_int(L"Menu A", V_UI_COL_A, 70, 170, 1);
			break;
		}
		}

		add_function(L"Unload", []() {
			g::Unload();
		});
		break;
	}
	case GLOW_TAB: {
		add_bool(L"Enable", V_GLOW_ENABLED, true);

		add_bool(L"Team", V_GLOW_TEAM);
		add_bool(L"Visible", V_GLOW_VISIBLE_ONLY);
		add_bool(L"Walking", V_GLOW_WALKING_ONLY, true);

		add_bool(L"C4", V_GLOW_C4, true);

		add_tab(L"Color");

		switch (s_entry_position) {
		case GLOW_COLOR_TAB: {
			add_sub_tab(L"Enemy");
			add_sub_tab(L"Team");
			add_sub_tab(L"C4", true);

			switch (ss_entry_position) {
			case GLOW_COLOR_ENEMY_TAB: {
				add_sub_sub_int(L"Enemy R", V_GLOW_ENEMY_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Enemy G", V_GLOW_ENEMY_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Enemy B", V_GLOW_ENEMY_COL_B, 0, 255, 1, true);

				add_sub_sub_int(L"Enemy A", V_GLOW_ENEMY_COL_A, 130, 255, 1);
				break;
			}
			case GLOW_COLOR_TEAM_TAB: {
				add_sub_sub_int(L"Team R", V_GLOW_TEAM_COL_R, 0, 255, 1);
				add_sub_sub_int(L"Team G", V_GLOW_TEAM_COL_G, 0, 255, 1);
				add_sub_sub_int(L"Team B", V_GLOW_TEAM_COL_B, 0, 255, 1, true);

				add_sub_sub_int(L"Team A", V_GLOW_TEAM_COL_A, 130, 255, 1);
				break;
			}
			case GLOW_COLOR_C4_TAB: {
				add_sub_sub_int(L"C4 R", V_GLOW_C4_COL_R, 0, 255, 1);
				add_sub_sub_int(L"C4 G", V_GLOW_C4_COL_G, 0, 255, 1);
				add_sub_sub_int(L"C4 B", V_GLOW_C4_COL_B, 0, 255, 1, true);

				add_sub_sub_int(L"C4 A", V_GLOW_C4_COL_A, 130, 255, 1);
				break;
			}
			}

			add_sub_bool(L"Health Based", V_GLOW_HEALTH_BASED);
			break;
		}
		}

		break;
	}
	case CHAMS_TAB: {
		add_bool(L"Enable", V_CHAMS_ENABLED, true);

		add_bool(L"Team", V_CHAMS_TEAM);
		add_bool(L"Visible", V_CHAMS_VISIBLE_ONLY);
		add_bool(L"Walking", V_CHAMS_WALKING_ONLY, true);

		add_item(L"Type", V_CHAMS_TYPE, { L"1", L"2", L"3" }, true);

		add_int(L"Chams R", V_CHAMS_COL_R, 0, 255, 1);
		add_int(L"Chams G", V_CHAMS_COL_G, 0, 255, 1);
		add_int(L"Chams B", V_CHAMS_COL_B, 0, 255, 1, true);

		add_bool(L"Health Based", V_CHAMS_HEALTH_BASED);
		break;
	}
	case KEYS_TAB: {
		add_tab(L"On Toggle");
		add_tab(L"On Hold");

		switch (s_entry_position) {
		case KEYS_ON_TOGGLE_TAB: {
			add_sub_tab(L"Aim");
			add_sub_tab(L"Misc");
			add_sub_tab(L"Other");

			switch (ss_entry_position) {
			case KEYS_ON_TOGGLE_AIM_TAB: {
				add_sub_sub_key(L"Aimbot", V_KEYS_ON_TOGGLE_AIMBOT);
				add_sub_sub_key(L"Trigger", V_KEYS_ON_TOGGLE_TRIGGERBOT);
				break;
			}
			case KEYS_ON_TOGGLE_MISC_TAB: {
				add_sub_sub_key(L"Thirdperson", V_KEYS_ON_TOGGLE_THIRDPERSON);
				break;
			}
			case KEYS_ON_TOGGLE_OTHER_TAB: {
				add_sub_sub_key(L"Menu", V_KEYS_ON_TOGGLE_UI);
				add_sub_sub_key(L"Panic", V_KEYS_ON_TOGGLE_PANIC);
				break;
			}
			}
			break;
		}
		case KEYS_ON_HOLD_TAB: {
			add_sub_tab(L"Misc");

			switch (ss_entry_position) {
			case KEYS_ON_HOLD_MISC_TAB: {
				add_sub_sub_key(L"Blockbot", V_KEYS_ON_HOLD_BLOCKBOT);
				break;
			}
			}
			break;
		}
		}
		break;
	}
	case LOAD_SETTINGS_TAB: {
		for (const auto& cfg : cfg_list) {
			add_function(cfg, [cfg]() {
				config_manager::load_config(cfg);
			});
		}
		break;
	}
	case SAVE_SETTINGS_TAB: {
		for (const auto& cfg : cfg_list) {
			add_function(cfg, [cfg]() {
				config_manager::save_config(cfg);
			});
		}
		break;
	}
	}
}

void ui::handle_toggle_keys(unsigned int k)
{
	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_UI).value())
		m_opened = !m_opened;

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_AIMBOT).value())
		g_vars.set(V_AIMBOT_ENABLED, !g_vars.get_as<bool>(V_AIMBOT_ENABLED).value());

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_TRIGGERBOT).value())
		g_vars.set(V_TRIGGERBOT_ENABLED, !g_vars.get_as<bool>(V_TRIGGERBOT_ENABLED).value());

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_THIRDPERSON).value())
		g_vars.set(V_MISC_VISUAL_THIRDPERSON, !g_vars.get_as<bool>(V_MISC_VISUAL_THIRDPERSON).value());

	if (k == g_vars.get_as<int>(V_KEYS_ON_TOGGLE_PANIC).value())
		GLOBAL(panic) = !GLOBAL(panic);
}

void ui::handle_input(unsigned int k)
{
	if (!s_opened[UI_SUB_POS])
	{
		if (k == VK_RIGHT)
		{
			s_entry_position = 0;
			s_opened[UI_SUB_POS] = true;
		}
		else if (k == VK_UP)
		{
			if (m_entry_position > 0)
				--m_entry_position;
		}
		else if (k == VK_DOWN)
		{
			if (m_entry_position < (m_entry_size - 1))
				++m_entry_position;
		}
	}
	else
	{
		if (!s_opened[UI_SUB_SUB_POS])
		{
			if (k == VK_UP)
			{
				if (!subm_entry[s_entry_position].m_key_hold)
				{
					if (s_entry_position > 0)
						--s_entry_position;
					else
						s_opened[UI_SUB_POS] = false;
				}
			}
			else if (k == VK_DOWN)
			{
				if (!subm_entry[s_entry_position].m_key_hold)
				{
					if (s_entry_position < (s_entry_size - 1))
						++s_entry_position;
				}
			}
			else if (k == VK_LEFT)
			{
				if (subm_entry[s_entry_position].m_state == UI_BOOL_STATE)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, false);
				}
				else if (subm_entry[s_entry_position].m_state == UI_INT_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value -= subm_entry[s_entry_position].m_int_step);

					if (value < subm_entry[s_entry_position].m_int_min)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_min);
				}
				else if (subm_entry[s_entry_position].m_state == UI_FLOAT_STATE)
				{
					auto value = g_vars.get_as<float>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value -= subm_entry[s_entry_position].m_float_step);

					if (value < subm_entry[s_entry_position].m_float_min)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_float_min);
				}
				else if (subm_entry[s_entry_position].m_state == UI_ITEM_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value -= subm_entry[s_entry_position].m_int_step);

					if (value < subm_entry[s_entry_position].m_int_min)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_min);
				}
			}
			else if (k == VK_RIGHT)
			{
				if (subm_entry[s_entry_position].m_state == UI_BOOL_STATE)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, true);
				}
				else if (subm_entry[s_entry_position].m_state == UI_INT_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value += subm_entry[s_entry_position].m_int_step);

					if (value > subm_entry[s_entry_position].m_int_max)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_max);
				}
				else if (subm_entry[s_entry_position].m_state == UI_FLOAT_STATE)
				{
					auto value = g_vars.get_as<float>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value += subm_entry[s_entry_position].m_float_step);

					if (value > subm_entry[s_entry_position].m_float_max)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_float_max);
				}
				else if (subm_entry[s_entry_position].m_state == UI_ITEM_STATE)
				{
					auto value = g_vars.get_as<int>(subm_entry[s_entry_position].m_var).value();

					g_vars.set(subm_entry[s_entry_position].m_var, value += subm_entry[s_entry_position].m_int_step);

					if (value > subm_entry[s_entry_position].m_int_max)
						g_vars.set(subm_entry[s_entry_position].m_var, subm_entry[s_entry_position].m_int_max);
				}
				else if (subm_entry[s_entry_position].m_state == UI_TAB_STATE)
				{
					ss_entry_position = 0;
					s_opened[UI_SUB_SUB_POS] = true;
				}
			}
			else if (k == VK_BACK)
			{
				if (subm_entry[s_entry_position].m_state == UI_KEY_STATE &&
					subm_entry[s_entry_position].m_key_hold)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, 0);
					subm_entry[s_entry_position].m_key_hold = false;
				}
				else
					s_opened[UI_SUB_POS] = false;
			}
			else if (k == VK_RETURN)
			{
				if (subm_entry[s_entry_position].m_state == UI_KEY_STATE)
				{
					subm_entry[s_entry_position].m_key_hold = true;
				}
				else if (subm_entry[s_entry_position].m_state == UI_FUNCTION_STATE)
				{
					subm_entry[s_entry_position].m_fn();
				}
			}
			else
			{
				if (subm_entry[s_entry_position].m_state == UI_KEY_STATE &&
					subm_entry[s_entry_position].m_key_hold)
				{
					g_vars.set(subm_entry[s_entry_position].m_var, static_cast<int>(k));
					subm_entry[s_entry_position].m_key_hold = false;
				}
			}
		}
		else
		{
			if (!s_opened[UI_SUB_SUB_SUB_POS])
			{
				if (k == VK_UP)
				{
					if (!ssubm_entry[ss_entry_position].m_key_hold)
					{
						if (ss_entry_position > 0)
							--ss_entry_position;
						else
							s_opened[UI_SUB_SUB_POS] = false;
					}
				}
				else if (k == VK_DOWN)
				{
					if (!ssubm_entry[ss_entry_position].m_key_hold)
					{
						if (ss_entry_position < (ss_entry_size - 1))
							++ss_entry_position;
					}
				}
				else if (k == VK_LEFT)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, false);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value -= ssubm_entry[ss_entry_position].m_int_step);

						if (value < ssubm_entry[ss_entry_position].m_int_min)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_min);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value -= ssubm_entry[ss_entry_position].m_float_step);

						if (value < ssubm_entry[ss_entry_position].m_float_min)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_float_min);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value -= ssubm_entry[ss_entry_position].m_int_step);

						if (value < ssubm_entry[ss_entry_position].m_int_min)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_min);
					}
				}
				else if (k == VK_RIGHT)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, true);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value += ssubm_entry[ss_entry_position].m_int_step);

						if (value > ssubm_entry[ss_entry_position].m_int_max)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_max);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value += ssubm_entry[ss_entry_position].m_float_step);

						if (value > ssubm_entry[ss_entry_position].m_float_max)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_float_max);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(ssubm_entry[ss_entry_position].m_var).value();

						g_vars.set(ssubm_entry[ss_entry_position].m_var, value += ssubm_entry[ss_entry_position].m_int_step);

						if (value > ssubm_entry[ss_entry_position].m_int_max)
							g_vars.set(ssubm_entry[ss_entry_position].m_var, ssubm_entry[ss_entry_position].m_int_max);
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_TAB_STATE)
					{
						sss_entry_position = 0;
						s_opened[UI_SUB_SUB_SUB_POS] = true;
					}
				}
				else if (k == VK_BACK)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_KEY_STATE &&
						ssubm_entry[ss_entry_position].m_key_hold)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, 0);
						ssubm_entry[ss_entry_position].m_key_hold = false;
					}
					else
						s_opened[UI_SUB_SUB_POS] = false;
				}
				else if (k == VK_RETURN)
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_KEY_STATE)
					{
						ssubm_entry[ss_entry_position].m_key_hold = true;
					}
					else if (ssubm_entry[ss_entry_position].m_state == UI_FUNCTION_STATE)
					{
						ssubm_entry[ss_entry_position].m_fn();
					}
				}
				else
				{
					if (ssubm_entry[ss_entry_position].m_state == UI_KEY_STATE &&
						ssubm_entry[ss_entry_position].m_key_hold)
					{
						g_vars.set(ssubm_entry[ss_entry_position].m_var, static_cast<int>(k));
						ssubm_entry[ss_entry_position].m_key_hold = false;
					}
				}
			}
			else
			{
				if (k == VK_UP)
				{
					if (!sssubm_entry[sss_entry_position].m_key_hold)
					{
						if (sss_entry_position > 0)
							--sss_entry_position;
						else
							s_opened[UI_SUB_SUB_SUB_POS] = false;
					}
				}
				else if (k == VK_DOWN)
				{
					if (!sssubm_entry[sss_entry_position].m_key_hold)
					{
						if (sss_entry_position < (sss_entry_size - 1))
							++sss_entry_position;
					}
				}
				else if (k == VK_LEFT)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, false);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value -= sssubm_entry[sss_entry_position].m_int_step);

						if (value < sssubm_entry[sss_entry_position].m_int_min)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_min);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value -= sssubm_entry[sss_entry_position].m_float_step);

						if (value < sssubm_entry[sss_entry_position].m_float_min)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_float_min);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value -= sssubm_entry[sss_entry_position].m_int_step);

						if (value < sssubm_entry[sss_entry_position].m_int_min)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_min);
					}
				}
				else if (k == VK_RIGHT)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_BOOL_STATE)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, true);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_INT_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value += sssubm_entry[sss_entry_position].m_int_step);

						if (value > sssubm_entry[sss_entry_position].m_int_max)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_max);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_FLOAT_STATE)
					{
						auto value = g_vars.get_as<float>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value += sssubm_entry[sss_entry_position].m_float_step);

						if (value > sssubm_entry[sss_entry_position].m_float_max)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_float_max);
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_ITEM_STATE)
					{
						auto value = g_vars.get_as<int>(sssubm_entry[sss_entry_position].m_var).value();

						g_vars.set(sssubm_entry[sss_entry_position].m_var, value += sssubm_entry[sss_entry_position].m_int_step);

						if (value > sssubm_entry[sss_entry_position].m_int_max)
							g_vars.set(sssubm_entry[sss_entry_position].m_var, sssubm_entry[sss_entry_position].m_int_max);
					}
				}
				else if (k == VK_BACK)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_KEY_STATE &&
						sssubm_entry[sss_entry_position].m_key_hold)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, 0);
						sssubm_entry[sss_entry_position].m_key_hold = false;
					}
					else
						s_opened[UI_SUB_SUB_SUB_POS] = false;
				}
				else if (k == VK_RETURN)
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_KEY_STATE)
					{
						sssubm_entry[sss_entry_position].m_key_hold = true;
					}
					else if (sssubm_entry[sss_entry_position].m_state == UI_FUNCTION_STATE)
					{
						sssubm_entry[sss_entry_position].m_fn();
					}
				}
				else
				{
					if (sssubm_entry[sss_entry_position].m_state == UI_KEY_STATE &&
						sssubm_entry[sss_entry_position].m_key_hold)
					{
						g_vars.set(sssubm_entry[sss_entry_position].m_var, static_cast<int>(k));
						sssubm_entry[sss_entry_position].m_key_hold = false;
					}
				}
			}
		}
	}
}

void ui::draw(int x, int y)
{
	int HeadBoxWidth = 178;
	int HeadBoxHeight = 22;

	int MenuBoxX = x + 1;
	int MenuTextX = MenuBoxX + 6;

	int MenuBoxY = 0;
	int MenuTextY = 0;

	int HeadY = y;

	y += HeadBoxHeight + 4;

	int MenuLogoX = x + 1;
	int MenuLogoY = y - m_sprites[UI_SPRITE_LOGO].get_height() + 1;

	auto draw_float = [](float value, int x, int y, color_t color) {
		char ss[256];
		sprintf_s(ss, "%.2f", value);

		const auto font = g_render.get_font(Tahoma12px);
		const auto string_width = g_render.get_text_width(ss, font);

		g_render.draw_string(ss, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
	};

	auto draw_bool = [](bool value, int x, int y, color_t color) {
		g_render.draw_filled_rect(x - 38, y - 11, 26, 14, color_t(40, 40, 40));

		value ?
			g_render.draw_filled_rect(x - 25,
				y - 10, 12, 12, color) :

			g_render.draw_filled_rect(x - 37,
				y - 10, 12, 12, color_t(68, 68, 70));
	};

	auto draw_int = [](int value, int x, int y, color_t color) {
		const auto font = g_render.get_font(Tahoma12px);
		const auto string_width = g_render.get_text_width(std::to_string(value), font);

		g_render.draw_string(std::to_string(value), x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
	};

	auto draw_hotkey = [](int k, int x, int x_text, int y, int y_text, int h, bool hold, color_t color, color_t background_color) {
		const auto kss = g_input.virtual_key_to_wstring(k);

		const auto font = g_render.get_font(Tahoma12px);
		const auto key_string_width = g_render.get_text_widthW(hold ? L"---" : kss, font);

		g_render.draw_filled_rect(x - key_string_width - 47, y, 47 + key_string_width, h, background_color);

		g_render.draw_stringW(hold ? L"---" : kss, x_text - key_string_width - 30,
			y_text - 11, font, TEXT_OUTLINE, color);
	};

	auto draw_item = [](std::wstring item, int x, int y, color_t color) {
		if (item.length() > 20)
			item = item.substr(0, 20) + L"...";

		const auto font = g_render.get_font(Tahoma12px);
		const auto string_width = g_render.get_text_widthW(item, font);

		g_render.draw_stringW(item, x - string_width - 20, y - 11, font, TEXT_OUTLINE, color);
	};

	m_colors[UI_MAIN_COL] = color_t(V_UI_COL);
	m_colors[UI_PRIMARY_COL] = color_t(V_UI_COL, 130);
	m_colors[UI_SHADOW_COL] = color_t(20, 20, 20, g_vars.get_as<int>(V_UI_COL_A).value());
	m_colors[UI_TEXT_COL] = color_t(253, 253, 253);

	m_sprites[UI_SPRITE_LOGO].draw(MenuLogoX, MenuLogoY);

	for (int i = 0; i < m_entry_size; i++)
	{
		MenuBoxY = y + 1;
		MenuTextY = MenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		g_render.draw_filled_rect(MenuBoxX, MenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

		if (i == m_entry_position)
		{
			if (!s_opened[UI_SUB_POS])
				g_render.draw_filled_rect(MenuBoxX, MenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);

			g_render.draw_filled_rect(MenuBoxX, MenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
		}

		g_render.draw_stringW(menu_entry[i].m_name, MenuTextX + 3, MenuTextY - 11, g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

		g_render.draw_string("+", MenuTextX + HeadBoxWidth - 20, MenuTextY - 11, g_render.get_font(Tahoma12px),
			TEXT_OUTLINE, (i == m_entry_position && s_opened[UI_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);

		y += HeadBoxHeight + 1;

		if (menu_entry[i].m_space)
			y += HeadBoxHeight + 1;
	}

	x += HeadBoxWidth + 1;

	if (s_opened[UI_SUB_POS])
	{
		int SubMenuBoxX = x + 1;
		int SubMenuBoxY = HeadY + HeadBoxHeight + 5;
		int SubMenuTextX = SubMenuBoxX + 6;
		int SubMenuTextY = SubMenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		for (int i = 0; i < s_entry_size; i++)
		{
			g_render.draw_filled_rect(SubMenuBoxX, SubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

			if (i == s_entry_position)
			{
				if (!s_opened[UI_SUB_SUB_POS])
					g_render.draw_filled_rect(SubMenuBoxX, SubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);

				g_render.draw_filled_rect(SubMenuBoxX, SubMenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
			}

			g_render.draw_stringW(subm_entry[i].m_name, SubMenuTextX + 3, SubMenuTextY - 11,
				g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (subm_entry[i].m_state == UI_TAB_STATE)
			{
				g_render.draw_string("+", SubMenuTextX + HeadBoxWidth - 20, SubMenuTextY - 11, g_render.get_font(Tahoma12px),
					TEXT_OUTLINE, (i == s_entry_position && s_opened[UI_SUB_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else
			{
				if (subm_entry[i].m_state != UI_FUNCTION_STATE)
				{
					if (subm_entry[i].m_state == UI_BOOL_STATE)
					{
						draw_bool(g_vars.get_as<bool>(subm_entry[i].m_var).value(),
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_PRIMARY_COL]);
					}
					else if (subm_entry[i].m_state == UI_INT_STATE)
					{
						draw_int(g_vars.get_as<int>(subm_entry[i].m_var).value(),
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (subm_entry[i].m_state == UI_FLOAT_STATE)
					{
						draw_float(g_vars.get_as<float>(subm_entry[i].m_var).value(),
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (subm_entry[i].m_state == UI_KEY_STATE)
					{
						draw_hotkey(g_vars.get_as<int>(subm_entry[i].m_var).value(),
							SubMenuBoxX + HeadBoxWidth, SubMenuTextX + HeadBoxWidth, SubMenuBoxY,
							SubMenuTextY, HeadBoxHeight, subm_entry[i].m_key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
					}
					else if (subm_entry[i].m_state == UI_ITEM_STATE)
					{
						draw_item(subm_entry[i].m_items[g_vars.get_as<int>(subm_entry[i].m_var).value()],
							SubMenuTextX + HeadBoxWidth, SubMenuTextY, m_colors[UI_TEXT_COL]);
					}
				}
			}

			SubMenuBoxY += HeadBoxHeight + 1;
			SubMenuTextY += HeadBoxHeight + 1;

			if (subm_entry[i].m_space)
			{
				SubMenuBoxY += HeadBoxHeight + 1;
				SubMenuTextY += HeadBoxHeight + 1;
			}
		}
	}

	x += HeadBoxWidth + 1;

	if (s_opened[UI_SUB_SUB_POS])
	{
		int SubSubMenuBoxX = x + 1;
		int SubSubMenuBoxY = HeadY + HeadBoxHeight + 5;
		int SubSubMenuTextX = SubSubMenuBoxX + 6;
		int SubSubMenuTextY = SubSubMenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		for (int i = 0; i < ss_entry_size; i++)
		{
			g_render.draw_filled_rect(SubSubMenuBoxX, SubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

			if (i == ss_entry_position)
			{
				if (!s_opened[UI_SUB_SUB_SUB_POS])
					g_render.draw_filled_rect(SubSubMenuBoxX, SubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);

				g_render.draw_filled_rect(SubSubMenuBoxX, SubSubMenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
			}

			g_render.draw_stringW(ssubm_entry[i].m_name, SubSubMenuTextX + 3, SubSubMenuTextY - 11, g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (ssubm_entry[i].m_state == UI_TAB_STATE)
			{
				g_render.draw_string("+", SubSubMenuTextX + HeadBoxWidth - 20, SubSubMenuTextY - 11, g_render.get_font(Tahoma12px),
					TEXT_OUTLINE, (i == ss_entry_position && s_opened[UI_SUB_SUB_SUB_POS]) ? m_colors[UI_MAIN_COL] : m_colors[UI_TEXT_COL]);
			}
			else
			{
				if (ssubm_entry[i].m_state != UI_FUNCTION_STATE)
				{
					if (ssubm_entry[i].m_state == UI_BOOL_STATE)
					{
						draw_bool(g_vars.get_as<bool>(ssubm_entry[i].m_var).value(),
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_PRIMARY_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_INT_STATE)
					{
						draw_int(g_vars.get_as<int>(ssubm_entry[i].m_var).value(),
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_FLOAT_STATE)
					{
						draw_float(g_vars.get_as<float>(ssubm_entry[i].m_var).value(),
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_TEXT_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_KEY_STATE)
					{
						draw_hotkey(g_vars.get_as<int>(ssubm_entry[i].m_var).value(),
							SubSubMenuBoxX + HeadBoxWidth, SubSubMenuTextX + HeadBoxWidth, SubSubMenuBoxY,
							SubSubMenuTextY, HeadBoxHeight, ssubm_entry[i].m_key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
					}
					else if (ssubm_entry[i].m_state == UI_ITEM_STATE)
					{
						draw_item(ssubm_entry[i].m_items[g_vars.get_as<int>(ssubm_entry[i].m_var).value()],
							SubSubMenuTextX + HeadBoxWidth, SubSubMenuTextY, m_colors[UI_TEXT_COL]);
					}
				}
			}

			SubSubMenuBoxY += HeadBoxHeight + 1;
			SubSubMenuTextY += HeadBoxHeight + 1;

			if (ssubm_entry[i].m_space)
			{
				SubSubMenuBoxY += HeadBoxHeight + 1;
				SubSubMenuTextY += HeadBoxHeight + 1;
			}
		}
	}

	x += HeadBoxWidth + 1;

	if (s_opened[UI_SUB_SUB_SUB_POS])
	{
		int SubSubSubMenuBoxX = x + 1;
		int SubSubSubMenuBoxY = HeadY + HeadBoxHeight + 5;
		int SubSubSubMenuTextX = SubSubSubMenuBoxX + 6;
		int SubSubSubMenuTextY = SubSubSubMenuBoxY + int(HeadBoxHeight / 2.0f) + 4;

		for (int i = 0; i < sss_entry_size; i++)
		{
			g_render.draw_filled_rect(SubSubSubMenuBoxX, SubSubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_SHADOW_COL]);

			if (i == sss_entry_position)
			{
				g_render.draw_filled_rect(SubSubSubMenuBoxX, SubSubSubMenuBoxY, HeadBoxWidth, HeadBoxHeight, m_colors[UI_PRIMARY_COL]);
				g_render.draw_filled_rect(SubSubSubMenuBoxX, SubSubSubMenuBoxY, 2, HeadBoxHeight, m_colors[UI_MAIN_COL]);
			}

			g_render.draw_stringW(sssubm_entry[i].m_name, SubSubSubMenuTextX + 3, SubSubSubMenuTextY - 11, g_render.get_font(Tahoma12px), TEXT_OUTLINE, m_colors[UI_TEXT_COL]);

			if (sssubm_entry[i].m_state != UI_FUNCTION_STATE)
			{
				if (sssubm_entry[i].m_state == UI_BOOL_STATE)
				{
					draw_bool(g_vars.get_as<bool>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_PRIMARY_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_INT_STATE)
				{
					draw_int(g_vars.get_as<int>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_TEXT_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_FLOAT_STATE)
				{
					draw_float(g_vars.get_as<float>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_TEXT_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_KEY_STATE)
				{
					draw_hotkey(g_vars.get_as<int>(sssubm_entry[i].m_var).value(),
						SubSubSubMenuBoxX + HeadBoxWidth, SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuBoxY,
						SubSubSubMenuTextY, HeadBoxHeight, sssubm_entry[i].m_key_hold, m_colors[UI_TEXT_COL], m_colors[UI_MAIN_COL]);
				}
				else if (sssubm_entry[i].m_state == UI_ITEM_STATE)
				{
					draw_item(sssubm_entry[i].m_items[g_vars.get_as<int>(sssubm_entry[i].m_var).value()],
						SubSubSubMenuTextX + HeadBoxWidth, SubSubSubMenuTextY, m_colors[UI_TEXT_COL]);
				}
			}

			SubSubSubMenuBoxY += HeadBoxHeight + 1;
			SubSubSubMenuTextY += HeadBoxHeight + 1;

			if (sssubm_entry[i].m_space)
			{
				SubSubSubMenuBoxY += HeadBoxHeight + 1;
				SubSubSubMenuTextY += HeadBoxHeight + 1;
			}
		}
	}
}