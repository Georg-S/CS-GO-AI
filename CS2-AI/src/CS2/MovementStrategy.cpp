#include "CS2/MovementStrategy.h"

void MovementStrategy::update(GameInformationhandler* game_info_handler)
{
	const GameInformation game_info = game_info_handler->get_game_information();
	const auto current_time_ms = get_current_time_in_ms();

	handle_navmesh_load(std::string(game_info.current_map));

	if (!m_valid_navmesh_loaded)
		return;

	if (!game_info.closest_enemy_player || !game_info.controlled_player.health)
	{
		m_next_node = nullptr;
		// Add delay here, because it can happen that the health is not zero anymore but the new position is not yet set
		m_delay_time = current_time_ms + 1500; 
		return;
	}


	if (current_time_ms < m_delay_time)
	{
		game_info_handler->set_player_movement(Movement{});
		return;
	}

	if (game_info.player_in_crosshair && (game_info.player_in_crosshair->team != game_info.controlled_player.team))
	{
		constexpr int delay_in_ms = 500;
		game_info_handler->set_player_movement(Movement{});
		m_delay_time = current_time_ms + delay_in_ms;
		return;
	}

	const Vec3D<float> player_pos = game_info.controlled_player.position;

	if (!m_next_node)
		m_next_node = get_closest_node_to_position(player_pos);

	auto distance = m_next_node->position.distance(player_pos);
	if (distance <= 15)
	{
		auto current_closest_enemy_node = get_closest_node_to_position(game_info.closest_enemy_player->position);
		m_current_route = Dijkstra::get_route(m_next_node, current_closest_enemy_node);
		if (m_current_route.size() > 1)
			m_next_node = m_current_route[1];

		if (m_debug_print_route)
			Dijkstra::log_route(m_current_route);
	}

	if (m_next_node)
	{
		Movement move = calculate_move_info(game_info, m_next_node);
		game_info_handler->set_player_movement(move);
	}
}

void MovementStrategy::handle_navmesh_load(const std::string& map_name)
{
	if (map_name == m_loaded_map)
		return;

	if (map_name == "")
	{
		m_loaded_map = "";
		m_valid_navmesh_loaded = false;
		return;
	}

	m_loaded_map = map_name;

	std::string processed_map_name = m_loaded_map;
	std::replace(processed_map_name.begin(), processed_map_name.end(), '/', '_');

	std::string file_path = "Navmesh/json/" + processed_map_name + ".json";
	if (load_in_navmesh(file_path))
		m_valid_navmesh_loaded = true;
}

bool MovementStrategy::load_in_navmesh(const std::string& filename)
{
	try
	{
		std::ifstream ifs(filename);

		m_navmesh_json = json::parse(ifs);
		ifs.close();

		load_nodes(m_navmesh_json);
		load_edges(m_navmesh_json);
	}
	catch (std::exception const& e)
	{
		Logging::log_error(std::string(e.what()));
		return false;
	}
	return true;
}

void MovementStrategy::set_debug_print_route(bool value)
{
	m_debug_print_route = value;
}

void MovementStrategy::reset_loaded_navmesh()
{
	m_valid_navmesh_loaded = false;
	m_loaded_map = "";
}

bool MovementStrategy::is_valid_navmesh_loaded() const
{
	return m_valid_navmesh_loaded;
}

Movement MovementStrategy::calculate_move_info(const GameInformation& game_info, const std::shared_ptr<Node> node)
{
	if (!game_info.closest_enemy_player)
		return Movement{};

	float position_angle = calc_angle_between_two_positions(game_info.controlled_player.head_position, node->position);
	float walk_angle = calc_walk_angle(game_info.controlled_player.view_vec.y, position_angle);
	return get_movement_from_walking_angle(walk_angle);
}

float MovementStrategy::calc_angle_between_two_positions(const Vec3D<float>& pos1, const Vec3D<float>& pos2) const
{
	auto vec = pos2 - pos1;
	return (std::atan2(vec.y, vec.x) / PI * 180);
}

float MovementStrategy::calc_walk_angle(float view_angle, float position_angle) const
{
	float new_angle = position_angle - view_angle;
	if (new_angle < 0)
		new_angle += 360;

	return new_angle;
}

Movement MovementStrategy::get_movement_from_walking_angle(float walking_angle) const
{
	Movement new_movement;

	if ((walking_angle > 360) || (walking_angle < 0))
		return new_movement;

	if ((walking_angle > 337.5) || (walking_angle <= 22.5))
	{
		new_movement.forward = true;
	}
	else if ((walking_angle > 22.5) && (walking_angle <= 67.5))
	{
		new_movement.forward = true;
		new_movement.left = true;
	}
	else if ((walking_angle > 67.5) && (walking_angle <= 112.5))
	{
		new_movement.left = true;
	}
	else if ((walking_angle > 112.5) && (walking_angle <= 157.5))
	{
		new_movement.left = true;
		new_movement.backward = true;
	}
	else if ((walking_angle > 157.5) && (walking_angle <= 202.5))
	{
		new_movement.backward = true;
	}
	else if ((walking_angle > 202.5) && (walking_angle <= 247.5))
	{
		new_movement.backward = true;
		new_movement.right = true;
	}
	else if ((walking_angle > 247.5) && (walking_angle <= 292.5))
	{
		new_movement.right = true;
	}
	else if ((walking_angle > 292.5) && (walking_angle <= 337.5))
	{
		new_movement.right = true;
		new_movement.forward = true;
	}

	return new_movement;
}

void MovementStrategy::load_nodes(const json& json)
{
	for (const auto& json_node : json["nodes"])
	{
		Vec3D<float> pos;
		int id = json_node["id"];
		pos.x = json_node["x"];
		pos.y = json_node["y"];
		pos.z = json_node["z"];

		m_nodes.push_back(std::make_shared<Node>(id, pos));
	}
}

void MovementStrategy::load_edges(const json& json)
{
	for (const auto& json_edge : json["edges"])
	{
		int from_id = json_edge["from"];
		int to_id = json_edge["to"];
		float weight = json_edge["weight"];

		auto from = get_node_by_id(from_id);
		auto to = get_node_by_id(to_id);

		if (!to)
			continue;

		from->edges.push_back(Node::Edge{ weight, to });
	}
}

std::shared_ptr<Node> MovementStrategy::get_node_by_id(int id) const
{
	for (const auto& node : m_nodes)
	{
		if (node->id == id)
			return node;
	}

	return nullptr;
}

std::shared_ptr<Node> MovementStrategy::get_closest_node_to_position(const Vec3D<float>& position)
{
	std::shared_ptr<Node> closest_node = nullptr;
	float closest_distance = FLT_MAX;

	for (auto& node : m_nodes)
	{
		float distance_to_node = node->position.distance(position);

		if (distance_to_node <= closest_distance)
		{
			closest_distance = distance_to_node;
			closest_node = node;
		}
	}

	return closest_node;
}
