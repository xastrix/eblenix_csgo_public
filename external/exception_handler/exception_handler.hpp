#ifndef EXCEPTION_HANDLER_HPP
#define EXCEPTION_HANDLER_HPP

#include <windows.h>

namespace exception_handler
{
	LONG WINAPI custom_exception_filter(EXCEPTION_POINTERS* e);
}

#endif // EXCEPTION_HANDLER_HPP