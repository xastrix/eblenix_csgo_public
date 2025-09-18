#pragma once

#include "../globals.h"
#include "../var_manager/var_manager.h"
#include "../input_manager/input_manager.h"
#include "../helpers/helpers.h"
#include "../hooks/hooks.h"

#include "../../csgo/fnv.h"
#include "../../csgo/interfaces/interfaces.h"
#include "../../csgo/signatures/signatures.h"
#include "../../csgo/event_manager/event_manager.h"

#include <common.h>
#include <fonts.h>
#include <chrono>
#include <thread>

struct dll_t {
	dll_t(const int32_t reason) : _reason(reason) {};
	void in(const int32_t reason, std::function<void(void)> fn) {
		if (_reason == reason) {
			fn();
		}
	}
private:
	int32_t _reason{};
};

namespace mod
{
	void init(void* I);

	namespace util
	{
		bool wait_for_module(const std::string& module, int ms = 500)
		{
			const auto start_time = std::chrono::steady_clock::now();

			while (true) {
				auto modules_loaded = true;

				if (!GetModuleHandleA(module.c_str()))
					modules_loaded = false;

				if (modules_loaded)
					return true;

				const auto elapsed_time = std::chrono::steady_clock::now() - start_time;
				if (std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() >= ms)
					break;

				std::this_thread::sleep_for(std::chrono::milliseconds{ 100 });
			}

			return false;
		}
	}

	namespace fonts
	{
		inline HANDLE m_astriumwep{};
		inline DWORD  m_num{};

		void init()
		{
			m_astriumwep = AddFontMemResourceEx(astriumwep_ttf, sizeof(astriumwep_ttf), NULL, &m_num);
		}

		void undo()
		{
			if (m_astriumwep) {
				RemoveFontMemResourceEx(m_astriumwep);
				m_astriumwep = NULL;
			}

			m_num = 0;
		}
	}

	void undo();
}