#pragma once

#include "globals.h"
#include "vars.h"
#include "helpers.h"
#include "ui.h"
#include "interfaces.h"
#include "signatures.h"
#include "math.h"
#ifdef LUA_ENABLED
#include "luas.h"
#endif
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
	HK_SVCHEATSBOOLEAN,
	HK_DRAWSETCOLOR,
	HK_LEVELINITPOSTENTITY,
	HK_SHUTDOWN,
	maxHooks,
};

struct hook_t {
	/* Set up a hook */
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

	/* Set up a hook method without index, for non-virtual functions */
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

	/* Check if the hook is currently active */
	bool is_hooked() {
		return m_hooked;
	}

	/* Remove the hook */
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
	int    m_index;     // Index of the virtual function in the vtable
	void*  m_addr;      // Address of the function or object containing the function
	void*  m_src;       // Original function pointer obtained from the vtable
	void*  m_target;    // Target function to hook (redirect to)
	void** m_orig;      // Pointer to the original function pointer (for calling original)
	bool   m_hooked;    // Hook status flag
};

class c_hooks {
public:
	void init();
	void undo();

	static std::shared_ptr<c_hooks> make_shared() {
		return std::shared_ptr<c_hooks>(new c_hooks());
	}

private:
	hook_t m_hooks[maxHooks]{};
};

inline std::shared_ptr<c_hooks> g_hooks = c_hooks::make_shared();