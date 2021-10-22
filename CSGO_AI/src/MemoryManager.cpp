#include "MemoryManager.h"

MemoryManager::MemoryManager()
{
}

MemoryManager::~MemoryManager()
{
	if (process != NULL)
		CloseHandle(process);
}

bool MemoryManager::attach_to_process(const char* window_name)
{
	HWND handle = FindWindowA(NULL, window_name);
	if (!handle)
		return false;

	DWORD process_ID;
	GetWindowThreadProcessId(handle, &process_ID);
	process = OpenProcess(PROCESS_ALL_ACCESS, false, process_ID);
	if (!process)
		return false;

	Logging::log_success("Process found process ID: " + std::to_string(process_ID));

	return true;
}

DWORD MemoryManager::get_module_address(const char* module_name)
{
	HMODULE modules[1024];
	DWORD bytes_needed;

	if (!EnumProcessModules(process, modules, sizeof(modules), &bytes_needed))
		return 0;

	const int module_count = bytes_needed / sizeof(HMODULE);
	for (int i = 0; i < module_count; i++)
	{
		TCHAR name[MAX_PATH];
		GetModuleBaseName(process, modules[i], name, sizeof(name));

		if (!_tcscmp(name, module_name))
		{
			if (debug_print)
			{
				std::cout << module_name << " found address: ";
				print_4_byte_hex((DWORD)modules[i]);
				std::cout << std::endl;
			}
			return (DWORD)modules[i];
		}
	}

	return 0;
}

void MemoryManager::print_4_byte_hex(DWORD number)
{
	printf_s("0x%08x", number);
}
