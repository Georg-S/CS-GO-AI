#pragma once
#include <Windows.h>
#include <iostream>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <string>
#include <iomanip>

class MemoryManager
{
public:
	MemoryManager();
	~MemoryManager();
	bool attach_to_process(const char* window_name);
	DWORD get_module_address(const char* module_name);
	void print_4_byte_hex(DWORD address);

	template <typename type>
	type read_memory(DWORD address)
	{
		type result;
		if (!ReadProcessMemory(process, (LPVOID)address, &result, sizeof(type), NULL) && debug_print)
			std::cout << "Error Reading Memory Error Code: " << GetLastError() << std::endl;
		return result;
	}

	template <typename type>
	bool write_memory(DWORD address, const type& data)
	{
		if (!WriteProcessMemory(this->process, (LPVOID)address, &data, sizeof(data), NULL) && debug_print) 
		{
			std::cout << "Error writing to memory Error Code: " << GetLastError() << std::endl;
			return false;
		}
		return true;
	}

private:
	HANDLE process = NULL;
	static constexpr bool debug_print = true;
};