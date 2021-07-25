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

class MovementStrategy 
{
public:
	void update(GameInformationhandler* game_info_handler);
	bool load_in_navmesh(const std::string& filename);

private:
	void load_nodes(const json& json);
	void load_edges(const json& json);
	std::shared_ptr<Node> get_node_by_id(int id) const;
	std::shared_ptr<Node> get_closest_node_to_position(const Vec3D<float>& position);

	json navmesh_json;
	std::vector<std::shared_ptr<Node>> nodes;
};