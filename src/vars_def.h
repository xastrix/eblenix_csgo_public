#pragma once

#define R_COL_P "->r"
#define G_COL_P "->g"
#define B_COL_P "->b"
#define A_COL_P "->a"

/******************************************************************************/

#define V_AIMBOT_ENABLED "aimbot->enabled"
#define V_AIMBOT_SILENT "aimbot->silent"
#define V_AIMBOT_AUTOPISTOL "aimbot->autopistol"
#define V_AIMBOT_VISIBLE_CHECK "aimbot->visible_check"
#define V_AIMBOT_SMOKE_CHECK "aimbot->smoke_check"
#define V_AIMBOT_FLASH_CHECK "aimbot->flash_check"
#define V_AIMBOT_TEAMMATE_CHECK "aimbot->teammate_check"
#define V_AIMBOT_JUMP_CHECK "aimbot->jump_check"
#define V_AIMBOT_SCOPE_CHECK "aimbot->scope_check"

#define V_AIMBOT_ALL_WEAPONS "aimbot->all"
#define V_AIMBOT_ALL_WEAPONS_TYPE "aimbot->all->type"
#define V_AIMBOT_ALL_WEAPONS_BONE "aimbot->all->bone"
#define V_AIMBOT_ALL_WEAPONS_FOV "aimbot->all->fov"
#define V_AIMBOT_ALL_WEAPONS_SMOOTH "aimbot->all->smooth"

#define V_AIMBOT_TYPE_PISTOL "aimbot->type->pistol"
#define V_AIMBOT_TYPE_RIFLE "aimbot->type->rifle"
#define V_AIMBOT_TYPE_SNIPER "aimbot->type->sniper"
#define V_AIMBOT_TYPE_HEAVY "aimbot->type->heavy"
#define V_AIMBOT_TYPE_SMG "aimbot->type->smg"

#define V_AIMBOT_BONE_PISTOL "aimbot->bone->pistol"
#define V_AIMBOT_BONE_RIFLE "aimbot->bone->rifle"
#define V_AIMBOT_BONE_SNIPER "aimbot->bone->sniper"
#define V_AIMBOT_BONE_HEAVY "aimbot->bone->heavy"
#define V_AIMBOT_BONE_SMG "aimbot->bone->smg"

#define V_AIMBOT_FOV_PISTOL "aimbot->fov->pistol"
#define V_AIMBOT_SMOOTH_PISTOL "aimbot->smooth->pistol"
#define V_AIMBOT_FOV_RIFLE "aimbot->fov->rifle"
#define V_AIMBOT_SMOOTH_RIFLE "aimbot->smooth->rifle"
#define V_AIMBOT_FOV_SNIPER "aimbot->fov->sniper"
#define V_AIMBOT_SMOOTH_SNIPER "aimbot->smooth->sniper"
#define V_AIMBOT_FOV_HEAVY "aimbot->fov->heavy"
#define V_AIMBOT_SMOOTH_HEAVY "aimbot->smooth->heavy"
#define V_AIMBOT_FOV_SMG "aimbot->fov->smg"
#define V_AIMBOT_SMOOTH_SMG "aimbot->smooth->smg"

/******************************************************************************/

#define V_TRIGGERBOT_ENABLED "triggerbot->enabled"
#define V_TRIGGERBOT_FLASH_CHECK "triggerbot->flash_check"
#define V_TRIGGERBOT_SMOKE_CHECK "triggerbot->smoke_check"
#define V_TRIGGERBOT_TEAMMATE_CHECK "triggerbot->teammate_check"
#define V_TRIGGERBOT_JUMP_CHECK "triggerbot->jump_check"
#define V_TRIGGERBOT_SCOPE_CHECK "triggerbot->scope_check"

#define V_TRIGGERBOT_HITBOX_HEAD "triggerbot->hitbox->head"
#define V_TRIGGERBOT_HITBOX_BODY "triggerbot->hitbox->body"
#define V_TRIGGERBOT_HITBOX_ARMS "triggerbot->hitbox->arms"
#define V_TRIGGERBOT_HITBOX_LEGS "triggerbot->hitbox->legs"

/******************************************************************************/

#define V_KNIFEBOT_ENABLED "knifebot->enabled"
#define V_KNIFEBOT_FLASH_CHECK "knifebot->flash_check"
#define V_KNIFEBOT_SMOKE_CHECK "knifebot->smoke_check"
#define V_KNIFEBOT_TEAMMATE_CHECK "knifebot->teammate_check"
#define V_KNIFEBOT_DISTANCE "knifebot->distance"

