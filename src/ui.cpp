#include "ui.h"

#include "globals.h"
#include "cfg.h"
#include "interfaces.h"

#include "sprites.hpp"

void ui::init(IDirect3DDevice9* device)
{
	m_sprites[UI_SPRITE_LOGO].init(device, m_ui_logotype, sizeof(m_ui_logotype), 178, 33);
}

void ui::run()
{
	float old_progress{};
	calc_animation_progress(g_vars.get_as<float>(V_UI_SPEED_ANIMATION).value(), g_csgo.m_globals->frame_time, old_progress);

	m_colors[UI_TEXT_COL] = color_t(253, 253, 253,
		static_cast<int>(target_animation_progress * 255.0f));

	m_colors[UI_MAIN_COL] = color_t(V_UI_COL,
		static_cast<int>(target_animation_progress * 255.0f));

	m_colors[UI_PRIMARY_COL] = color_t(V_UI_COL,
		static_cast<int>(target_animation_progress * 130.0f));

	m_colors[UI_SHADOW_COL] = color_t(20, 20, 20,
		static_cast<int>(target_animation_progress * g_vars.get_as<int>(V_UI_COL_A).value()));

	m_colors[UI_OUTLINE_COL] = color_t(40, 40, 40,
		static_cast<int>(target_animation_progress * 255.0f));

	if (target_animation_progress > 0.0f)
	{
		clear();
		setup();

		for (int i = 0; i < maxUISprites; i++)
			m_sprites[i].begin(D3DXSPRITE_DONOTMODIFY_RENDERSTATE);

		draw(g_vars.get_as<int>(V_UI_POS_X).value(), g_vars.get_as<int>(V_UI_POS_Y).value());

		for (int i = 0; i < maxUISprites; i++)
			m_sprites[i].end();
	}

	current_animation_progress = old_progress;
}

void ui::calc_animation_progress(float anim_time, float delta_time, float& old_alpha)
{
	if (m_opened)
	{
		target_animation_progress += anim_time * delta_time;

		if (target_animation_progress > 1.0f)
			target_animation_progress = 1.0f;
	}
	else
	{
		target_animation_progress -= anim_time * delta_time;

		if (target_animation_progress < 0.0f)
			target_animation_progress = 0.0f;
	}

	old_alpha = current_animation_progress;
	current_animation_progress = target_animation_progress;
}

