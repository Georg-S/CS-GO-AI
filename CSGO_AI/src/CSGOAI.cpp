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

	if (!ConfigReader::read_in_config_data(config)) 
	{
		std::cout << "Config couldn't be read, make sure you have a valid config" << std::endl;
		return false;
	}

	this->game_info_handler = std::make_unique<GameInformationhandler>();
	if (!this->game_info_handler->init(config)) 
	{
		std::cout << "Error getting dll address " << std::endl;
		return false;
	}

	return true;
}

void CSGOAi::run()
{
	while (true) 
	{
		this->game_info_handler->update_game_information();
		this->triggerbot.update(game_info_handler.get());
		this->aimbot.update(game_info_handler.get());
	}
}
