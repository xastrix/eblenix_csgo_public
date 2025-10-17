#include "ui.h"

#include "globals.h"
#include "config_manager.h"
#include "input_manager.h"

ui g_ui;

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
	KEYS_ON_HOLD_TAB = 1,

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
	KEYS_ON_HOLD_MISC_TAB = 0,
};

void ui::instance()
{
	m_colors[ UI_MAIN_COL    ] = color_t("ui->col");
	m_colors[ UI_PRIMARY_COL ] = color_t("ui->col", 130);
	m_colors[ UI_SHADOW_COL  ] = color_t(20, 20, 20, g_vars.get_as<int>("ui->col->a").value());
	m_colors[ UI_TEXT_COL    ] = color_t(253, 253, 253);

	if (!m_opened)
		return;

	setup();

	for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
		if (i == UI_SPRITE_NONE)
			continue;

		m_sprites[i].begin();
	}

	draw(g_vars.get_as<int>("ui->pos->x").value(), g_vars.get_as<int>("ui->pos->y").value());

	for (int i = UI_SPRITE_NONE; i < maxUISprites; i++) {
		if (i == UI_SPRITE_NONE)
			continue;

		m_sprites[i].end();
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
		add_bool(L"Enable", "aimbot->enabled", true);

		add_bool(L"Silent", "aimbot->silent");
		add_bool(L"Auto Pistol", "aimbot->autopistol", true);

		add_bool(L"Visible", "aimbot->visible_check");
		add_bool(L"Team", "aimbot->teammate_check");
		add_bool(L"Smoke", "aimbot->smoke_check");
		add_bool(L"Flash", "aimbot->flash_check");
		add_bool(L"Scope", "aimbot->scope_check");
		add_bool(L"Jump", "aimbot->jump_check", true);

		add_tab(L"Weapons");
		add_tab(L"Knifes");

		switch (s_entry_position) {
		case AIMBOT_WEAPONS_TAB: {
			add_sub_bool(L"All", "aimbot->all", true);

			std::vector<std::wstring> m_hitboxes{ L"Head", L"Neck", L"Chest", L"Stomach", L"Pelvis" };
			std::vector<std::wstring> m_types{ L"Hitbox", L"Nearest" };

			if (g_vars.get_as<bool>("aimbot->all").value()) {
				add_sub_item(L"Type", "aimbot->all->type", m_types, true);

				if (g_vars.get_as<int>("aimbot->all->type").value() == 0)
					add_sub_item(L"Hitbox", "aimbot->all->bone", m_hitboxes, true);

				add_sub_float(L"Fov", "aimbot->all->fov", 0.0f, 180.0f, 0.5f);
				add_sub_float(L"Smooth", "aimbot->all->smooth", 1.0f, 10.0f, 0.1f);
			}
			else {
				add_sub_tab(L"Pistols");
				add_sub_tab(L"Rifles");
				add_sub_tab(L"Snipers");
				add_sub_tab(L"Heavies");
				add_sub_tab(L"Smgs");

				switch (ss_entry_position) {
				case AIMBOT_WEAPONS_PISTOL_TAB: {
					add_sub_sub_item(L"Type", "aimbot->type->pistol", m_types, true);

					if (g_vars.get_as<int>("aimbot->type->pistol").value() == 0)
						add_sub_sub_item(L"Hitbox", "aimbot->bone->pistol", m_hitboxes, true);

					add_sub_sub_float(L"Fov", "aimbot->fov->pistol", 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", "aimbot->smooth->pistol", 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_RIFLE_TAB: {
					add_sub_sub_item(L"Type", "aimbot->type->rifle", m_types, true);

					if (g_vars.get_as<int>("aimbot->type->rifle").value() == 0)
						add_sub_sub_item(L"Hitbox", "aimbot->bone->rifle", m_hitboxes, true);

					add_sub_sub_float(L"Fov", "aimbot->fov->rifle", 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", "aimbot->smooth->rifle", 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_SNIPER_TAB: {
					add_sub_sub_item(L"Type", "aimbot->type->sniper", m_types, true);

					if (g_vars.get_as<int>("aimbot->type->sniper").value() == 0)
						add_sub_sub_item(L"Hitbox", "aimbot->bone->sniper", m_hitboxes, true);

					add_sub_sub_float(L"Fov", "aimbot->fov->sniper", 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", "aimbot->smooth->sniper", 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_HEAVY_TAB: {
					add_sub_sub_item(L"Type", "aimbot->type->heavy", m_types, true);

					if (g_vars.get_as<int>("aimbot->type->heavy").value() == 0)
						add_sub_sub_item(L"Hitbox", "aimbot->bone->heavy", m_hitboxes, true);

					add_sub_sub_float(L"Fov", "aimbot->fov->heavy", 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", "aimbot->smooth->heavy", 1.0f, 10.0f, 0.1f);
					break;
				}
				case AIMBOT_WEAPONS_SMG_TAB: {
					add_sub_sub_item(L"Type", "aimbot->type->smg", m_types, true);

					if (g_vars.get_as<int>("aimbot->type->smg").value() == 0)
						add_sub_sub_item(L"Hitbox", "aimbot->bone->smg", m_hitboxes, true);

					add_sub_sub_float(L"Fov", "aimbot->fov->smg", 0.0f, 180.0f, 0.5f);
					add_sub_sub_float(L"Smooth", "aimbot->smooth->smg", 1.0f, 10.0f, 0.1f);
					break;
				}
				}
			}
			break;
		}
		case AIMBOT_KNIFE_TAB: {
			add_sub_bool(L"Enable", "knifebot->enabled", true);

			add_sub_bool(L"Team", "knifebot->teammate_check");
			add_sub_bool(L"Smoke", "knifebot->smoke_check");
			add_sub_bool(L"Flash", "knifebot->flash_check", true);

			add_sub_float(L"Distance", "knifebot->distance", 40.0f, 80.0f, 0.5f);
			break;
		}
		}
		break;
	}
	case TRIGGERBOT_TAB: {
		add_bool(L"Enable", "triggerbot->enabled", true);

		add_bool(L"Team", "triggerbot->teammate_check");
		add_bool(L"Smoke", "triggerbot->smoke_check");
		add_bool(L"Flash", "triggerbot->flash_check");
		add_bool(L"Scope", "triggerbot->scope_check");
		add_bool(L"Jump", "triggerbot->jump_check", true);

		add_bool(L"Head", "triggerbot->hitbox->head");
		add_bool(L"Body", "triggerbot->hitbox->body");
		add_bool(L"Arms", "triggerbot->hitbox->arms");
		add_bool(L"Legs", "triggerbot->hitbox->legs");
		break;
	}
	case ESP_TAB: {
		add_bool(L"Enable", "esp->enabled", true);

		add_bool(L"Team", "esp->team");
		add_bool(L"Visible", "esp->visible_only");
		add_bool(L"Walking", "esp->walking_only", true);

		add_item(L"Render Type", "esp->render_type", { L"Static", L"Dynamic" }, true);

		add_tab(L"Name");
		add_tab(L"Box");
		add_tab(L"Health");
		add_tab(L"Weapon");
		add_tab(L"Skeleton");
		add_tab(L"Armor");
		add_tab(L"Barrel");
		add_tab(L"Crosshair");
		add_tab(L"Offscreen Lines", true);

		add_tab(L"Flags");

		switch (s_entry_position) {
		case ESP_NAME_TAB: {
			add_sub_bool(L"Enable", "esp->name->enabled", true);

			add_sub_item(L"Name Type", "esp->name->type", { L"Default", L"Lower", L"Upper" }, true);

			add_sub_int(L"Name R", "esp->name->col->r", 0, 255, 1);
			add_sub_int(L"Name G", "esp->name->col->g", 0, 255, 1);
			add_sub_int(L"Name B", "esp->name->col->b", 0, 255, 1);
			break;
		}
		case ESP_BOX_TAB: {
			add_sub_bool(L"Enable", "esp->box->enabled", true);

			add_sub_item(L"Box Type", "esp->box->type",
				{ L"Default", L"Default (Background)", L"Corner", L"Corner (Background)" }, true);

			add_sub_int(L"Box R", "esp->box->col->r", 0, 255, 1);
			add_sub_int(L"Box G", "esp->box->col->g", 0, 255, 1);
			add_sub_int(L"Box B", "esp->box->col->b", 0, 255, 1);
			break;
		}
		case ESP_HEALTH_TAB: {
			add_sub_bool(L"Enable", "esp->health->enabled", true);

			add_sub_item(L"Health Type", "esp->health->type", { L"Default", L"Health Based" }, true);

			switch (g_vars.get_as<int>("esp->health->type").value()) {
			case 0: {
				add_sub_int(L"Health R", "esp->health->col->r", 0, 255, 1);
				add_sub_int(L"Health G", "esp->health->col->g", 0, 255, 1);
				add_sub_int(L"Health B", "esp->health->col->b", 0, 255, 1, true);
				break;
			}
			}

			add_sub_bool(L"Health Battery", "esp->health->battery");
			break;
		}
		case ESP_WEAPON_TAB: {
			add_sub_bool(L"Enable", "esp->weapon->enabled", true);

			add_sub_item(L"Weapon Type", "esp->weapon->type", { L"Text", L"Icon" }, true);

			add_sub_int(L"Weapon R", "esp->weapon->col->r", 0, 255, 1);
			add_sub_int(L"Weapon G", "esp->weapon->col->g", 0, 255, 1);
			add_sub_int(L"Weapon B", "esp->weapon->col->b", 0, 255, 1);
			break;
		}
		case ESP_SKELETON_TAB: {
			add_sub_bool(L"Enable", "esp->skeleton->enabled", true);

			add_sub_item(L"Skeleton Type", "esp->skeleton->type", { L"Default", L"Health Based" }, true);

			add_sub_int(L"Skeleton R", "esp->skeleton->col->r", 0, 255, 1);
			add_sub_int(L"Skeleton G", "esp->skeleton->col->g", 0, 255, 1);
			add_sub_int(L"Skeleton B", "esp->skeleton->col->b", 0, 255, 1);
			break;
		}
		case ESP_ARMOR_TAB: {
			add_sub_bool(L"Enable", "esp->armor->enabled", true);

			add_sub_int(L"Armor R", "esp->armor->col->r", 0, 255, 1);
			add_sub_int(L"Armor G", "esp->armor->col->g", 0, 255, 1);
			add_sub_int(L"Armor B", "esp->armor->col->b", 0, 255, 1);
			break;
		}
		case ESP_BARREL_TAB: {
			add_sub_bool(L"Enable", "esp->barrel->enabled", true);

			add_sub_int(L"Barrel R", "esp->barrel->col->r", 0, 255, 1);
			add_sub_int(L"Barrel G", "esp->barrel->col->g", 0, 255, 1);
			add_sub_int(L"Barrel B", "esp->barrel->col->b", 0, 255, 1);
			break;
		}
		case ESP_CROSSHAIR_TAB: {
			add_sub_bool(L"Enable", "esp->crosshair->enabled", true);

			add_sub_float(L"Crosshair Fov", "esp->crosshair->fov", 1.0f, 2.0f, 0.1f, true);

			add_sub_int(L"Crosshair No Target R", "esp->crosshair->col->r", 0, 255, 1);
			add_sub_int(L"Crosshair No Target G", "esp->crosshair->col->g", 0, 255, 1);
			add_sub_int(L"Crosshair No Target B", "esp->crosshair->col->b", 0, 255, 1, true);

			add_sub_int(L"Crosshair On Target R", "esp->crosshair->col->on_target->r", 0, 255, 1);
			add_sub_int(L"Crosshair On Target G", "esp->crosshair->col->on_target->g", 0, 255, 1);
			add_sub_int(L"Crosshair On Target B", "esp->crosshair->col->on_target->b", 0, 255, 1);
			break;
		}
		case ESP_OFFSCREEN_LINES_TAB: {
			add_sub_bool(L"Enable", "esp->offscreen_lines->enabled", true);

			add_sub_int(L"Offscreen Lines R", "esp->offscreen_lines->col->r", 0, 255, 1);
			add_sub_int(L"Offscreen Lines G", "esp->offscreen_lines->col->g", 0, 255, 1);
			add_sub_int(L"Offscreen Lines B", "esp->offscreen_lines->col->b", 0, 255, 1);
			break;
		}
		case ESP_FLAGS_TAB: {
			add_sub_bool(L"Enable", "esp->flags->enabled", true);

			add_sub_bool(L"HK", "esp->flags->hk");
			add_sub_bool(L"Health", "esp->flags->health");
			add_sub_bool(L"Money", "esp->flags->money");
			add_sub_bool(L"Ammo", "esp->flags->ammo");
			add_sub_bool(L"Scoped", "esp->flags->scoped");
			add_sub_bool(L"Flashed", "esp->flags->flashed");
			add_sub_bool(L"Defusing", "esp->flags->defusing");
			add_sub_bool(L"Distance", "esp->flags->distance", true);

			add_sub_int(L"Flags R", "esp->flags->col->r", 0, 255, 1);
			add_sub_int(L"Flags G", "esp->flags->col->g", 0, 255, 1);
			add_sub_int(L"Flags B", "esp->flags->col->b", 0, 255, 1, true);

			add_sub_tab(L"Inventory Items");

			switch (ss_entry_position) {
			case ESP_FLAGS_INVENTORY_ITEMS_TAB: {
				add_sub_sub_bool(L"Enable", "esp->flags->items->enabled", true);

				add_sub_sub_int(L"Inventory Items R", "esp->flags->items->col->r", 0, 255, 1);
				add_sub_sub_int(L"Inventory Items G", "esp->flags->items->col->g", 0, 255, 1);
				add_sub_sub_int(L"Inventory Items B", "esp->flags->items->col->b", 0, 255, 1);
				break;
			}
			}
			break;
		}
		}
		break;
	}
	case VISUALS_TAB: {
		add_bool(L"Enable", "visuals->enabled", true);

		add_tab(L"Interface");
		add_tab(L"World");
		add_tab(L"Removals");

		switch (s_entry_position) {
		case VISUALS_INTERFACE_TAB: {
			add_sub_bool(L"Draw Status", "visuals->interface->status");
			add_sub_bool(L"Draw Spectators", "visuals->interface->spectators");
			break;
		}
		case VISUALS_WORLD_TAB: {
			add_sub_tab(L"Items");
			add_sub_tab(L"Grenades (In use)");
			add_sub_tab(L"Weapons");
			add_sub_tab(L"Bomb (Planted)");

			switch (ss_entry_position) {
			case VISUALS_WORLD_ITEMS_TAB: {
				add_sub_sub_bool(L"Enable", "visuals->world->items->enabled", true);

				add_sub_sub_bool(L"Items Distance", "visuals->world->items->distance", true);

				add_sub_sub_int(L"Items R", "visuals->world->items->col->r", 0, 255, 1);
				add_sub_sub_int(L"Items G", "visuals->world->items->col->g", 0, 255, 1);
				add_sub_sub_int(L"Items B", "visuals->world->items->col->b", 0, 255, 1);
				break;
			}
			case VISUALS_WORLD_GRENADES_TAB: {
				add_sub_sub_bool(L"Enable", "visuals->world->projectlines->enabled", true);

				add_sub_sub_item(L"Grenades Type", "visuals->world->projectlines->type", { L"Text", L"Icon" }, true);

				add_sub_sub_bool(L"Grenades Distance", "visuals->world->projectlines->distance", true);

				add_sub_sub_int(L"Grenades R", "visuals->world->projectlines->col->r", 0, 255, 1);
				add_sub_sub_int(L"Grenades G", "visuals->world->projectlines->col->g", 0, 255, 1);
				add_sub_sub_int(L"Grenades B", "visuals->world->projectlines->col->b", 0, 255, 1);
				break;
			}
			case VISUALS_WORLD_WEAPONS_TAB: {
				add_sub_sub_bool(L"Enable", "visuals->world->weapons->enabled", true);

				add_sub_sub_item(L"Weapons Type", "visuals->world->weapons->type", { L"Text", L"Icon" }, true);

				add_sub_sub_bool(L"Weapons Distance", "visuals->world->weapons->distance");
				add_sub_sub_bool(L"Weapons Ammo Bar", "visuals->world->weapons->ammo_bar", true);

				add_sub_sub_int(L"Weapons R", "visuals->world->weapons->col->r", 0, 255, 1);
				add_sub_sub_int(L"Weapons G", "visuals->world->weapons->col->g", 0, 255, 1);
				add_sub_sub_int(L"Weapons B", "visuals->world->weapons->col->b", 0, 255, 1, true);

				add_sub_sub_int(L"Weapons Ammo Bar R", "visuals->world->weapons->ammo_bar->col->r", 0, 255, 1);
				add_sub_sub_int(L"Weapons Ammo Bar G", "visuals->world->weapons->ammo_bar->col->g", 0, 255, 1);
				add_sub_sub_int(L"Weapons Ammo Bar B", "visuals->world->weapons->ammo_bar->col->b", 0, 255, 1);
				break;
			}
			case VISUALS_WORLD_BOMB_TAB: {
				add_sub_sub_bool(L"Enable", "visuals->world->c4->enabled", true);

				add_sub_sub_item(L"Bomb Type", "visuals->world->c4->type", { L"Text", L"Icon" }, true);

				add_sub_sub_bool(L"Bomb Time Bar", "visuals->world->c4->time_bar");
				add_sub_sub_bool(L"Bomb Defuse Bar", "visuals->world->c4->defuse_bar");
				add_sub_sub_bool(L"Bomb Damage Indicator", "visuals->world->c4->damage_indicator", true);

				add_sub_sub_int(L"Bomb R", "visuals->world->c4->col->r", 0, 255, 1);
				add_sub_sub_int(L"Bomb G", "visuals->world->c4->col->g", 0, 255, 1);
				add_sub_sub_int(L"Bomb B", "visuals->world->c4->col->b", 0, 255, 1, true);

				add_sub_sub_int(L"Bomb Time Bar R", "visuals->world->c4->time_bar->col->r", 0, 255, 1);
				add_sub_sub_int(L"Bomb Time Bar G", "visuals->world->c4->time_bar->col->g", 0, 255, 1);
				add_sub_sub_int(L"Bomb Time Bar B", "visuals->world->c4->time_bar->col->b", 0, 255, 1, true);

				add_sub_sub_int(L"Bomb Defuse Bar R", "visuals->world->c4->defuse_bar->col->r", 0, 255, 1);
				add_sub_sub_int(L"Bomb Defuse Bar G", "visuals->world->c4->defuse_bar->col->g", 0, 255, 1);
				add_sub_sub_int(L"Bomb Defuse Bar B", "visuals->world->c4->defuse_bar->col->b", 0, 255, 1);
				break;
			}
			}
			break;
		}
		case VISUALS_REMOVALS_TAB: {
			add_sub_bool(L"Smoke", "visuals->removals->smoke");
			add_sub_bool(L"Flash", "visuals->removals->flash");
			add_sub_bool(L"Hands", "visuals->removals->hands");
			add_sub_bool(L"Sleeves", "visuals->removals->sleeves");
			add_sub_bool(L"Scope", "visuals->removals->scope");
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
			add_sub_bool(L"Radar", "misc->visual->engine_radar");
			add_sub_bool(L"Inventory Unlock", "misc->visual->inventory_unlock");
			add_sub_bool(L"Reveal Ranks", "misc->visual->reveal_ranks", true);

			add_sub_float(L"Viewmodel Fov", "misc->visual->viewmodel->fov", 54.0f, 130.0f, 1.0f, true);

			add_sub_bool(L"Aspect Ratio", "misc->visual->aspect_ratio");
			add_sub_float(L"Aspect Ratio Value", "misc->visual->aspect_ratio->value", 1.0f, 3.0f, 0.01f, true);

			add_sub_bool(L"Thirdperson", "misc->visual->thirdperson");
			add_sub_float(L"Thirdperson Fov", "misc->visual->thirdperson->fov", 20.0f, 90.0f, 1.0f);
			break;
		}
		case ESSENTIALS_MOVEMENT_TAB: {
			add_sub_bool(L"Bunnyhop", "misc->movement->bunnyhop");
			add_sub_bool(L"Infinite Duck", "misc->movement->infinite_duck");
			add_sub_bool(L"Autostrafe", "misc->movement->auto_strafe");
			break;
		}
		case ESSENTIALS_EVENT_LOGS_TAB: {
			add_sub_bool(L"Player Hurt", "misc->event_logs->player_hurt");
			add_sub_bool(L"Player Purchase", "misc->event_logs->player_purchase");
			break;
		}
		case ESSENTIALS_MENU_TAB: {
			add_sub_int(L"Position X", "ui->pos->x", 0, 500, 1);
			add_sub_int(L"Position Y", "ui->pos->y", 0, 700, 1, true);

			add_sub_int(L"Menu R", "ui->col->r", 0, 255, 1);
			add_sub_int(L"Menu G", "ui->col->g", 0, 255, 1);
			add_sub_int(L"Menu B", "ui->col->b", 0, 255, 1, true);

			add_sub_int(L"Menu A", "ui->col->a", 70, 170, 1);
			break;
		}
		}

		add_function(L"Unload", []() {
			g::unload();
		});
		break;
	}
	case GLOW_TAB: {
		add_bool(L"Enable", "glow->enabled", true);

		add_bool(L"Team", "glow->team");
		add_bool(L"Visible", "glow->visible_only");
		add_bool(L"Walking", "glow->walking_only", true);

		add_bool(L"C4", "glow->c4", true);

		add_tab(L"Color");

		switch (s_entry_position) {
		case GLOW_COLOR_TAB: {
			add_sub_tab(L"Enemy");
			add_sub_tab(L"Team");
			add_sub_tab(L"C4", true);

			switch (ss_entry_position) {
			case GLOW_COLOR_ENEMY_TAB: {
				add_sub_sub_int(L"Enemy R", "glow->enemy->col->r", 0, 255, 1);
				add_sub_sub_int(L"Enemy G", "glow->enemy->col->g", 0, 255, 1);
				add_sub_sub_int(L"Enemy B", "glow->enemy->col->b", 0, 255, 1, true);

				add_sub_sub_int(L"Enemy A", "glow->enemy->col->a", 130, 255, 1);
				break;
			}
			case GLOW_COLOR_TEAM_TAB: {
				add_sub_sub_int(L"Team R", "glow->team->col->r", 0, 255, 1);
				add_sub_sub_int(L"Team G", "glow->team->col->g", 0, 255, 1);
				add_sub_sub_int(L"Team B", "glow->team->col->b", 0, 255, 1, true);

				add_sub_sub_int(L"Team A", "glow->team->col->a", 130, 255, 1);
				break;
			}
			case GLOW_COLOR_C4_TAB: {
				add_sub_sub_int(L"C4 R", "glow->c4->col->r", 0, 255, 1);
				add_sub_sub_int(L"C4 G", "glow->c4->col->g", 0, 255, 1);
				add_sub_sub_int(L"C4 B", "glow->c4->col->b", 0, 255, 1, true);

				add_sub_sub_int(L"C4 A", "glow->c4->col->a", 130, 255, 1);
				break;
			}
			}

			add_sub_bool(L"Health Based", "glow->health_based");
			break;
		}
		}

		break;
	}
	case CHAMS_TAB: {
		add_bool(L"Enable", "chams->enabled", true);

		add_bool(L"Team", "chams->team");
		add_bool(L"Visible", "chams->visible_only");
		add_bool(L"Walking", "chams->walking_only", true);

		add_item(L"Type", "chams->type", { L"1", L"2", L"3" }, true);

		add_int(L"Chams R", "chams->col->r", 0, 255, 1);
		add_int(L"Chams G", "chams->col->g", 0, 255, 1);
		add_int(L"Chams B", "chams->col->b", 0, 255, 1, true);

		add_bool(L"Health Based", "chams->health_based");
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
				add_sub_sub_key(L"Aimbot", "keys->on_toggle->aimbot");
				add_sub_sub_key(L"Trigger", "keys->on_toggle->triggerbot");
				break;
			}
			case KEYS_ON_TOGGLE_MISC_TAB: {
				add_sub_sub_key(L"Thirdperson", "keys->on_toggle->thirdperson");
				break;
			}
			case KEYS_ON_TOGGLE_OTHER_TAB: {
				add_sub_sub_key(L"Menu", "keys->on_toggle->ui");
				add_sub_sub_key(L"Panic", "keys->on_toggle->panic");
				break;
			}
			}
			break;
		}
		case KEYS_ON_HOLD_TAB: {
			add_sub_tab(L"Misc");

			switch (ss_entry_position) {
			case KEYS_ON_HOLD_MISC_TAB: {
				add_sub_sub_key(L"Blockbot", "keys->on_hold->blockbot");
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
	if (k == g_vars.get_as<int>("keys->on_toggle->ui").value())
		m_opened = !m_opened;

	if (k == g_vars.get_as<int>("keys->on_toggle->aimbot").value())
		g_vars.set("aimbot->enabled", !g_vars.get_as<bool>("aimbot->enabled").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->triggerbot").value())
		g_vars.set("triggerbot->enabled", !g_vars.get_as<bool>("triggerbot->enabled").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->thirdperson").value())
		g_vars.set("misc->visual->thirdperson", !g_vars.get_as<bool>("misc->visual->thirdperson").value());

	if (k == g_vars.get_as<int>("keys->on_toggle->panic").value())
		g::panic_mode = !g::panic_mode;
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
					g_vars.set(subm_entry[s_entry_position].m_var, (int)k);
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
						g_vars.set(ssubm_entry[ss_entry_position].m_var, (int)k);
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
						g_vars.set(sssubm_entry[sss_entry_position].m_var, (int)k);
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

	auto draw_hotkey = [&](int k, int x, int x_text, int y, int y_text, int h, bool hold, color_t color) {
		const auto kss = g_input.virtual_key_to_wstring(k);

		const auto font = g_render.get_font(Tahoma12px);
		const auto key_string_width = g_render.get_text_widthW(hold ? L"---" : kss, font);

		g_render.draw_filled_rect(x - key_string_width - 47, y, 47 + key_string_width, h, m_colors[UI_MAIN_COL]);

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

	m_sprites[UI_SPRITE_LOGO].draw(MenuLogoX, MenuLogoY);

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
							SubMenuTextY, HeadBoxHeight, subm_entry[i].m_key_hold, m_colors[UI_TEXT_COL]);
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
							SubSubMenuTextY, HeadBoxHeight, ssubm_entry[i].m_key_hold, m_colors[UI_TEXT_COL]);
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
						SubSubSubMenuTextY, HeadBoxHeight, sssubm_entry[i].m_key_hold, m_colors[UI_TEXT_COL]);
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