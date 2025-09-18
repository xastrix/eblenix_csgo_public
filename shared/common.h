#pragma once

#define CONFIG_DIRECTORY_NAME          "ebl/cfg/"
#define CSGO_CLASS_NAME                "Valve001"
#define LOADER_UNLOAD_HOOK_MESSAGE     "UNLOAD_EBLENIX_MSG"

#ifdef _DEBUG
#define _DBG_NOTIFY(ss) OutputDebugStringA(ss); \
                        DebugBreak();
#endif

enum fn_num {
	create_move_fn_index                 = 24,
	paint_traverse_fn_index              = 41,
	present_fn_index                     = 17,
	reset_fn_index                       = 16,
	do_post_screen_effects_fn_index      = 44,
	scene_end_fn_index                   = 9,
	screen_viewmodel_fov_change_fn_index = 35,
	get_screen_aspect_ratio_fn_index     = 101,
	draw_model_execute_fn_index          = 21,
	override_view_fn_index               = 18,
	is_connected_fn_index                = 27,
	list_in_leaves_box_fn_index          = 6,
	sv_cheats_boolean_fn_index           = 13,
	shutdown_fn_index                    = 4,
};