/******************************************************************************/

#define V_ESP_ENABLED "esp->enabled"
#define V_ESP_TEAM "esp->team"
#define V_ESP_VISIBLE_ONLY "esp->visible_only"
#define V_ESP_WALKING_ONLY "esp->walking_only"
#define V_ESP_RENDER_TYPE "esp->render_type"

#define V_ESP_NAME_ENABLED "esp->name->enabled"
#define V_ESP_NAME_TYPE "esp->name->type"
#define V_ESP_NAME_COL "esp->name->col"
#define V_ESP_NAME_COL_R V_ESP_NAME_COL R_COL_P
#define V_ESP_NAME_COL_G V_ESP_NAME_COL G_COL_P
#define V_ESP_NAME_COL_B V_ESP_NAME_COL B_COL_P

#define V_ESP_BOX_ENABLED "esp->box->enabled"
#define V_ESP_BOX_TYPE "esp->box->type"
#define V_ESP_BOX_COL "esp->box->col"
#define V_ESP_BOX_COL_R V_ESP_BOX_COL R_COL_P
#define V_ESP_BOX_COL_G V_ESP_BOX_COL G_COL_P
#define V_ESP_BOX_COL_B V_ESP_BOX_COL B_COL_P

#define V_ESP_HEALTH_ENABLED "esp->health->enabled"
#define V_ESP_HEALTH_TYPE "esp->health->type"
#define V_ESP_HEALTH_BATTERY "esp->health->battery"
#define V_ESP_HEALTH_COL "esp->health->col"
#define V_ESP_HEALTH_COL_R V_ESP_HEALTH_COL R_COL_P
#define V_ESP_HEALTH_COL_G V_ESP_HEALTH_COL G_COL_P
#define V_ESP_HEALTH_COL_B V_ESP_HEALTH_COL B_COL_P

#define V_ESP_WEAPON_ENABLED "esp->weapon->enabled"
#define V_ESP_WEAPON_TYPE "esp->weapon->type"
#define V_ESP_WEAPON_COL "esp->weapon->col"
#define V_ESP_WEAPON_COL_R V_ESP_WEAPON_COL R_COL_P
#define V_ESP_WEAPON_COL_G V_ESP_WEAPON_COL G_COL_P
#define V_ESP_WEAPON_COL_B V_ESP_WEAPON_COL B_COL_P

#define V_ESP_ARMOR_ENABLED "esp->armor->enabled"
#define V_ESP_ARMOR_COL "esp->armor->col"
#define V_ESP_ARMOR_COL_R V_ESP_ARMOR_COL R_COL_P
#define V_ESP_ARMOR_COL_G V_ESP_ARMOR_COL G_COL_P
#define V_ESP_ARMOR_COL_B V_ESP_ARMOR_COL B_COL_P

#define V_ESP_SKELETON_ENABLED "esp->skeleton->enabled"
#define V_ESP_SKELETON_TYPE "esp->skeleton->type"
#define V_ESP_SKELETON_COL "esp->skeleton->col"
#define V_ESP_SKELETON_COL_R V_ESP_SKELETON_COL R_COL_P
#define V_ESP_SKELETON_COL_G V_ESP_SKELETON_COL G_COL_P
#define V_ESP_SKELETON_COL_B V_ESP_SKELETON_COL B_COL_P

#define V_ESP_BARREL_ENABLED "esp->barrel->enabled"
#define V_ESP_BARREL_COL "esp->barrel->col"
#define V_ESP_BARREL_COL_R V_ESP_BARREL_COL R_COL_P
#define V_ESP_BARREL_COL_G V_ESP_BARREL_COL G_COL_P
#define V_ESP_BARREL_COL_B V_ESP_BARREL_COL B_COL_P

#define V_ESP_CROSSHAIR_ENABLED "esp->crosshair->enabled"
#define V_ESP_CROSSHAIR_FOV "esp->crosshair->fov"
#define V_ESP_CROSSHAIR_COL "esp->crosshair->col"
#define V_ESP_CROSSHAIR_COL_R V_ESP_CROSSHAIR_COL R_COL_P
#define V_ESP_CROSSHAIR_COL_G V_ESP_CROSSHAIR_COL G_COL_P
#define V_ESP_CROSSHAIR_COL_B V_ESP_CROSSHAIR_COL B_COL_P
#define V_ESP_CROSSHAIR_COL_ON_TARGET_R V_ESP_CROSSHAIR_COL "->on_target" R_COL_P
#define V_ESP_CROSSHAIR_COL_ON_TARGET_G V_ESP_CROSSHAIR_COL "->on_target" G_COL_P
#define V_ESP_CROSSHAIR_COL_ON_TARGET_B V_ESP_CROSSHAIR_COL "->on_target" B_COL_P

