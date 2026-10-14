#include <iostream>
#include <windows.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <filesystem>

using _NtOpenProcess = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, CLIENT_ID*);
using _NtAllocateVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID*, ULONG, PSIZE_T, ULONG, ULONG);
using _NtWriteVirtualMemory = NTSTATUS(NTAPI*)(HANDLE, PVOID, PVOID, SIZE_T, PSIZE_T);
using _NtCreateThreadEx = NTSTATUS(NTAPI*)(PHANDLE, ACCESS_MASK, POBJECT_ATTRIBUTES, HANDLE,
	PVOID, PVOID, ULONG, SIZE_T, SIZE_T, SIZE_T, PVOID);
using _LoadLibraryW = FARPROC(WINAPI*)(LPCWSTR);

static DWORD get_proc_id(const std::wstring& process_name)
{
	DWORD pid{};

	PROCESSENTRY32W proc_info{};
	proc_info.dwSize = sizeof(proc_info);

	const auto h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (h == INVALID_HANDLE_VALUE)
		return pid;

	Process32FirstW(h, &proc_info);

	while (Process32NextW(h, &proc_info))
	{
		if (proc_info.szExeFile == process_name) {
			pid = proc_info.th32ProcessID;
			break;
		}
	}

	CloseHandle(h);

	return pid;
}

template <typename T>
static T get_export(const char* mod_name, const char* function_name)
{
	return reinterpret_cast<T>(GetProcAddress(GetModuleHandleA(mod_name), function_name));
}

namespace fs = std::filesystem;
static int inject(const DWORD pid, const std::wstring& dll_name)
{
	CLIENT_ID         cid{};
	NTSTATUS          status{};
	HANDLE            proc{}, thread{};
	OBJECT_ATTRIBUTES oa{};
	PVOID             base_addr{};
	SIZE_T            bytes_written{};

	wchar_t exe_path[MAX_PATH];
	GetModuleFileNameW(NULL, exe_path, MAX_PATH);

	fs::path dll_path = fs::path(exe_path).parent_path() / dll_name;

	if (!fs::is_regular_file(dll_path)) {
		wprintf(L"DLL not found at %ls\n", dll_path.c_str());
		return 1;
	}

	SIZE_T dll_path_size = (dll_path.wstring().size() + 1) * sizeof(wchar_t);

	wprintf(L"Attempting to inject %ls into PID %lu\n", dll_name.c_str(), pid);

	InitializeObjectAttributes(&oa, NULL, 0, NULL, NULL);

	cid.UniqueProcess = (HANDLE)pid;
	cid.UniqueThread = NULL;
	status = get_export<_NtOpenProcess>("ntdll.dll", "NtOpenProcess")(
		&proc,
		PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
		&oa, &cid);

	if (status != 0L) {
		printf("NtOpenProcess failed with status 0x%X\n", status);
		return 1;
	}

	status = get_export<_NtAllocateVirtualMemory>("ntdll.dll", "NtAllocateVirtualMemory")(
		proc, &base_addr, 0, &dll_path_size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	if (status != 0L) {
		printf("NtAllocateVirtualMemory failed with status 0x%X\n", status);
		return 1;
	}

	status = get_export<_NtWriteVirtualMemory>("ntdll.dll", "NtWriteVirtualMemory")(
		proc, base_addr, (PVOID)dll_path.c_str(), (ULONG)dll_path_size, &bytes_written);

	if (status != 0L) {
		printf("NtWriteVirtualMemory failed with status 0x%X\n", status);
		return 1;
	}

	const auto ll = get_export<_LoadLibraryW>("kernel32.dll", "LoadLibraryW");

	if (!ll) {
		printf("Failed to get LoadLibraryW export from kernel32.dll\n");
		return 1;
	}

	status = get_export<_NtCreateThreadEx>("ntdll.dll", "NtCreateThreadEx")(
		&thread, 0x1fffff, 0, proc, ll, base_addr, FALSE, 0, 0, 0, 0);

	if (status != 0L) {
		printf("NtCreateThreadEx failed with status: 0x%X\n", status);
		return 1;
	}

	CloseHandle(thread);
	CloseHandle(proc);

	return 0;
}

int main(int argc, wchar_t* argv[])
{
	std::wstring process_name = (argc > 1) ? argv[1] : L"csgo.exe";
	std::wstring dll_name = (argc > 2) ? argv[2] : L"eblenix_csgo.dll";

	DWORD pid = get_proc_id(process_name.c_str());

	if (!pid) {
		wprintf(L"Game is not running... (%ls > %ls)\n", dll_name.c_str(), process_name.c_str());
		return 1;
	}

	return inject(pid, dll_name);
}