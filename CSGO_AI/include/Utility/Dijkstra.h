#pragma once
#include <memory>
#include <algorithm>
#include <vector>
#include "Logging.h"
#include "Vec3D.h"

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

class Dijkstra
{
public:
	static std::vector<std::shared_ptr<Node>> get_route(std::shared_ptr<Node> from, std::shared_ptr<Node> to);
	static std::vector<std::shared_ptr<DijkstraListentry>> run_algorithm(std::shared_ptr<Node> from);
	static void log_route(const std::vector<std::shared_ptr<Node>>& route);

private:
	static bool is_inside_list(const std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node);
	static float get_weight_of_node_in_list(const std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node);
	static void update_list_entry(std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<DijkstraListentry>& update);
	static std::shared_ptr<DijkstraListentry> remove_list_element(std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node);
	static std::vector<std::shared_ptr<Node>> calculate_route_from_closed_list(
		const std::vector<std::shared_ptr<DijkstraListentry>>& closed_list, const std::shared_ptr<Node> to_node);
};
