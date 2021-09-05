#include "CSGOAI.h"

CSGOAi::CSGOAi()
{
	this->game_info_handler = std::make_unique<GameInformationhandler>();
}

CSGOAi::CSGOAi(std::shared_ptr<GameInformationhandler> game_info_handler)
{
	this->game_info_handler = game_info_handler;
}

bool CSGOAi::init()
{
	if (!load_offsets("offsets.json"))
	{
		std::cout << "Offsets couldn't be read, make sure you have a valid offsets file" << std::endl;
		return false;
	}

	if (!load_config("config.json"))
	{
		std::cout << "Config couldn't be read, make sure you have a valid config" << std::endl;
		return false;
	}

	if (!load_navmesh("nav_mesh.json"))
	{
		std::cout << "Error loading/parsing Navmesh, make sure you have a valid nav-mesh file" << std::endl;
		return false;
	}

	if (!attach_to_csgo_process())
	{
		std::cout << "Error getting dll address " << std::endl;
		return false;
	}

	toggle_button.set_toggle_button(config.trigger_button);

	return true;
}


bool CSGOAi::load_config(const std::string& file_name)
{
	return ConfigReader::read_in_config_data(config, file_name);
}

bool CSGOAi::load_offsets(const std::string& file_name)
{
	return Offsets::load_offsets_from_file(file_name);
}

bool CSGOAi::load_navmesh(const std::string& file_name)
{
	return movement_strategy.load_in_navmesh(file_name);
}

bool CSGOAi::attach_to_csgo_process()
{
	return this->game_info_handler->init(config);
}

void CSGOAi::set_activated_behavior(const ActivatedFeatures& behavior)
{
	this->activated_behavior = behavior;
}

std::shared_ptr<GameInformationhandler> CSGOAi::get_game_info_handler()
{
	return this->game_info_handler;
}

void CSGOAi::update()
{
	if (!game_info_handler->is_attached_to_process())
		return;

	this->game_info_handler->update_game_information();

	if (activated_behavior.triggerBot)
		this->triggerbot.update(game_info_handler.get());
	if (activated_behavior.aimbot)
		this->aimbot.update(game_info_handler.get());
	if (activated_behavior.movement)
		this->movement_strategy.update(game_info_handler.get());
}

void CSGOAi::console_run()
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
			this->movement_strategy.update(game_info_handler.get());
		}
	}
}
