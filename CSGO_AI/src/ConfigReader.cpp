#include "ConfigReader.h"

bool ConfigReader::readInConfigData(ConfigData& destination)
{
	static constexpr int DEC = 10;
	static constexpr int HEX = 16;

	destination.windowname = FileReader::read_value_of_string_in_file("config.txt", "windowName");
	destination.client_dll_name = FileReader::read_value_of_string_in_file("config.txt", "client Dll Name");
	destination.engine_dll_name = FileReader::read_value_of_string_in_file("config.txt", "engine Dll Name");
	destination.delay = stol(FileReader::read_value_of_string_in_file("config.txt", "delay"), NULL, DEC);
	destination.trigger_button = stol(FileReader::read_value_of_string_in_file("config.txt", "triggerButton"), NULL, HEX);

	if (destination.windowname == "" || destination.client_dll_name == "" || destination.trigger_button == 0)
		return false;

	return false;
}
