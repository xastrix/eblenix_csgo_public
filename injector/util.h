#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <time.h>

#ifndef STATUS_SUCCESS
#define STATUS_SUCCESS (NTSTATUS)0L
#endif

#define ERR_MSG_LEN 256
#define MAX_FILES   1024

using _NtOpenProcess = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, CLIENT_ID*);
using _NtAllocateVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID*, ULONG, PSIZE_T, ULONG, ULONG);
using _NtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
using _NtCreateThreadEx = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE,
	PVOID, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PVOID);

using _LoadLibraryA = FARPROC(WINAPI*)(LPCSTR);

namespace util
{
	inline auto get_pID(const std::string& process_name) -> DWORD
	{
		auto ret = static_cast<DWORD>(0);

		{
			PROCESSENTRY32 proc_info{};
			proc_info.dwSize = sizeof(proc_info);

			const auto h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
			if (h == INVALID_HANDLE_VALUE)
				return ret;

			Process32First(h, &proc_info);

			while (Process32Next(h, &proc_info))
			{
				if (proc_info.szExeFile == process_name) {
					ret = proc_info.th32ProcessID;
					break;
				}
			}

			CloseHandle(h);
		}

		return ret;
	}

	inline auto file_exists(const std::string& path) -> bool
	{
		struct stat s{};
		stat(path.c_str(), &s);

		return (s.st_mode & S_IFREG);
	}

	inline auto send_msg_to_proc(const HWND target, const char* msg) -> void
	{
		SIZE_T         cb_data{ static_cast<DWORD>(strlen(msg) + 1) };
		COPYDATASTRUCT cds{ 1, cb_data, const_cast<char*>(msg) };

		SendMessageA(target, WM_COPYDATA, (WPARAM)nullptr, (LPARAM)&cds);
	}

	inline auto get_current_path() -> std::string
	{
		char path[MAX_PATH];
		GetCurrentDirectoryA(MAX_PATH, path);

		return std::string{ path } + "\\";
	}

	inline auto is_dll_used(const std::string& dll_path) -> bool
	{
		bool ret{};

		auto h = CreateFileA(
			dll_path.c_str(),
			GENERIC_WRITE,
			0,
			nullptr,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			nullptr);

		if (h == INVALID_HANDLE_VALUE)
			ret = (GetLastError() == ERROR_SHARING_VIOLATION || GetLastError() == ERROR_LOCK_VIOLATION);

		CloseHandle(h);

		return ret;
	}

	inline auto random_string(int len, const std::string& chars = "qwertyuiopasdfghjklzxcvbnm") -> std::string
	{
		std::string ss{};

		auto dictonary_length = chars.length() - 1;
		ss.reserve(len);

		for (unsigned int i = 0; i < len; i++) {
			unsigned int random_index = rand() % dictonary_length;
			ss += chars.at(random_index);
		}

		return ss;
	}

	template <typename T>
	inline auto get_export(const char* mod_name, const char* function_name) -> T
	{
		return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(mod_name), function_name));
	}

	inline auto inject(const DWORD pid, const std::string& dll_name) -> bool
	{
		CLIENT_ID         cid{};
		NTSTATUS          status{};
		HANDLE            proc{},
			              thread{};
		OBJECT_ATTRIBUTES oa{};
		PVOID             base_addr{};
		SIZE_T            bytes_written{};

		std::string dll_path = get_current_path() + dll_name;

		SIZE_T dll_size{ dll_path.size() + 1 };
		if (!file_exists(dll_path))
			return false;

		InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);

		cid.UniqueProcess              = (HANDLE)pid;
		cid.UniqueThread               = NULL;
		status                         = get_export<_NtOpenProcess>("ntdll.dll", "NtOpenProcess")(
			&proc,
			PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
			PROCESS_VM_OPERATION  | PROCESS_VM_WRITE | PROCESS_VM_READ,
			&oa, &cid);

		if (status != STATUS_SUCCESS)
			return false;

		status                         = get_export<_NtAllocateVirtualMemory>("ntdll.dll", "NtAllocateVirtualMemory")(
			proc, &base_addr, 0,
			&dll_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE
		);

		if (status != STATUS_SUCCESS)
			return false;

		status                         = get_export<_NtWriteVirtualMemory>("ntdll.dll", "NtWriteVirtualMemory")(
			proc, base_addr,
			(PVOID)dll_path.c_str(),
			(ULONG)dll_size, &bytes_written
		);

		if (status != STATUS_SUCCESS)
			return false;

		const auto load_library        = get_export<_LoadLibraryA>("kernel32.dll", "LoadLibraryA");

		if (!load_library)
			return false;

		status                         = get_export<_NtCreateThreadEx>("ntdll.dll", "NtCreateThreadEx")(
			&thread, 0x1fffff, 0, proc,
			load_library, base_addr, FALSE,
			0, 0, 0, 0
		);

		if (status != STATUS_SUCCESS)
			return false;

		CloseHandle(thread);
		CloseHandle(proc);

		return true;
	}
}