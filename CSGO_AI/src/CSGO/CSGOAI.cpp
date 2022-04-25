#include "CSGO/CSGOAI.h"

CSGOAi::CSGOAi()
{
	game_info_handler = std::make_shared<GameInformationhandler>();
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

bool CSGOAi::load_config(const std::string& file_name)
{
	auto readConfig = Config::read_in_config_data(file_name);
	if (!readConfig)
	{
		Logging::log_error("Config couldn't be read, make sure you have a valid config");
		return false;
	}
	config = readConfig.value();

	return true;
}

bool CSGOAi::load_offsets(const std::string& file_name)
{
	if (!Offsets::load_offsets_from_file(file_name)) 
	{
		Logging::log_error("Offsets couldn't be read, make sure you have a valid offsets file");
		return false;
	}
	return true;
}

bool CSGOAi::load_navmesh()
{
	if (!game_info_handler->is_attached_to_process()) 
	{
		Logging::log_error("Not attached to process -> can't load navmesh");
		return false;
	}

	game_info_handler->update_game_information();
	const auto game_info = game_info_handler->get_game_information();

	movement_strategy.reset_loaded_navmesh();
	if (game_info.current_map == "")
	{
		Logging::log("Player is not on a map currently -> delaying navmesh loading");
		return false;
	}

	movement_strategy.handle_navmesh_load(std::string(game_info.current_map));
	bool loading_successful = movement_strategy.is_valid_navmesh_loaded();
	if(!loading_successful)
		Logging::log_error("Error loading / parsing Navmesh, make sure you have a valid nav - mesh file");

	return loading_successful;
}

bool CSGOAi::attach_to_csgo_process()
{
	return game_info_handler->init(config);
}

void CSGOAi::set_activated_behavior(const ActivatedFeatures& behavior)
{
	activated_behavior = behavior;
}

std::shared_ptr<GameInformationhandler> CSGOAi::get_game_info_handler() const
{
	return game_info_handler;
}
