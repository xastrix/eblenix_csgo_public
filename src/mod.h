#pragma once

#include "font_manager.h"
#include "input_manager.h"
#include "event_manager.h"
#include "hooks.h"

#include <chrono>
#include <thread>

enum _wfm_stat {
	WM_OK,
	WM_TIMEOUT,
};

namespace mod
{
	void init(void* I);

	namespace util
	{
		_wfm_stat wait_for_module(const _module_list module_index, const _module_list fallback_module_index = maxModules, int ms = 600)
		{
			const auto timeout_ms{ 50000 };
			auto waited_ms{ 0 };

			while (!GetModuleHandleA(GLOBAL(module_list[module_index]).c_str()))
			{
				if (waited_ms >= timeout_ms)
					return WM_TIMEOUT;

				if (fallback_module_index != maxModules)
					GLOBAL(module_list[module_index]) = GLOBAL(module_list[fallback_module_index]);

				std::this_thread::sleep_for(std::chrono::milliseconds(ms));
				waited_ms += ms;
			}

			return WM_OK;
		}
	}

	void undo();
}