#include "Utility/Dijkstra.h"

std::vector<std::shared_ptr<Node>> Dijkstra::get_route(std::shared_ptr<Node> from, std::shared_ptr<Node> to)
{
	auto closed_list = Dijkstra::run_algorithm(from);

	return calculate_route_from_closed_list(closed_list, to);
}

std::vector<std::shared_ptr<DijkstraListentry>> Dijkstra::run_algorithm(std::shared_ptr<Node> from)
{
	auto compare_weight = [](std::shared_ptr<DijkstraListentry> a, std::shared_ptr<DijkstraListentry> b) -> bool
	{
		return a->weight < b->weight;
	};

	std::vector<std::shared_ptr<DijkstraListentry>> open_list;
	std::vector<std::shared_ptr<DijkstraListentry>> closed_list;

	std::shared_ptr<Node> current_node = from;
	float current_weight = 0;
	auto current_list_entry = std::make_shared<DijkstraListentry>(current_node, nullptr, nullptr, current_weight);

	open_list.push_back(current_list_entry);

	while (open_list.size() > 0)
	{
		current_list_entry = open_list[0];
		current_node = open_list[0]->node;
		current_weight = open_list[0]->weight;

		for (const auto& edge : current_node->edges)
		{
			if (is_inside_list(closed_list, edge.toNode))
				continue;

			const float new_weight = current_weight + edge.weight;

			if (!is_inside_list(open_list, edge.toNode))
				open_list.push_back(std::make_shared<DijkstraListentry>(edge.toNode, current_node, current_list_entry, new_weight));
			else if (new_weight < get_weight_of_node_in_list(open_list, edge.toNode))
				update_list_entry(open_list, std::make_shared<DijkstraListentry>(edge.toNode, current_node, current_list_entry, new_weight));
		}
		std::shared_ptr<DijkstraListentry> element = remove_list_element(open_list, current_node);
		if (element)
			closed_list.push_back(element);
		std::sort(open_list.begin(), open_list.end(), compare_weight);
	}

	return closed_list;
}

void Dijkstra::log_route(const std::vector<std::shared_ptr<Node>>& route)
{
	std::string route_str = "";
	for (unsigned int i = 0; i < route.size(); i++)
	{
		route_str += std::to_string(route[i]->id);

		if (i < route.size() - 1)
			route_str += " -> ";
	}

	Logging::log("Current route: " + route_str);
}

bool Dijkstra::is_inside_list(const std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node)
{
	for (const auto& list_element : list)
	{
		if (list_element->node == node)
			return true;
	}

	return false;
}

float Dijkstra::get_weight_of_node_in_list(const std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node)
{
	for (const auto& list_element : list)
	{
		if (list_element->node == node)
			return list_element->weight;
	}
	return 0.f;
}

void Dijkstra::update_list_entry(std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<DijkstraListentry>& update)
{
	for (auto& list_element : list)
	{
		if (list_element->node == update->node)
			list_element = update;
	}
}

std::shared_ptr<DijkstraListentry> Dijkstra::remove_list_element(std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node)
{
	std::shared_ptr<DijkstraListentry> return_entry = nullptr;

	for (unsigned int i = 0; i < list.size(); i++)
	{
		if (list[i]->node == node)
		{
			return_entry = list[i];
			list.erase(list.begin() + i);
			return return_entry;
		}
	}
	return return_entry;
}

std::vector<std::shared_ptr<Node>> Dijkstra::calculate_route_from_closed_list(const std::vector<std::shared_ptr<DijkstraListentry>>& closed_list, const std::shared_ptr<Node> to_node)
{
	std::vector<std::shared_ptr<Node>> result;

	auto get_list_entry_by_node = [](const std::vector<std::shared_ptr<DijkstraListentry>>& list, const std::shared_ptr<Node> node)
	{
		std::shared_ptr<DijkstraListentry> entry;

		for (unsigned int i = 0; i < list.size(); i++)
		{
			if (node == list[i]->node)
				return list[i];
		}

		return entry;
	};

	std::shared_ptr<DijkstraListentry> entry = get_list_entry_by_node(closed_list, to_node);

	if (!entry || !entry->node)
		return result;

	result.insert(result.begin(), entry->node);

	while (entry->previous_node_list_entry)
	{
		entry = entry->previous_node_list_entry;

		if (entry->node)
			result.insert(result.begin(), entry->node);
	}

	return result;
}
