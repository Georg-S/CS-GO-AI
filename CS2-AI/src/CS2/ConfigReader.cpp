#include "CS2/ConfigReader.h"

#include "CS2/Constants.h"

std::optional<Config> Config::read_in_config_data()
{
	try
	{
		std::ifstream ifs(config_file_path);
		auto config_json = json::parse(ifs);

		Config result;
		result.client_dll_name = config_json["client_dll_name"];
		result.engine_dll_name = config_json["engine_dll_name"];
		result.windowname = config_json["window_name"];
		result.trigger_button = static_cast<DWORD>(config_json["trigger_button"]);
		result.delay = static_cast<int>(config_json["delay"]);

		return result;
	}
	catch (std::exception const& e)
	{
		Logging::log_error(std::string(e.what()));
	}
	return std::nullopt;
}
