#include "CS2/CS2AI.h"

CS2Ai::CS2Ai()
{
	m_game_info_handler = std::make_shared<GameInformationhandler>();
}

void CS2Ai::update()
{
	if (!m_game_info_handler->is_attached_to_process())
		return;

	m_game_info_handler->update_game_information();

	if (m_activated_behavior.triggerBot)
		m_triggerbot.update(m_game_info_handler.get());
	if (m_activated_behavior.aimbot)
		m_aimbot.update(m_game_info_handler.get());
	if (m_activated_behavior.movement)
		m_movement_strategy.update(m_game_info_handler.get());
}

bool CS2Ai::load_config()
{
	auto readConfig = Config::read_in_config_data();
	if (!readConfig)
	{
		Logging::log_error("Config couldn't be read, make sure you have a valid config");
		return false;
	}
	m_config = readConfig.value();

	return true;
}

bool CS2Ai::load_offsets()
{
	auto success = m_game_info_handler->loadOffsets();
	if (!success)
	{
		Logging::log_error("Offsets couldn't be read, make sure you have a valid offsets file");
		return false;
	}
	return true;
}

bool CS2Ai::load_navmesh()
{
	if (!m_game_info_handler->is_attached_to_process()) 
	{
		Logging::log_error("Not attached to process -> can't load navmesh");
		return false;
	}

	m_game_info_handler->update_game_information();
	const auto game_info = m_game_info_handler->get_game_information();

	m_movement_strategy.reset_loaded_navmesh();
	if (game_info.current_map == "")
	{
		Logging::log("Player is not on a map currently -> delaying navmesh loading");
		return false;
	}

	m_movement_strategy.handle_navmesh_load(std::string(game_info.current_map));
	bool loading_successful = m_movement_strategy.is_valid_navmesh_loaded();
	if(!loading_successful)
		Logging::log_error("Error loading / parsing Navmesh, make sure you have a valid nav - mesh file");

	return loading_successful;
}

bool CS2Ai::attach_to_cs2_process()
{
	return m_game_info_handler->init(m_config);
}

void CS2Ai::set_activated_behavior(const ActivatedFeatures& behavior)
{
	m_activated_behavior = behavior;
}

std::shared_ptr<GameInformationhandler> CS2Ai::get_game_info_handler() const
{
	return m_game_info_handler;
}
