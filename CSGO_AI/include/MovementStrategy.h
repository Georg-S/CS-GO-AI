#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <Windows.h>
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>
#include "Vec3D.h"
#include "json.hpp"
#include "GameInformationHandler.h"
#include "Logger.h"

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
	DijkstraListentry(std::shared_ptr<Node> node, std::shared_ptr<Node> previous_node, std::shared_ptr<DijkstraListentry> previous_node_list_entry, float weight)
	{
		this->node = node;
		this->previous_node = previous_node;
		this->previous_node_list_entry = previous_node_list_entry;
		this->weight = weight;
	}

	std::shared_ptr<Node> node;
	std::shared_ptr<Node> previous_node;
	std::shared_ptr<DijkstraListentry> previous_node_list_entry;
	float weight = 0.f;

	bool operator<(const DijkstraListentry& a) const
	{
		return weight < a.weight;
	}
};

class MovementStrategy 
{
public:
	MovementStrategy(std::shared_ptr<Logger> logger);
	void update(GameInformationhandler* game_info_handler);
	bool load_in_navmesh(const std::string& filename);
	void set_debug_print_route(bool value);

private:
	std::shared_ptr<Node> get_node_by_id(int id) const;
	std::vector<std::shared_ptr<Node>> calculate_new_route(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	void console_print_current_route() const;
	Movement calculate_move_info(const GameInformation& game_info, const std::shared_ptr<Node> node);
	float calc_angle_between_two_positions(const Vec3D<float>& pos1, const Vec3D<float>& pos2) const;
	float calc_walk_angle(float view_angle, float position_angle) const;
	Movement get_movement_from_walking_angle(float walking_angle) const;
	void load_nodes(const json& json);
	void load_edges(const json& json);
	std::shared_ptr<Node> get_closest_node_to_position(const Vec3D<float>& position);
	std::vector<std::shared_ptr<DijkstraListentry>> dijkstra_algorithm(std::shared_ptr<Node> from);
	std::vector<std::shared_ptr<Node>> get_route(const std::vector<std::shared_ptr<DijkstraListentry>>& closed_list, const std::shared_ptr<Node> to_node);

	json navmesh_json;
	std::vector<std::shared_ptr<Node>> nodes;
	std::shared_ptr<Node> next_node = nullptr;
	std::vector<std::shared_ptr<Node>> current_route;
	std::vector<DWORD> movement_keys;
	std::shared_ptr<Logger> logger = nullptr;
	long long delay_time = 0;

	bool debug_print_route = false;
	const DWORD w_key_code = 0x57;
	const DWORD s_key_code = 0x53;
	const DWORD probably_a_key_code = 0x41;
	const DWORD d_key_code = 0x44;
	const DWORD KEYEVENT_KEYUP = 0x02;
	const DWORD KEYEVENT_KEYDOWN = 0x0;
};