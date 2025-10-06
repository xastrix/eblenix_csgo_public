#pragma once

#include "../globals.h"
#include "../font_manager/font_manager.h"
#include "../var_manager/var_manager.h"
#include "../input_manager/input_manager.h"
#include "../helpers/helpers.h"
#include "../hooks/hooks.h"

#include "../../csgo/fnv.h"
#include "../../csgo/interfaces/interfaces.h"
#include "../../csgo/signatures/signatures.h"
#include "../../csgo/event_manager/event_manager.h"

#include <common.h>
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
		_wfm_stat wait_for_module(const _module_list module_index, std::function<void(void)> fn = []() {}, int ms = 600)
		{
			const auto timeout_ms{ 40000 };
			auto waited_ms{ 0 };

			while (!GetModuleHandleA(g.module_list[module_index].c_str()))
			{
				if (waited_ms >= timeout_ms)
					return WM_TIMEOUT;

				fn();

				std::this_thread::sleep_for(std::chrono::milliseconds(ms));
				waited_ms += ms;
			}

			return WM_OK;
		}
	}

	void undo();
}