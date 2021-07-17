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

	if (!movement_strategy.load_in_navmesh("nav_mesh.json"))
	{
		std::cout << "Error loading/parsing Navmesh, make sure you have a valid nav-mesh file" << std::endl;
		return false;
	}

	toggle_button.set_toggle_button(config.trigger_button);

	return true;
}

void CSGOAi::run()
{
	while (true)
	{
		toggle_button.update();

		if (toggle_button.was_clicked() && toggle_button.is_toggled())
			std::cout << "Toggled ON" << std::endl;
		if (toggle_button.was_clicked() && !toggle_button.is_toggled())
			std::cout << "Toggled OFF" << std::endl;

		if (toggle_button.is_toggled())
		{
			this->game_info_handler->update_game_information();
			this->triggerbot.update(game_info_handler.get());
			this->aimbot.update(game_info_handler.get());
			this->movement_strategy.load_in_navmesh("nav_mesh.json");
		}
	}
}
