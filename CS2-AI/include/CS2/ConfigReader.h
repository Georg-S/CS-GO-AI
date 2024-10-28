#pragma once
#include <string>
#include <wtypes.h>
#include <fstream>
#include <optional>
#include "Utility/json.hpp"
#include "Utility/Logging.h"

using nlohmann::json;

struct Config 
{
	std::string client_dll_name;
	std::string engine_dll_name;
	std::string  windowname;
	int delay = 0;
	DWORD trigger_button = 0;

	static std::optional<Config> read_in_config_data();
};
