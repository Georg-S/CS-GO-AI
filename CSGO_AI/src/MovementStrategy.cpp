#include "MovementStrategy.h"

void MovementStrategy::update(GameInformationhandler* game_info_handler)
{
	GameInformation game_info = game_info_handler->get_game_information();

	if (!game_info.closest_enemy_player)
		return;
	auto current_player_node = get_closest_node_to_position(game_info.controlled_player.head_position);
	auto current_closest_enemy_node = get_closest_node_to_position(game_info.closest_enemy_player->head_position);

	if (!current_player_node || !current_closest_enemy_node)
		return;

	if (current_closest_enemy_node != closest_enemy_locked_node)
		current_route = calculate_new_route(current_player_node, current_closest_enemy_node);
	if (current_player_node != player_locked_node) 
	{
		if((current_route.size() > 0) && (current_route[0] != current_player_node))
			current_route = calculate_new_route(current_player_node, current_closest_enemy_node);
	}
}

bool MovementStrategy::load_in_navmesh(const std::string& filename)
{
	try
	{
		std::ifstream ifs(filename);

		navmesh_json = json::parse(ifs);
		ifs.close();

		load_nodes(navmesh_json);
		load_edges(navmesh_json);
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << std::endl;
		return false;
	}
	return true;
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

		nodes.push_back(std::make_shared<Node>(id, pos));
	}
}

void MovementStrategy::load_edges(const json& json)
{
	for (const auto& json_edge : json["edges"])
	{
		int from_id = json_edge["from"];
		int to_id = json_edge["to"];
		float weight = json_edge["distance"];

		auto from = get_node_by_id(from_id);
		auto to = get_node_by_id(to_id);

		if (!to)
			continue;
		
		from->edges.push_back(Node::Edge{weight, to});
	}
}

std::shared_ptr<Node> MovementStrategy::get_node_by_id(int id) const
{
	for (const auto node : this->nodes) 
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

	for (auto node : nodes) 
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

std::vector<std::shared_ptr<Node>> MovementStrategy::calculate_new_route(std::shared_ptr<Node> from, std::shared_ptr<Node> to)
{
	auto closed_list = dijkstra_algorithm(from);
	return get_route(closed_list, to);
}

std::vector<DijkstraListentry> MovementStrategy::dijkstra_algorithm(std::shared_ptr<Node> from)
{
	auto is_inside_list = [](const std::vector<DijkstraListentry>& list, const std::shared_ptr<Node> node)
	{
		for (const auto& list_element : list)
		{
			if (list_element.node == node)
				return true;
		}

		return false;
	};

	auto get_weight_of_node_in_list = [](const std::vector<DijkstraListentry>& list, const std::shared_ptr<Node> node)
	{
		for (const auto& list_element : list)
		{
			if (list_element.node == node)
				return list_element.weight;
		}
		return 0.f;
	};

	auto update_list_entry = [](std::vector<DijkstraListentry>& list, const DijkstraListentry& update)
	{
		for (auto& list_element : list)
		{
			if (list_element.node == update.node)
			{
				list_element.previous_node = update.previous_node;
				list_element.weight = update.weight;
			}
		}
	};

	auto remove_list_element = [](std::vector<DijkstraListentry>& list, const std::shared_ptr<Node> node)
	{
		DijkstraListentry return_entry;

		for (int i = 0; i < list.size(); i++)
		{
			if (list[i].node == node)
			{
				return_entry = list[i];
				list.erase(list.begin() + i);
				return return_entry;
			}
		}
		return return_entry;
	};

	std::vector<DijkstraListentry> open_list;
	std::vector<DijkstraListentry> closed_list;

	auto current_node = from;
	float current_weight = 0;

	open_list.push_back(DijkstraListentry{ current_node, nullptr, current_weight });

	while (open_list.size() > 0)
	{
		current_node = open_list[0].node;
		current_weight = open_list[0].weight;

		for (const auto& edge : current_node->edges)
		{
			if (is_inside_list(closed_list, edge.toNode))
				continue;

			const float new_weight = current_weight + edge.weight;

			if (!is_inside_list(open_list, edge.toNode))
			{
				open_list.push_back(DijkstraListentry{ edge.toNode, current_node, new_weight });
			}
			else
			{
				if (new_weight < get_weight_of_node_in_list(open_list, edge.toNode))
					update_list_entry(open_list, DijkstraListentry{ edge.toNode, current_node, new_weight });
			}
		}
		DijkstraListentry element = remove_list_element(open_list, current_node);
		closed_list.push_back(element);
		std::sort(open_list.begin(), open_list.end());
	}

	return closed_list;
}

std::vector<std::shared_ptr<Node>> MovementStrategy::get_route(const std::vector<DijkstraListentry>& closed_list, const std::shared_ptr<Node> to_node)
{
	return std::vector<std::shared_ptr<Node>>();
}
