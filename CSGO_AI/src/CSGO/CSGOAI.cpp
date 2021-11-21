#include "CSGO/CSGOAI.h"

CSGOAi::CSGOAi()
{
	game_info_handler = std::make_shared<GameInformationhandler>();
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
	if (!game_info_handler->is_attached_to_process()) 
	{
		Logging::log_error("Not attached to process -> can't load navmesh");
		return false;
	}

	game_info_handler->update_game_information();
	auto game_info = game_info_handler->get_game_information();

	movement_strategy.reset_loaded_navmesh();
	movement_strategy.handle_navmesh_load(std::string(game_info.current_map));

	return movement_strategy.is_valid_navmesh_loaded();
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
