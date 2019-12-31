#pragma once

#include "globals.h"
#include "vars.h"
#include "helpers.h"
#include "aimbot.h"
#include "triggerbot.h"
#include "knifebot.h"
#include "esp.h"
#include "visuals.h"
#include "movement.h"
#include "ui.h"
#include "interfaces.h"
#include "signatures.h"
#include "math.h"
#include "fnv.h"

#include <MinHook.h>
#include <intrin.h>
#include <sstream>

using create_move_fn = bool(__stdcall*)(float, i_user_cmd*);
using paint_traverse_fn = void(__thiscall*)(c_panel*, unsigned int, bool, bool);
using present_fn = long(D3DAPI*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
using reset_fn = long(D3DAPI*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
using do_post_screen_effects_fn = int(__thiscall*)(void*, int);
using scene_end_fn = void(__thiscall*)(void*);
using screen_viewmodel_fov_change_fn = float(__thiscall*)(uintptr_t*);
using get_screen_aspect_ratio_fn = float(__thiscall*)(void*, int, int);
using draw_model_execute_fn = void(__thiscall*)(c_model_render*, i_mat_render_ctx*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
using override_view_fn = void(__fastcall*)(uintptr_t*, void*, c_view_setup*);
using is_connected_fn = bool(__fastcall*)(void*);
using list_in_leaves_box_fn = int(__thiscall*)(void*, const vec3&, const vec3&, unsigned short*, int);
using sv_cheats_boolean_fn = bool(__thiscall*)(convar*);
using shutdown_fn = void(__fastcall*)(void*, void*);

struct hooks {
	void init();
	void undo();
private:
	MH_STATUS m_status{ MH_UNKNOWN };
};

inline hooks g_hooks;