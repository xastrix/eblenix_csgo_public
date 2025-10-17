#pragma once

#include "globals.h"
#include "font_manager.h"
#include "var_manager.h"
#include "input_manager.h"
#include "helpers.h"
#include "hooks.h"
#include "fnv.h"
#include "interfaces.h"
#include "signatures.h"
#include "event_manager.h"

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
			const auto timeout_ms{ 50000 };
			auto waited_ms{ 0 };

			while (!GetModuleHandleA(g::module_list[module_index].c_str()))
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