#define V_ESP_SNAP_LINES_ENABLED "esp->snap_lines->enabled"
#define V_ESP_SNAP_LINES_COL "esp->snap_lines->col"
#define V_ESP_SNAP_LINES_COL_R V_ESP_SNAP_LINES_COL R_COL_P
#define V_ESP_SNAP_LINES_COL_G V_ESP_SNAP_LINES_COL G_COL_P
#define V_ESP_SNAP_LINES_COL_B V_ESP_SNAP_LINES_COL B_COL_P

#define V_ESP_FLAGS_ENABLED "esp->flags->enabled"
#define V_ESP_FLAGS_HK "esp->flags->hk"
#define V_ESP_FLAGS_HEALTH "esp->flags->health"
#define V_ESP_FLAGS_MONEY "esp->flags->money"
#define V_ESP_FLAGS_AMMO "esp->flags->ammo"
#define V_ESP_FLAGS_FLASHED "esp->flags->flashed"
#define V_ESP_FLAGS_DEFUSING "esp->flags->defusing"
#define V_ESP_FLAGS_DISTANCE "esp->flags->distance"
#define V_ESP_FLAGS_SCOPED "esp->flags->scoped"
#define V_ESP_FLAGS_COL "esp->flags->col"
#define V_ESP_FLAGS_COL_R V_ESP_FLAGS_COL R_COL_P
#define V_ESP_FLAGS_COL_G V_ESP_FLAGS_COL G_COL_P
#define V_ESP_FLAGS_COL_B V_ESP_FLAGS_COL B_COL_P
#define V_ESP_FLAGS_ITEMS_ENABLED "esp->flags->items->enabled"
#define V_ESP_FLAGS_ITEMS_COL "esp->flags->items->col"
#define V_ESP_FLAGS_ITEMS_COL_R V_ESP_FLAGS_ITEMS_COL R_COL_P
#define V_ESP_FLAGS_ITEMS_COL_G V_ESP_FLAGS_ITEMS_COL G_COL_P
#define V_ESP_FLAGS_ITEMS_COL_B V_ESP_FLAGS_ITEMS_COL B_COL_P

/******************************************************************************/

#define V_VISUALS_ENABLED "visuals->enabled"
#define V_VISUALS_INTERFACE_STATUS "visuals->interface->status"
#define V_VISUALS_INTERFACE_SPECTATORS "visuals->interface->spectators"

#define V_VISUALS_WORLD_PROJECTLINES_ENABLED "visuals->world->projectlines->enabled"
#define V_VISUALS_WORLD_PROJECTLINES_DISTANCE "visuals->world->projectlines->distance"
#define V_VISUALS_WORLD_PROJECTLINES_TYPE "visuals->world->projectlines->type"
#define V_VISUALS_WORLD_PROJECTLINES_COL "visuals->world->projectlines->col"
#define V_VISUALS_WORLD_PROJECTLINES_COL_R V_VISUALS_WORLD_PROJECTLINES_COL R_COL_P
#define V_VISUALS_WORLD_PROJECTLINES_COL_G V_VISUALS_WORLD_PROJECTLINES_COL G_COL_P
#define V_VISUALS_WORLD_PROJECTLINES_COL_B V_VISUALS_WORLD_PROJECTLINES_COL B_COL_P

#define V_VISUALS_WORLD_ITEMS_ENABLED "visuals->world->items->enabled"
#define V_VISUALS_WORLD_ITEMS_DISTANCE "visuals->world->items->distance"
#define V_VISUALS_WORLD_ITEMS_COL "visuals->world->items->col"
#define V_VISUALS_WORLD_ITEMS_COL_R V_VISUALS_WORLD_ITEMS_COL R_COL_P
#define V_VISUALS_WORLD_ITEMS_COL_G V_VISUALS_WORLD_ITEMS_COL G_COL_P
#define V_VISUALS_WORLD_ITEMS_COL_B V_VISUALS_WORLD_ITEMS_COL B_COL_P

