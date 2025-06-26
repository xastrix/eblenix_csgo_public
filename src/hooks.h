#pragma once

#include "globals.h"
#include "vars.h"
#include "helpers.h"
#include "ui.h"
#include "interfaces.h"
#include "signatures.h"
#include "math.h"
#include "fnv.h"

#include <MinHook.h>

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
	HK_DRAWSETCOLOR,
	HK_LEVELINITPOSTENTITY,
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

		MH_STATUS status = MH_CreateHook(m_src, m_target, m_orig);

		if (status != MH_OK)
			return;

		status = MH_EnableHook(m_src);

		if (status != MH_OK)
			return;

		m_hooked = true;
	}

	void hook(void* addr, void* target, void** orig) {
		if (m_hooked)
			return;

		m_addr = addr;
		m_target = target;
		m_orig = orig;

		MH_STATUS status = MH_CreateHook(m_src, m_target, m_orig);

		if (status != MH_OK)
			return;

		status = MH_EnableHook(m_src);

		if (status != MH_OK)
			return;

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

		MH_STATUS status = MH_DisableHook(m_src);

		if (status != MH_OK)
			return;

		status = MH_RemoveHook(m_src);

		if (status != MH_OK)
			return;

		m_hooked = false;
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