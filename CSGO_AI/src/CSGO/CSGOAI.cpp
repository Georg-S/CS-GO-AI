#include "CSGO/CSGOAI.h"

CSGOAi::CSGOAi()
{
	game_info_handler = std::make_unique<GameInformationhandler>();
}

bool CSGOAi::init()
{
	if (!load_offsets("offsets.json"))
	{
		Logging::log_error("Offsets couldn't be read, make sure you have a valid offsets file");
		return false;
	}

	if (!load_config("config.json"))
	{
		Logging::log_error("Config couldn't be read, make sure you have a valid config");
		return false;
	}

	if (!load_navmesh("nav_mesh.json"))
	{
		Logging::log_error("Error loading/parsing Navmesh, make sure you have a valid nav-mesh file");
		return false;
	}

	if (!attach_to_csgo_process())
	{
		Logging::log_error("Error getting dll address");
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
	return game_info_handler->init(config);
}

void CSGOAi::set_activated_behavior(const ActivatedFeatures& behavior)
{
	activated_behavior = behavior;
}

std::shared_ptr<GameInformationhandler> CSGOAi::get_game_info_handler()
{
	return game_info_handler;
}

void CSGOAi::update()
{
	if (!game_info_handler->is_attached_to_process())
		return;

	game_info_handler->update_game_information();

	if (activated_behavior.triggerBot)
		triggerbot.update(game_info_handler.get());
	if (activated_behavior.aimbot)
		aimbot.update(game_info_handler.get());
	if (activated_behavior.movement)
		movement_strategy.update(game_info_handler.get());
}