#define V_VISUALS_WORLD_WEAPONS_ENABLED "visuals->world->weapons->enabled"
#define V_VISUALS_WORLD_WEAPONS_DISTANCE "visuals->world->weapons->distance"
#define V_VISUALS_WORLD_WEAPONS_AMMO_BAR "visuals->world->weapons->ammo_bar"
#define V_VISUALS_WORLD_WEAPONS_TYPE "visuals->world->weapons->type"
#define V_VISUALS_WORLD_WEAPONS_COL "visuals->world->weapons->col"
#define V_VISUALS_WORLD_WEAPONS_COL_R V_VISUALS_WORLD_WEAPONS_COL R_COL_P
#define V_VISUALS_WORLD_WEAPONS_COL_G V_VISUALS_WORLD_WEAPONS_COL G_COL_P
#define V_VISUALS_WORLD_WEAPONS_COL_B V_VISUALS_WORLD_WEAPONS_COL B_COL_P
#define V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL "visuals->world->weapons->ammo_bar->col"
#define V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_R V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL R_COL_P
#define V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_G V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL G_COL_P
#define V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL_B V_VISUALS_WORLD_WEAPONS_AMMO_BAR_COL B_COL_P

#define V_VISUALS_WORLD_C4_ENABLED "visuals->world->c4->enabled"
#define V_VISUALS_WORLD_C4_DAMAGE_INDICATOR "visuals->world->c4->damage_indicator"
#define V_VISUALS_WORLD_C4_TIME_BAR "visuals->world->c4->time_bar"
#define V_VISUALS_WORLD_C4_DEFUSE_BAR "visuals->world->c4->defuse_bar"
#define V_VISUALS_WORLD_C4_TYPE "visuals->world->c4->type"
#define V_VISUALS_WORLD_C4_COL "visuals->world->c4->col"
#define V_VISUALS_WORLD_C4_COL_R V_VISUALS_WORLD_C4_COL R_COL_P
#define V_VISUALS_WORLD_C4_COL_G V_VISUALS_WORLD_C4_COL G_COL_P
#define V_VISUALS_WORLD_C4_COL_B V_VISUALS_WORLD_C4_COL B_COL_P
#define V_VISUALS_WORLD_C4_COL_TIME_BAR "visuals->world->c4->time_bar->col"
#define V_VISUALS_WORLD_C4_COL_TIME_BAR_R V_VISUALS_WORLD_C4_COL_TIME_BAR R_COL_P
#define V_VISUALS_WORLD_C4_COL_TIME_BAR_G V_VISUALS_WORLD_C4_COL_TIME_BAR G_COL_P
#define V_VISUALS_WORLD_C4_COL_TIME_BAR_B V_VISUALS_WORLD_C4_COL_TIME_BAR B_COL_P
#define V_VISUALS_WORLD_C4_COL_DEFUSE_BAR "visuals->world->c4->defuse_bar->col"
#define V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_R V_VISUALS_WORLD_C4_COL_DEFUSE_BAR R_COL_P
#define V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_G V_VISUALS_WORLD_C4_COL_DEFUSE_BAR G_COL_P
#define V_VISUALS_WORLD_C4_COL_DEFUSE_BAR_B V_VISUALS_WORLD_C4_COL_DEFUSE_BAR B_COL_P

#define V_VISUALS_WORLD_NIGHTMODE_ENABLED "visuals->world->nightmode->enabled"
#define V_VISUALS_WORLD_NIGHTMODE_WORLD_BRIGHTNESS "visuals->world->nightmode->world->brightness"
#define V_VISUALS_WORLD_NIGHTMODE_SKY_BRIGHTNESS "visuals->world->nightmode->sky->brightness"
#define V_VISUALS_WORLD_NIGHTMODE_MODEL_BRIGHTNESS "visuals->world->nightmode->model_textures->brightness"
#define V_VISUALS_WORLD_NIGHTMODE_DECAL_BRIGHTNESS "visuals->world->nightmode->decal_textures->brightness"
#define V_VISUALS_WORLD_NIGHTMODE_OTHER_BRIGHTNESS "visuals->world->nightmode->other_textures->brightness"


#define V_VISUALS_REMOVALS_SMOKE "visuals->removals->smoke"
#define V_VISUALS_REMOVALS_FLASH "visuals->removals->flash"
#define V_VISUALS_REMOVALS_SCOPE "visuals->removals->scope"
#define V_VISUALS_REMOVALS_SLEEVES "visuals->removals->sleeves"
#define V_VISUALS_REMOVALS_HANDS "visuals->removals->hands"

/******************************************************************************/

#define V_GLOW_ENABLED "glow->enabled"
#define V_GLOW_VISIBLE_ONLY "glow->visible_only"
#define V_GLOW_WALKING_ONLY "glow->walking_only"
#define V_GLOW_TEAM "glow->team"
#define V_GLOW_HEALTH_BASED "glow->health_based"
#define V_GLOW_C4 "glow->c4"

