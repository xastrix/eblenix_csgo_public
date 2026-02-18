#include "exception_handler.hpp"

#include <iostream>

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

static bool i{};

LONG WINAPI exception_handler::custom_exception_filter(EXCEPTION_POINTERS* e)
{
	if (!i) {
		SymInitialize(GetCurrentProcess(), NULL, TRUE);
		i = true;
	}

	auto get_line_info = [](EXCEPTION_POINTERS* e) -> const char* {
		DWORD64 addr = (DWORD64)e->ExceptionRecord->ExceptionAddress;

		IMAGEHLP_LINE64 line;
		line.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		DWORD disp;

		static char buffer[256]{};
		if (SymGetLineFromAddr64(GetCurrentProcess(), addr, &disp, &line)) {
			snprintf(buffer, sizeof(buffer), "%s:%lu", line.FileName, line.LineNumber);
			return buffer;
		}

		return "?";
	};

	printf("%s -> Exception 0x%X at address 0x%p\n",
		get_line_info(e),
		e->ExceptionRecord->ExceptionCode,
		e->ExceptionRecord->ExceptionAddress);

	if (e->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		e->ContextRecord->Eip += 1;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if (e->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
		return EXCEPTION_CONTINUE_EXECUTION;

	return EXCEPTION_CONTINUE_SEARCH;
}