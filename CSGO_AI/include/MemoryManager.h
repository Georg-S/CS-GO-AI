#pragma once
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <string>
#include <iomanip>
#include "Logging.h"

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	bool attach_to_process(const char* window_name);
	DWORD get_module_address(const char* module_name);
	void print_4_byte_hex(DWORD address);

	void read_string_from_memory(DWORD address, char* buffer, DWORD size, bool* success = nullptr) 
	{
		if (!ReadProcessMemory(process, (LPVOID)address, buffer, size, NULL))
		{
			if (success != nullptr)
				*success = false;
			if (debug_print)
				Logging::log_error("Error Reading Memory Error Code: " + std::to_string(GetLastError()));
		}
		else if (success != nullptr)
			*success = false;
	}

	template <typename type>
	type read_memory(DWORD address, bool* success = nullptr)
	{
		type result{};
		if (!ReadProcessMemory(process, (LPVOID)address, &result, sizeof(type), NULL)) 
		{
			if (success != nullptr)
				*success = false;
			if(debug_print)
				Logging::log_error("Error Reading Memory Error Code: " + std::to_string(GetLastError()));
		}
		else if (success != nullptr)
			*success = false;

		return result;
	}

	template <typename type>
	bool write_memory(DWORD address, const type& data)
	{
		if (!WriteProcessMemory(this->process, (LPVOID)address, &data, sizeof(data), NULL) && debug_print) 
		{
			Logging::log_error("Error Reading Memory Error Code: " + std::to_string(GetLastError()));
			return false;
		}
		return true;
	}

private:
	HANDLE process = NULL;
	static constexpr bool debug_print = false;
};