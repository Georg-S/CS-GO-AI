#pragma once
#include <string>
#include <wtypes.h>
#include "FileReader.h"

struct ConfigData
{
	std::string client_dll_name;
	std::string engine_dll_name;
	std::string  windowname;
	int delay = 0;
	DWORD trigger_button = 0;
};


class ConfigReader 
{
public:
	static bool read_in_config_data(ConfigData& destination);
};