void ui::setup()
{
	const auto cfgs = std::vector<std::wstring>{ L"1", L"2", L"3", L"4", L"5" };

	add(L"Aim", [&]() {
		add_bool<UI_SUB_POS>(L"Enable", V_AIMBOT_ENABLED, true);

		add_bool<UI_SUB_POS>(L"Silent", V_AIMBOT_SILENT);
		add_bool<UI_SUB_POS>(L"Auto Pistol", V_AIMBOT_AUTOPISTOL, true);

		add_bool<UI_SUB_POS>(L"Visible", V_AIMBOT_VISIBLE_CHECK);
		add_bool<UI_SUB_POS>(L"Team", V_AIMBOT_TEAMMATE_CHECK);
		add_bool<UI_SUB_POS>(L"Smoke", V_AIMBOT_SMOKE_CHECK);
		add_bool<UI_SUB_POS>(L"Flash", V_AIMBOT_FLASH_CHECK);
		add_bool<UI_SUB_POS>(L"Scope", V_AIMBOT_SCOPE_CHECK);
		add_bool<UI_SUB_POS>(L"Jump", V_AIMBOT_JUMP_CHECK, true);

		add_tab<UI_SUB_POS>(L"Weapons", [&]() {
			std::vector<std::wstring> m_hitboxes{ L"Head", L"Neck", L"Chest", L"Stomach", L"Pelvis" };
			std::vector<std::wstring> m_types{ L"Hitbox", L"Nearest" };

			add_tab<UI_SUB_SUB_POS>(L"Pistols", [&]() {
				add_item<UI_SUB_SUB_SUB_POS>(L"Type", V_AIMBOT_TYPE_PISTOL, m_types, true);

				if (g_vars.get_as<int>(V_AIMBOT_TYPE_PISTOL).value() == 0)
					add_item<UI_SUB_SUB_SUB_POS>(L"Hitbox", V_AIMBOT_BONE_PISTOL, m_hitboxes, true);

				add_float<UI_SUB_SUB_SUB_POS>(L"Fov", V_AIMBOT_FOV_PISTOL, 0.0f, 180.0f, 0.5f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Smooth", V_AIMBOT_SMOOTH_PISTOL, 1.0f, 10.0f, 0.1f);
			});

			add_tab<UI_SUB_SUB_POS>(L"Rifles", [&]() {
				add_item<UI_SUB_SUB_SUB_POS>(L"Type", V_AIMBOT_TYPE_RIFLE, m_types, true);

				if (g_vars.get_as<int>(V_AIMBOT_TYPE_RIFLE).value() == 0)
					add_item<UI_SUB_SUB_SUB_POS>(L"Hitbox", V_AIMBOT_BONE_RIFLE, m_hitboxes, true);

				add_float<UI_SUB_SUB_SUB_POS>(L"Fov", V_AIMBOT_FOV_RIFLE, 0.0f, 180.0f, 0.5f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Smooth", V_AIMBOT_SMOOTH_RIFLE, 1.0f, 10.0f, 0.1f);
			});

			add_tab<UI_SUB_SUB_POS>(L"Snipers", [&]() {
				add_item<UI_SUB_SUB_SUB_POS>(L"Type", V_AIMBOT_TYPE_SNIPER, m_types, true);

				if (g_vars.get_as<int>(V_AIMBOT_TYPE_SNIPER).value() == 0)
					add_item<UI_SUB_SUB_SUB_POS>(L"Hitbox", V_AIMBOT_BONE_SNIPER, m_hitboxes, true);

				add_float<UI_SUB_SUB_SUB_POS>(L"Fov", V_AIMBOT_FOV_SNIPER, 0.0f, 180.0f, 0.5f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Smooth", V_AIMBOT_SMOOTH_SNIPER, 1.0f, 10.0f, 0.1f);
			});

			add_tab<UI_SUB_SUB_POS>(L"Heavies", [&]() {
				add_item<UI_SUB_SUB_SUB_POS>(L"Type", V_AIMBOT_TYPE_HEAVY, m_types, true);

				if (g_vars.get_as<int>(V_AIMBOT_TYPE_HEAVY).value() == 0)
					add_item<UI_SUB_SUB_SUB_POS>(L"Hitbox", V_AIMBOT_BONE_HEAVY, m_hitboxes, true);

				add_float<UI_SUB_SUB_SUB_POS>(L"Fov", V_AIMBOT_FOV_HEAVY, 0.0f, 180.0f, 0.5f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Smooth", V_AIMBOT_SMOOTH_HEAVY, 1.0f, 10.0f, 0.1f);
			});

			add_tab<UI_SUB_SUB_POS>(L"Smgs", [&]() {
				add_item<UI_SUB_SUB_SUB_POS>(L"Type", V_AIMBOT_TYPE_SMG, m_types, true);

				if (g_vars.get_as<int>(V_AIMBOT_TYPE_SMG).value() == 0)
					add_item<UI_SUB_SUB_SUB_POS>(L"Hitbox", V_AIMBOT_BONE_SMG, m_hitboxes, true);

				add_float<UI_SUB_SUB_SUB_POS>(L"Fov", V_AIMBOT_FOV_SMG, 0.0f, 180.0f, 0.5f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Smooth", V_AIMBOT_SMOOTH_SMG, 1.0f, 10.0f, 0.1f);
			});
		});

		add_tab<UI_SUB_POS>(L"Knifes", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_KNIFEBOT_ENABLED, true);

			add_bool<UI_SUB_SUB_POS>(L"Team", V_KNIFEBOT_TEAMMATE_CHECK);
			add_bool<UI_SUB_SUB_POS>(L"Smoke", V_KNIFEBOT_SMOKE_CHECK);
			add_bool<UI_SUB_SUB_POS>(L"Flash", V_KNIFEBOT_FLASH_CHECK, true);

			add_float<UI_SUB_SUB_POS>(L"Distance", V_KNIFEBOT_DISTANCE, 40.0f, 80.0f, 0.5f);
		});
	});

	add(L"Trigger", [&]() {
		add_bool<UI_SUB_POS>(L"Enable", V_TRIGGERBOT_ENABLED, true);

		add_bool<UI_SUB_POS>(L"Team", V_TRIGGERBOT_TEAMMATE_CHECK);
		add_bool<UI_SUB_POS>(L"Smoke", V_TRIGGERBOT_SMOKE_CHECK);
		add_bool<UI_SUB_POS>(L"Flash", V_TRIGGERBOT_FLASH_CHECK);
		add_bool<UI_SUB_POS>(L"Scope", V_TRIGGERBOT_SCOPE_CHECK);
		add_bool<UI_SUB_POS>(L"Jump", V_TRIGGERBOT_JUMP_CHECK, true);

		add_bool<UI_SUB_POS>(L"Head", V_TRIGGERBOT_HITBOX_HEAD);
		add_bool<UI_SUB_POS>(L"Body", V_TRIGGERBOT_HITBOX_BODY);
		add_bool<UI_SUB_POS>(L"Arms", V_TRIGGERBOT_HITBOX_ARMS);
		add_bool<UI_SUB_POS>(L"Legs", V_TRIGGERBOT_HITBOX_LEGS);
	});

	add(L"Esp", [&]() {
		add_bool<UI_SUB_POS>(L"Enable", V_ESP_ENABLED, true);

		add_bool<UI_SUB_POS>(L"Team", V_ESP_TEAM);
		add_bool<UI_SUB_POS>(L"Visible", V_ESP_VISIBLE_ONLY);
		add_bool<UI_SUB_POS>(L"Walking", V_ESP_WALKING_ONLY, true);

		add_item<UI_SUB_POS>(L"Render Type", V_ESP_RENDER_TYPE, { L"Static", L"Dynamic" }, true);

		add_tab<UI_SUB_POS>(L"Name", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_NAME_ENABLED, true);

			add_item<UI_SUB_SUB_POS>(L"Name Type", V_ESP_NAME_TYPE, { L"Default", L"Lower", L"Upper" }, true);

			add_int<UI_SUB_SUB_POS>(L"Name R", V_ESP_NAME_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Name G", V_ESP_NAME_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Name B", V_ESP_NAME_COL_B, 0, 255, 1);
		});

		add_tab<UI_SUB_POS>(L"Box", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_BOX_ENABLED, true);

			add_item<UI_SUB_SUB_POS>(L"Box Type", V_ESP_BOX_TYPE,
				{ L"Default", L"Default (Background)", L"Corner", L"Corner (Background)" }, true);

			add_int<UI_SUB_SUB_POS>(L"Box R", V_ESP_BOX_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Box G", V_ESP_BOX_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Box B", V_ESP_BOX_COL_B, 0, 255, 1);
		});

		add_tab<UI_SUB_POS>(L"Health", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_HEALTH_ENABLED, true);

			add_item<UI_SUB_SUB_POS>(L"Health Type", V_ESP_HEALTH_TYPE, { L"Default", L"Health Based" }, true);

			if (g_vars.get_as<int>(V_ESP_HEALTH_TYPE).value() == 0)
			{
				add_int<UI_SUB_SUB_POS>(L"Health R", V_ESP_HEALTH_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_POS>(L"Health G", V_ESP_HEALTH_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_POS>(L"Health B", V_ESP_HEALTH_COL_B, 0, 255, 1, true);
			}

			add_bool<UI_SUB_SUB_POS>(L"Health Battery", V_ESP_HEALTH_BATTERY);
		});

		add_tab<UI_SUB_POS>(L"Weapon", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_WEAPON_ENABLED, true);

			add_item<UI_SUB_SUB_POS>(L"Weapon Type", V_ESP_WEAPON_TYPE, { L"Text", L"Icon" }, true);

			add_int<UI_SUB_SUB_POS>(L"Weapon R", V_ESP_WEAPON_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Weapon G", V_ESP_WEAPON_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Weapon B", V_ESP_WEAPON_COL_B, 0, 255, 1);
		});

		add_tab<UI_SUB_POS>(L"Skeleton", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_SKELETON_ENABLED, true);

			add_item<UI_SUB_SUB_POS>(L"Skeleton Type", V_ESP_SKELETON_TYPE, { L"Default", L"Health Based" }, true);

			add_int<UI_SUB_SUB_POS>(L"Skeleton R", V_ESP_SKELETON_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Skeleton G", V_ESP_SKELETON_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Skeleton B", V_ESP_SKELETON_COL_B, 0, 255, 1);
		});

		add_tab<UI_SUB_POS>(L"Armor", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_ARMOR_ENABLED, true);

			add_int<UI_SUB_SUB_POS>(L"Armor R", V_ESP_ARMOR_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Armor G", V_ESP_ARMOR_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Armor B", V_ESP_ARMOR_COL_B, 0, 255, 1, true);

			add_bool<UI_SUB_SUB_POS>(L"Armor Battery", V_ESP_ARMOR_BATTERY);
		});

		add_tab<UI_SUB_POS>(L"Barrel", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_BARREL_ENABLED, true);

			add_int<UI_SUB_SUB_POS>(L"Barrel R", V_ESP_BARREL_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Barrel G", V_ESP_BARREL_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Barrel B", V_ESP_BARREL_COL_B, 0, 255, 1);
		});

		add_tab<UI_SUB_POS>(L"Crosshair", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_CROSSHAIR_ENABLED, true);

			add_int<UI_SUB_SUB_POS>(L"Crosshair R", V_ESP_CROSSHAIR_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Crosshair G", V_ESP_CROSSHAIR_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Crosshair B", V_ESP_CROSSHAIR_COL_B, 0, 255, 1);
		});

		add_tab<UI_SUB_POS>(L"Snap Lines", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_SNAP_LINES_ENABLED, true);

			add_int<UI_SUB_SUB_POS>(L"Snap Lines R", V_ESP_SNAP_LINES_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Snap Lines G", V_ESP_SNAP_LINES_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Snap Lines B", V_ESP_SNAP_LINES_COL_B, 0, 255, 1);
		}, true);

		add_tab<UI_SUB_POS>(L"Flags", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Enable", V_ESP_FLAGS_ENABLED, true);

			add_bool<UI_SUB_SUB_POS>(L"HK", V_ESP_FLAGS_HK);
			add_bool<UI_SUB_SUB_POS>(L"Health", V_ESP_FLAGS_HEALTH);
			add_bool<UI_SUB_SUB_POS>(L"Money", V_ESP_FLAGS_MONEY);
			add_bool<UI_SUB_SUB_POS>(L"Ammo", V_ESP_FLAGS_AMMO);
			add_bool<UI_SUB_SUB_POS>(L"Scoped", V_ESP_FLAGS_SCOPED);
			add_bool<UI_SUB_SUB_POS>(L"Flashed", V_ESP_FLAGS_FLASHED);
			add_bool<UI_SUB_SUB_POS>(L"Defusing", V_ESP_FLAGS_DEFUSING);
			add_bool<UI_SUB_SUB_POS>(L"Distance", V_ESP_FLAGS_DISTANCE, true);

			add_int<UI_SUB_SUB_POS>(L"Flags R", V_ESP_FLAGS_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Flags G", V_ESP_FLAGS_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Flags B", V_ESP_FLAGS_COL_B, 0, 255, 1, true);

			add_tab<UI_SUB_SUB_POS>(L"Inventory Items", [&]() {
				add_bool<UI_SUB_SUB_SUB_POS>(L"Enable", V_ESP_FLAGS_ITEMS_ENABLED, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Inventory Items R", V_ESP_FLAGS_ITEMS_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Inventory Items G", V_ESP_FLAGS_ITEMS_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Inventory Items B", V_ESP_FLAGS_ITEMS_COL_B, 0, 255, 1);
			});
		});
	});

	add(L"Visuals", [&]() {
		add_bool<UI_SUB_POS>(L"Enable", V_VISUALS_ENABLED);
		add_bool<UI_SUB_POS>(L"Engine Radar", V_VISUALS_ENGINE_RADAR, true);

		add_tab<UI_SUB_POS>(L"Interface", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Draw Status", V_VISUALS_INTERFACE_STATUS);
			add_bool<UI_SUB_SUB_POS>(L"Draw Spectators", V_VISUALS_INTERFACE_SPECTATORS);
		});

		add_tab<UI_SUB_POS>(L"World", [&]() {
			add_tab<UI_SUB_SUB_POS>(L"Items", [&]() {
				add_bool<UI_SUB_SUB_SUB_POS>(L"Enable", V_VISUALS_WORLD_ITEMS_ENABLED, true);

				add_bool<UI_SUB_SUB_SUB_POS>(L"Items Distance", V_VISUALS_WORLD_ITEMS_DISTANCE, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Items R", V_VISUALS_WORLD_ITEMS_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Items G", V_VISUALS_WORLD_ITEMS_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Items B", V_VISUALS_WORLD_ITEMS_COL_B, 0, 255, 1);
			});

			add_tab<UI_SUB_SUB_POS>(L"Grenades (In use)", [&]() {
				add_bool<UI_SUB_SUB_SUB_POS>(L"Enable", V_VISUALS_WORLD_PROJECTLINES_ENABLED, true);

				add_item<UI_SUB_SUB_SUB_POS>(L"Grenades Type", V_VISUALS_WORLD_PROJECTLINES_TYPE, { L"Text", L"Icon" }, true);

				add_bool<UI_SUB_SUB_SUB_POS>(L"Grenades Distance", V_VISUALS_WORLD_PROJECTLINES_DISTANCE, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Grenades R", V_VISUALS_WORLD_PROJECTLINES_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Grenades G", V_VISUALS_WORLD_PROJECTLINES_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Grenades B", V_VISUALS_WORLD_PROJECTLINES_COL_B, 0, 255, 1);
			});

			add_tab<UI_SUB_SUB_POS>(L"Weapons", [&]() {
				add_bool<UI_SUB_SUB_SUB_POS>(L"Enable", V_VISUALS_WORLD_WEAPONS_ENABLED, true);

				add_item<UI_SUB_SUB_SUB_POS>(L"Weapons Type", V_VISUALS_WORLD_WEAPONS_TYPE, { L"Text", L"Icon" }, true);

				add_bool<UI_SUB_SUB_SUB_POS>(L"Weapons Distance", V_VISUALS_WORLD_WEAPONS_DISTANCE);
				add_bool<UI_SUB_SUB_SUB_POS>(L"Weapons Ammo Bar", V_VISUALS_WORLD_WEAPONS_AMMO_BAR, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Weapons R", V_VISUALS_WORLD_WEAPONS_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Weapons G", V_VISUALS_WORLD_WEAPONS_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Weapons B", V_VISUALS_WORLD_WEAPONS_COL_B, 0, 255, 1, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Weapons Ammo Bar R", V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Weapons Ammo Bar G", V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Weapons Ammo Bar B", V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_B, 0, 255, 1);
			});

			add_tab<UI_SUB_SUB_POS>(L"Bomb (Planted)", [&]() {
				add_bool<UI_SUB_SUB_SUB_POS>(L"Enable", V_VISUALS_WORLD_C4_ENABLED, true);

				add_item<UI_SUB_SUB_SUB_POS>(L"Bomb Type", V_VISUALS_WORLD_C4_TYPE, { L"Text", L"Icon" }, true);

				add_bool<UI_SUB_SUB_SUB_POS>(L"Bomb Time Bar", V_VISUALS_WORLD_C4_TIME_BAR);
				add_bool<UI_SUB_SUB_SUB_POS>(L"Bomb Defuse Bar", V_VISUALS_WORLD_C4_DEFUSE_BAR);
				add_bool<UI_SUB_SUB_SUB_POS>(L"Bomb Damage Indicator", V_VISUALS_WORLD_C4_DAMAGE_INDICATOR, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb R", V_VISUALS_WORLD_C4_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb G", V_VISUALS_WORLD_C4_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb B", V_VISUALS_WORLD_C4_COL_B, 0, 255, 1, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb Time Bar R", V_VISUALS_WORLD_C4_COL_TIME_BAR_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb Time Bar G", V_VISUALS_WORLD_C4_COL_TIME_BAR_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb Time Bar B", V_VISUALS_WORLD_C4_COL_TIME_BAR_B, 0, 255, 1, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb Defuse Bar R", V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb Defuse Bar G", V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Bomb Defuse Bar B", V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_B, 0, 255, 1);
			});

			add_tab<UI_SUB_SUB_POS>(L"Nightmode", [&]() {
				add_bool<UI_SUB_SUB_SUB_POS>(L"Enable", V_VISUALS_WORLD_NIGHTMODE_ENABLED, true);

				add_float<UI_SUB_SUB_SUB_POS>(L"World Brightness", V_VISUALS_WORLD_NIGHTMODE_WORLD_BRIGHTNESS, 0.1f, 1.0f, 0.01f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Sky Brightness", V_VISUALS_WORLD_NIGHTMODE_SKY_BRIGHTNESS, 0.1f, 1.0f, 0.01f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Models Brightness", V_VISUALS_WORLD_NIGHTMODE_MODEL_BRIGHTNESS, 0.1f, 1.0f, 0.01f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Decals Brightness", V_VISUALS_WORLD_NIGHTMODE_DECAL_BRIGHTNESS, 0.1f, 1.0f, 0.01f);
				add_float<UI_SUB_SUB_SUB_POS>(L"Others Brightness", V_VISUALS_WORLD_NIGHTMODE_OTHER_BRIGHTNESS, 0.1f, 1.0f, 0.01f, true);

				add_function<UI_SUB_SUB_SUB_POS>(L"Apply Changes", []() {
					if (g_vars.get_as<bool>(V_VISUALS_WORLD_NIGHTMODE_ENABLED).value())
						GLOBAL(b_flags[BF_NIGHTMODE_HOLD]) = false;
				});
			});
		});

		add_tab<UI_SUB_POS>(L"Removals", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Smoke", V_VISUALS_REMOVALS_SMOKE);
			add_bool<UI_SUB_SUB_POS>(L"Flash", V_VISUALS_REMOVALS_FLASH);
			add_bool<UI_SUB_SUB_POS>(L"Hands", V_VISUALS_REMOVALS_HANDS);
			add_bool<UI_SUB_SUB_POS>(L"Sleeves", V_VISUALS_REMOVALS_SLEEVES);
			add_bool<UI_SUB_SUB_POS>(L"Scope", V_VISUALS_REMOVALS_SCOPE);
		});
	});

	add(L"Essentials", [&]() {
		add_tab<UI_SUB_POS>(L"Visual", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Inventory Unlock", V_MISC_VISUAL_INVENTORY_UNLOCK);
			add_bool<UI_SUB_SUB_POS>(L"Reveal Ranks", V_MISC_VISUAL_REVEAL_RANKS, true);

			add_float<UI_SUB_SUB_POS>(L"Viewmodel Fov", V_MISC_VISUAL_VIEWMODEL_FOV, 54.0f, 130.0f, 1.0f, true);

			add_bool<UI_SUB_SUB_POS>(L"Aspect Ratio", V_MISC_VISUAL_ASPECT_RATIO);
			add_float<UI_SUB_SUB_POS>(L"Aspect Ratio Value", V_MISC_VISUAL_ASPECT_RATIO_VALUE, 10.0f, 30.0f, 0.5f, true);

			add_bool<UI_SUB_SUB_POS>(L"Thirdperson", V_MISC_VISUAL_THIRDPERSON);
			add_float<UI_SUB_SUB_POS>(L"Thirdperson Fov", V_MISC_VISUAL_THIRDPERSON_FOV, 20.0f, 90.0f, 1.0f);
		});

		add_tab<UI_SUB_POS>(L"Movement", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Bunnyhop", V_MISC_MOVEMENT_BUNNYHOP);
			add_bool<UI_SUB_SUB_POS>(L"Infinite Duck", V_MISC_MOVEMENT_INFINITE_DUCK);
			add_bool<UI_SUB_SUB_POS>(L"Autostrafe", V_MISC_MOVEMENT_AUTO_STRAFE);
		});

		add_tab<UI_SUB_POS>(L"Event Logs", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Player Hurt", V_MISC_EVENT_LOGS_PLAYER_HURT);
			add_bool<UI_SUB_SUB_POS>(L"Player Purchase", V_MISC_EVENT_LOGS_PLAYER_PURCHASE);
		});

		add_tab<UI_SUB_POS>(L"Menu", [&]() {
			add_bool<UI_SUB_SUB_POS>(L"Menu MWheel Control", V_UI_MOUSE_WHEEL_NAVIGATION);
			add_float<UI_SUB_SUB_POS>(L"Menu Animation Speed", V_UI_SPEED_ANIMATION, 2.0f, 10.0f, 0.5f, true);

			add_int<UI_SUB_SUB_POS>(L"Menu R", V_UI_COL_R, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Menu G", V_UI_COL_G, 0, 255, 1);
			add_int<UI_SUB_SUB_POS>(L"Menu B", V_UI_COL_B, 0, 255, 1, true);

			add_int<UI_SUB_SUB_POS>(L"Menu A", V_UI_COL_A, 70, 170, 1);
		}, true);

		add_function<UI_SUB_POS>(L"Unload", []() {
			g::unload();
		});

		add_function<UI_SUB_POS>(L"Quit", []() {
			g_csgo.m_engine->execute_cmd("quit");
		});
	});

	add(L"Glow", [&]() {
		add_bool<UI_SUB_POS>(L"Enable", V_GLOW_ENABLED, true);

		add_bool<UI_SUB_POS>(L"Team", V_GLOW_TEAM);
		add_bool<UI_SUB_POS>(L"Visible", V_GLOW_VISIBLE_ONLY);
		add_bool<UI_SUB_POS>(L"Walking", V_GLOW_WALKING_ONLY, true);

		add_bool<UI_SUB_POS>(L"C4", V_GLOW_C4, true);

		add_tab<UI_SUB_POS>(L"Color", [&]() {
			add_tab<UI_SUB_SUB_POS>(L"Enemy", [&]() {
				add_int<UI_SUB_SUB_SUB_POS>(L"Enemy R", V_GLOW_ENEMY_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Enemy G", V_GLOW_ENEMY_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Enemy B", V_GLOW_ENEMY_COL_B, 0, 255, 1, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Enemy A", V_GLOW_ENEMY_COL_A, 130, 255, 1);
			});

			add_tab<UI_SUB_SUB_POS>(L"Team", [&]() {
				add_int<UI_SUB_SUB_SUB_POS>(L"Team R", V_GLOW_TEAM_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Team G", V_GLOW_TEAM_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"Team B", V_GLOW_TEAM_COL_B, 0, 255, 1, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"Team A", V_GLOW_TEAM_COL_A, 130, 255, 1);
			});

			add_tab<UI_SUB_SUB_POS>(L"C4", [&]() {
				add_int<UI_SUB_SUB_SUB_POS>(L"C4 R", V_GLOW_C4_COL_R, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"C4 G", V_GLOW_C4_COL_G, 0, 255, 1);
				add_int<UI_SUB_SUB_SUB_POS>(L"C4 B", V_GLOW_C4_COL_B, 0, 255, 1, true);

				add_int<UI_SUB_SUB_SUB_POS>(L"C4 A", V_GLOW_C4_COL_A, 130, 255, 1);
			}, true);

			add_bool<UI_SUB_SUB_POS>(L"Health Based", V_GLOW_HEALTH_BASED);
		});
	});

	add(L"Chams", [&]() {
		add_bool<UI_SUB_POS>(L"Enable", V_CHAMS_ENABLED, true);

		add_bool<UI_SUB_POS>(L"Team", V_CHAMS_TEAM);
		add_bool<UI_SUB_POS>(L"Visible", V_CHAMS_VISIBLE_ONLY);
		add_bool<UI_SUB_POS>(L"Walking", V_CHAMS_WALKING_ONLY, true);

		add_item<UI_SUB_POS>(L"Type", V_CHAMS_TYPE, { L"1", L"2", L"3" }, true);

		add_int<UI_SUB_POS>(L"Chams R", V_CHAMS_COL_R, 0, 255, 1);
		add_int<UI_SUB_POS>(L"Chams G", V_CHAMS_COL_G, 0, 255, 1);
		add_int<UI_SUB_POS>(L"Chams B", V_CHAMS_COL_B, 0, 255, 1, true);

		add_bool<UI_SUB_POS>(L"Health Based", V_CHAMS_HEALTH_BASED);
	});

	add(L"Keys", [&]() {
		add_tab<UI_SUB_POS>(L"On Toggle", [&]() {
			add_tab<UI_SUB_SUB_POS>(L"Aim", [&]() {
				add_key<UI_SUB_SUB_SUB_POS>(L"Aimbot", V_KEYS_ON_TOGGLE_AIMBOT);
				add_key<UI_SUB_SUB_SUB_POS>(L"Trigger", V_KEYS_ON_TOGGLE_TRIGGERBOT);
			});

			add_tab<UI_SUB_SUB_POS>(L"Misc", [&]() {
				add_key<UI_SUB_SUB_SUB_POS>(L"Thirdperson", V_KEYS_ON_TOGGLE_THIRDPERSON);
			});

			add_tab<UI_SUB_SUB_POS>(L"Other", [&]() {
				add_key<UI_SUB_SUB_SUB_POS>(L"Menu", V_KEYS_ON_TOGGLE_UI);
				add_key<UI_SUB_SUB_SUB_POS>(L"Panic", V_KEYS_ON_TOGGLE_PANIC);
			});
		});

		add_tab<UI_SUB_POS>(L"On Hold", [&]() {
			add_tab<UI_SUB_SUB_POS>(L"Misc", [&]() {
				add_key<UI_SUB_SUB_SUB_POS>(L"Blockbot", V_KEYS_ON_HOLD_BLOCKBOT);
			});
		});
	}, true);

	add(L"Load Settings", [&]() {
		for (const auto& cfg : cfgs) {
			add_function<UI_SUB_POS>(cfg, [cfg]() {
				cfg::load(cfg);
			});
		}
	});

	add(L"Save Settings", [&]() {
		for (const auto& cfg : cfgs) {
			add_function<UI_SUB_POS>(cfg, [cfg]() {
				cfg::save(cfg);
			});
		}
	});
}

void ui::on_reset()
{
	for (int i = 0; i < maxUISprites; i++)
		m_sprites[i].on_reset();
}

void ui::on_reset_end()
{
	for (int i = 0; i < maxUISprites; i++)
		m_sprites[i].on_reset_end();
}

void ui::clear()
{
	for (int i = 0; i < m_entry_sz; i++) {
		m_entry[i].m_name.clear();
		m_entry[i].m_space = false;
	}

	for (int i = 0; i < s_entry_sz[UI_SUB_POS]; i++) {
		s_entries[i][UI_SUB_POS].m_name.clear();
		s_entries[i][UI_SUB_POS].m_var.clear();

		s_entries[i][UI_SUB_POS].m_int_min = 0;
		s_entries[i][UI_SUB_POS].m_int_max = 0;
		s_entries[i][UI_SUB_POS].m_int_step = 0;

		s_entries[i][UI_SUB_POS].m_float_min = 0.0f;
		s_entries[i][UI_SUB_POS].m_float_max = 0.0f;
		s_entries[i][UI_SUB_POS].m_float_step = 0.0f;

		s_entries[i][UI_SUB_POS].m_space = false;
		s_entries[i][UI_SUB_POS].m_state = UI_NONE_STATE;

		s_entries[i][UI_SUB_POS].m_items.clear();
		s_entries[i][UI_SUB_POS].m_fn = []() {};
	}

	for (int i = 0; i < s_entry_sz[UI_SUB_SUB_POS]; i++) {
		s_entries[i][UI_SUB_SUB_POS].m_name.clear();
		s_entries[i][UI_SUB_SUB_POS].m_var.clear();

		s_entries[i][UI_SUB_SUB_POS].m_int_min = 0;
		s_entries[i][UI_SUB_SUB_POS].m_int_max = 0;
		s_entries[i][UI_SUB_SUB_POS].m_int_step = 0;

		s_entries[i][UI_SUB_SUB_POS].m_float_min = 0.0f;
		s_entries[i][UI_SUB_SUB_POS].m_float_max = 0.0f;
		s_entries[i][UI_SUB_SUB_POS].m_float_step = 0.0f;

		s_entries[i][UI_SUB_SUB_POS].m_space = false;
		s_entries[i][UI_SUB_SUB_POS].m_state = UI_NONE_STATE;

		s_entries[i][UI_SUB_SUB_POS].m_items.clear();
		s_entries[i][UI_SUB_SUB_POS].m_fn = []() {};
	}

	for (int i = 0; i < s_entry_sz[UI_SUB_SUB_SUB_POS]; i++) {
		s_entries[i][UI_SUB_SUB_SUB_POS].m_name.clear();
		s_entries[i][UI_SUB_SUB_SUB_POS].m_var.clear();

		s_entries[i][UI_SUB_SUB_SUB_POS].m_int_min = 0;
		s_entries[i][UI_SUB_SUB_SUB_POS].m_int_max = 0;
		s_entries[i][UI_SUB_SUB_SUB_POS].m_int_step = 0;

		s_entries[i][UI_SUB_SUB_SUB_POS].m_float_min = 0.0f;
		s_entries[i][UI_SUB_SUB_SUB_POS].m_float_max = 0.0f;
		s_entries[i][UI_SUB_SUB_SUB_POS].m_float_step = 0.0f;

		s_entries[i][UI_SUB_SUB_SUB_POS].m_space = false;
		s_entries[i][UI_SUB_SUB_SUB_POS].m_state = UI_NONE_STATE;

		s_entries[i][UI_SUB_SUB_SUB_POS].m_items.clear();
		s_entries[i][UI_SUB_SUB_SUB_POS].m_fn = []() {};
	}

	m_entry_sz = 0;

	s_entry_sz[UI_SUB_POS] = 0;
	s_entry_sz[UI_SUB_SUB_POS] = 0;
	s_entry_sz[UI_SUB_SUB_SUB_POS] = 0;
}