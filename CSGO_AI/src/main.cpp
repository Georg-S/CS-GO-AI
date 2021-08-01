#pragma once
#include "CSGOAI.h"
#include "MovementStrategy.h"


int main()
{
	/*
	CSGOAi ai{};

	if (ai.init())
		ai.run();
	*/

	MovementStrategy strategy;
	if (strategy.load_in_navmesh("nav_mesh.json")) 
	{
		auto node_from = strategy.get_node_by_id(0);
		auto node_to = strategy.get_node_by_id(1);

		strategy.calculate_new_route(node_from, node_to);
		strategy.print_current_route();
	}


	return 0;
}
