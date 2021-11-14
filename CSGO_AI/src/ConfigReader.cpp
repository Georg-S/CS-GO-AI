#include "CSGO/ConfigReader.h"

bool ConfigReader::read_in_config_data(ConfigData& destination, const std::string& file_name)
{
	try
	{
		std::ifstream ifs(file_name);
		auto config_json = json::parse(ifs);

		destination.client_dll_name = config_json["client_dll_name"];
		destination.engine_dll_name = config_json["engine_dll_name"];
		destination.windowname = config_json["window_name"];
		destination.trigger_button = (DWORD)config_json["trigger_button"];
		destination.delay = (int)config_json["delay"];
	}
	catch (std::exception const& e)
	{
		Logging::log_error(std::string(e.what()));
		return false;
	}

	return true;
}
