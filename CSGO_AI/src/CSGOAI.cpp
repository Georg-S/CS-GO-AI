#include "CSGOAI.h"

CSGOAi::CSGOAi()
{
}

bool CSGOAi::init()
{
	if (!Offsets::load_offsets_from_file("offsets.json"))
	{
		std::cout << "Offsets couldn't be read, make sure you have a valid offsets file" << std::endl;
		return false;
	}

	ConfigData data;
	if (!ConfigReader::read_in_config_data(data)) 
	{
		std::cout << "Config couldn't be read, make sure you have a valid config" << std::endl;
		return false;
	}

	this->gameInfoHandler = std::make_unique<GameInformationhandler>(data);

	return true;
}
