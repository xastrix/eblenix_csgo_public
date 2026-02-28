#pragma once

#define _PRODUCT_NAME              "EBLENIX"

#define CSGO_CLASS_NAME            "Valve001"
#define LOADER_UNLOAD_HOOK_MESSAGE "UNLOAD_EBLENIX_MSG"

enum fn_num {
	CREATE_MOVE_FN_INDEX                 = 24,
	PAINT_TRAVERSE_FN_INDEX              = 41,
	PRESENT_FN_INDEX                     = 17,
	RESET_FN_INDEX                       = 16,
	DO_POST_SCREEN_EFFECTS_FN_INDEX      = 44,
	SCENE_END_FN_INDEX                   = 9,
	SCREEN_VIEWMODEL_FOV_CHANGE_FN_INDEX = 35,
	GET_SCREEN_ASPECT_RATIO_FN_INDEX     = 101,
	DRAW_MODEL_EXECUTE_FN_INDEX          = 21,
	OVERRIDE_VIEW_FN_INDEX               = 18,
	IS_CONNECTED_FN_INDEX                = 27,
	SV_CHEATS_BOOLEAN_FN_INDEX           = 13,
	DRAW_SET_COLOR_FN_INDEX              = 15,
	LEVEL_INIT_POST_ENTITY_FN_INDEX      = 6,
	SHUTDOWN_FN_INDEX                    = 4,
};