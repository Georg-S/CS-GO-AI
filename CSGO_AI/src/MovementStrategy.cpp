#include "MovementStrategy.h"

void MovementStrategy::update(GameInformationhandler* game_info_handler)
{
	GameInformation game_info = game_info_handler->get_game_information();

	auto player_pos = get_closest_node_to_position(game_info.controlled_player.head_position);
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
