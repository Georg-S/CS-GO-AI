#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include "Vec3D.h"
#include "json.hpp"
#include "GameInformationHandler.h"

using nlohmann::json;

struct Node
{
	struct Edge
	{
		float weight;
		std::shared_ptr<Node> toNode;
	};

	Node(int id, Vec3D<float> position) 
	{
		this->id = id;
		this->position = position;
	}

	int id;
	Vec3D<float> position;
	std::vector<Edge> edges;
};

struct DijkstraListentry
{
	std::shared_ptr<Node> node;
	std::shared_ptr<Node> previous_node;
	float weight = 0.0;

	bool operator<(const DijkstraListentry& a) const
	{
		return weight < a.weight;
	}
};

class MovementStrategy 
{
public:
	void update(GameInformationhandler* game_info_handler);
	bool load_in_navmesh(const std::string& filename);
	std::shared_ptr<Node> get_node_by_id(int id) const;
	std::vector<std::shared_ptr<Node>> calculate_new_route(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	void print_current_route() const;

private:
	void load_nodes(const json& json);
	void load_edges(const json& json);
	std::shared_ptr<Node> get_closest_node_to_position(const Vec3D<float>& position);
	std::vector<DijkstraListentry> dijkstra_algorithm(std::shared_ptr<Node> from);
	std::vector<std::shared_ptr<Node>> get_route(const std::vector<DijkstraListentry>& closed_list, const std::shared_ptr<Node> to_node);

	json navmesh_json;
	std::vector<std::shared_ptr<Node>> nodes;
	std::shared_ptr<Node> player_locked_node = nullptr;
	std::shared_ptr<Node> closest_enemy_locked_node = nullptr;
	std::vector<std::shared_ptr<Node>> current_route;
};