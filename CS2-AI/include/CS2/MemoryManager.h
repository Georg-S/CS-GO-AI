#pragma once
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <string>
#include <iomanip>
#include "Utility/Logging.h"

class MemoryManager
{
public:
	MemoryManager() = default;
	~MemoryManager();
	bool attach_to_process(const char* window_name);
	uintptr_t get_module_address(const char* module_name) const;
	void print_4_byte_hex(uintptr_t address) const;

	void read_string_from_memory(uintptr_t address, char* buffer, size_t size, bool* success = nullptr) const
	{
		if (!ReadProcessMemory(m_process, reinterpret_cast<LPVOID>(address), buffer, size, nullptr))
		{
			if (success)
				*success = false;
			if (debug_print)
				Logging::log_error("Error Reading Memory Error Code: " + std::to_string(GetLastError()));
		}
		else if (success) 
		{
			*success = false;
		}
	}

	template <typename type>
	[[nodiscard]] type read_memory(uintptr_t address, bool* success = nullptr) const
	{
		type result{};
		if (!ReadProcessMemory(m_process, reinterpret_cast<LPVOID>(address), &result, sizeof(type), nullptr))
		{
			if (success != nullptr)
				*success = false;
			if (debug_print)
				Logging::log_error("Error Reading Memory Error Code: " + std::to_string(GetLastError()));
		}
		else if (success) 
		{
			*success = true;
		}

		return result;
	}

	template <typename type>
	bool write_memory(uintptr_t address, const type& data)
	{
		if (!WriteProcessMemory(m_process, reinterpret_cast<LPVOID>(address), &data, sizeof(data), NULL) && debug_print)
		{
			Logging::log_error("Error Reading Memory Error Code: " + std::to_string(GetLastError()));
			return false;
		}
		return true;
	}

private:
	HANDLE m_process = nullptr;
	static constexpr bool debug_print = false;
};