#define V_GLOW_ENEMY_COL "glow->enemy->col"
#define V_GLOW_ENEMY_COL_R V_GLOW_ENEMY_COL R_COL_P
#define V_GLOW_ENEMY_COL_G V_GLOW_ENEMY_COL G_COL_P
#define V_GLOW_ENEMY_COL_B V_GLOW_ENEMY_COL B_COL_P
#define V_GLOW_ENEMY_COL_A V_GLOW_ENEMY_COL A_COL_P

#define V_GLOW_TEAM_COL "glow->team->col"
#define V_GLOW_TEAM_COL_R V_GLOW_TEAM_COL R_COL_P
#define V_GLOW_TEAM_COL_G V_GLOW_TEAM_COL G_COL_P
#define V_GLOW_TEAM_COL_B V_GLOW_TEAM_COL B_COL_P
#define V_GLOW_TEAM_COL_A V_GLOW_TEAM_COL A_COL_P

#define V_GLOW_C4_COL "glow->c4->col"
#define V_GLOW_C4_COL_R V_GLOW_C4_COL R_COL_P
#define V_GLOW_C4_COL_G V_GLOW_C4_COL G_COL_P
#define V_GLOW_C4_COL_B V_GLOW_C4_COL B_COL_P
#define V_GLOW_C4_COL_A V_GLOW_C4_COL A_COL_P

/******************************************************************************/

#define V_CHAMS_ENABLED "chams->enabled"
#define V_CHAMS_WALKING_ONLY "chams->walking_only"
#define V_CHAMS_VISIBLE_ONLY "chams->visible_only"
#define V_CHAMS_TEAM "chams->team"

#define V_CHAMS_HEALTH_BASED "chams->health_based"
#define V_CHAMS_TYPE "chams->type"

#define V_CHAMS_COL_R "chams->col" R_COL_P
#define V_CHAMS_COL_G "chams->col" G_COL_P
#define V_CHAMS_COL_B "chams->col" B_COL_P

/******************************************************************************/

#define V_MISC_VISUAL_ENGINE_RADAR "misc->visual->engine_radar"
#define V_MISC_VISUAL_INVENTORY_UNLOCK "misc->visual->inventory_unlock"
#define V_MISC_VISUAL_REVEAL_RANKS "misc->visual->reveal_ranks"
#define V_MISC_VISUAL_THIRDPERSON "misc->visual->thirdperson"
#define V_MISC_VISUAL_THIRDPERSON_FOV "misc->visual->thirdperson->fov"
#define V_MISC_VISUAL_ASPECT_RATIO "misc->visual->aspect_ratio"
#define V_MISC_VISUAL_ASPECT_RATIO_VALUE "misc->visual->aspect_ratio->value"
#define V_MISC_VISUAL_VIEWMODEL_FOV "misc->visual->viewmodel->fov"

#define V_MISC_MOVEMENT_BUNNYHOP "misc->movement->bunnyhop"
#define V_MISC_MOVEMENT_INFINITE_DUCK "misc->movement->infinite_duck"
#define V_MISC_MOVEMENT_AUTO_STRAFE "misc->movement->auto_strafe"

#define V_MISC_EVENT_LOGS_PLAYER_HURT "misc->event_logs->player_hurt"
#define V_MISC_EVENT_LOGS_PLAYER_PURCHASE "misc->event_logs->player_purchase"

/******************************************************************************/

#define V_UI_POS_X "ui->pos->x"
#define V_UI_POS_Y "ui->pos->y"

#define V_UI_COL "ui->col"
#define V_UI_COL_R V_UI_COL R_COL_P
#define V_UI_COL_G V_UI_COL G_COL_P
#define V_UI_COL_B V_UI_COL B_COL_P
#define V_UI_COL_A V_UI_COL A_COL_P

#define V_UI_MOUSE_WHEEL_NAVIGATION "ui->mouse_wheel_navigation"

/******************************************************************************/

#define V_KEYS_ON_TOGGLE_AIMBOT "keys->on_toggle->aimbot"
#define V_KEYS_ON_TOGGLE_TRIGGERBOT "keys->on_toggle->triggerbot"
#define V_KEYS_ON_TOGGLE_THIRDPERSON "keys->on_toggle->thirdperson"
#define V_KEYS_ON_TOGGLE_PANIC "keys->on_toggle->panic"
#define V_KEYS_ON_TOGGLE_UI "keys->on_toggle->ui"

#define V_KEYS_ON_HOLD_BLOCKBOT "keys->on_hold->blockbot"