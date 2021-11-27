#pragma once
#include <fstream>
#include <iostream>
#include <memory>
#include <Windows.h>
#include "Utility/Utility.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Utility/Vec3D.h"
#include "Utility/json.hpp"
#include "GameInformationHandler.h"
#include "Utility/Logging.h"
#include "Utility/Dijkstra.h"

using nlohmann::json;

class MovementStrategy 
{
public:
	void update(GameInformationhandler* game_info_handler);
	void handle_navmesh_load(const std::string& map_name);
	bool load_in_navmesh(const std::string& filename);
	void set_debug_print_route(bool value);
	void reset_loaded_navmesh();
	bool is_valid_navmesh_loaded() const;

private:
	std::shared_ptr<Node> get_node_by_id(int id) const;
	Movement calculate_move_info(const GameInformation& game_info, const std::shared_ptr<Node> node);
	float calc_angle_between_two_positions(const Vec3D<float>& pos1, const Vec3D<float>& pos2) const;
	float calc_walk_angle(float view_angle, float position_angle) const;
	Movement get_movement_from_walking_angle(float walking_angle) const;
	void load_nodes(const json& json);
	void load_edges(const json& json);
	std::shared_ptr<Node> get_closest_node_to_position(const Vec3D<float>& position);

	json navmesh_json;
	std::vector<std::shared_ptr<Node>> nodes;
	std::shared_ptr<Node> next_node = nullptr;
	std::vector<std::shared_ptr<Node>> current_route;
	std::string loaded_map;
	bool valid_navmesh_loaded;
	long long delay_time = 0;
	bool debug_print_route = false;
};