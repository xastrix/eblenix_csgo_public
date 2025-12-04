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

enum _hk_list {
	HK_CREATEMOVE,
	HK_PAINTTRAVERSE,
	HK_PRESENT,
	HK_RESET,
	HK_DOPOSTSCREENEFFECTS,
	HK_SCENEEND,
	HK_SCREENMODELFOVCHANGE,
	HK_GETSCREENASPECTRATIO,
	HK_DRAWMODELEXECUTE,
	HK_OVERRIDEVIEW,
	HK_ISCONNECTED,
	HK_LISTLEAVESINBOX,
	HK_SVCHEATSBOOLEAN,
	HK_SHUTDOWN,
	maxHooks,
};

struct hook_t {
	template <typename T, int index>
	void hook(void* addr, void* target, void** orig) {
		if (m_hooked)
			return;

		m_index = index;
		m_addr = addr;
		m_src = Helpers::get_virtual_fn<T>(m_addr, m_index);
		m_target = target;
		m_orig = orig;

		MH_STATUS status{};

		status = MH_CreateHook(m_src, m_target, m_orig);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to create %s hook\n", get_hook_name_by_id(get_hook_id_by_index(m_index)));
#endif
			return;
		}

		status = MH_EnableHook(m_src);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to enable %s hook\n", get_hook_name_by_id(get_hook_id_by_index(m_index)));
#endif
			return;
		}

		m_hooked = true;
	}

	int get_index() {
		return m_index;
	}

	bool is_hooked() {
		return m_hooked;
	}

	void unhook() {
		if (!m_hooked)
			return;

		MH_STATUS status{};

		status = MH_DisableHook(m_src);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to disable %s hook\n", get_hook_name_by_id(get_hook_id_by_index(m_index)));
#endif
			return;
		}

		status = MH_RemoveHook(m_src);

		if (status != MH_OK) {
#ifdef _DEBUG
			printf("Failed to remove %s hook\n", get_hook_name_by_id(get_hook_id_by_index(m_index)));
#endif
			return;
		}

		m_hooked = false;
	}

private:
	const char* get_hook_name_by_id(_hk_list id) {
		switch (id) {
		case HK_CREATEMOVE:
			return "Create move";
		case HK_PAINTTRAVERSE:
			return "Paint traverse";
		case HK_PRESENT:
			return "Present";
		case HK_RESET:
			return "Reset";
		case HK_DOPOSTSCREENEFFECTS:
			return "Do post screen effects";
		case HK_SCENEEND:
			return "Scene end";
		case HK_SCREENMODELFOVCHANGE:
			return "Screen model fov change";
		case HK_GETSCREENASPECTRATIO:
			return "Get aspect ratio";
		case HK_DRAWMODELEXECUTE:
			return "Draw model execute";
		case HK_OVERRIDEVIEW:
			return "Override view";
		case HK_ISCONNECTED:
			return "Is connected";
		case HK_LISTLEAVESINBOX:
			return "List leaves in box";
		case HK_SVCHEATSBOOLEAN:
			return "Sv cheats boolean";
		case HK_SHUTDOWN:
			return "Shutdown";
		}
	}

	_hk_list get_hook_id_by_index(int index) {
		switch (index) {
		case CREATE_MOVE_FN_INDEX:
			return HK_CREATEMOVE;
		case PAINT_TRAVERSE_FN_INDEX:
			return HK_PAINTTRAVERSE;
		case PRESENT_FN_INDEX:
			return HK_PRESENT;
		case RESET_FN_INDEX:
			return HK_RESET;
		case DO_POST_SCREEN_EFFECTS_FN_INDEX:
			return HK_DOPOSTSCREENEFFECTS;
		case SCENE_END_FN_INDEX:
			return HK_SCENEEND;
		case SCREEN_VIEWMODEL_FOV_CHANGE_FN_INDEX:
			return HK_SCREENMODELFOVCHANGE;
		case GET_SCREEN_ASPECT_RATIO_FN_INDEX:
			return HK_GETSCREENASPECTRATIO;
		case DRAW_MODEL_EXECUTE_FN_INDEX:
			return HK_DRAWMODELEXECUTE;
		case OVERRIDE_VIEW_FN_INDEX:
			return HK_OVERRIDEVIEW;
		case IS_CONNECTED_FN_INDEX:
			return HK_ISCONNECTED;
		case LIST_IN_LEAVES_BOX_FN_INDEX:
			return HK_LISTLEAVESINBOX;
		case SV_CHEATS_BOOLEAN_FN_INDEX:
			return HK_SVCHEATSBOOLEAN;
		case SHUTDOWN_FN_INDEX:
			return HK_SHUTDOWN;
		}
	}

private:
	int    m_index{};
	void*  m_addr{};
	void*  m_src{};
	void*  m_target{};
	void** m_orig{};
	bool   m_hooked{};
};

struct hooks {
	void init();
	void undo();
private:
	hook_t m_hooks[maxHooks];
};

inline hooks g_hooks;