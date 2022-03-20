#pragma once

#include <math.h>
#include "Grid.h"
#include <unordered_map>

inline bool contatinsPtr(const std::vector<Cell*>& vec, Cell*& item)
{
	for (int i = 0; i < static_cast<int>(vec.size()); i++)
	{
		//std::cout << vec.at(i)->pos.x << " - " << item->pos.x << " && " << vec.at(i)->pos.y << " - " << item->pos.y << '\n';
		if (vec.at(i)==item)
		{
			return true;
		}
	}
	return false;
}

template <typename T>
inline auto contains(const std::vector<T>& vec, const T &_item)
{
	struct returnInfo
	{
		bool contains;
		int index;
	};

	for (int i = 0 ; i < static_cast<int>(vec.size()); i++)
	{
		if (vec.at(i) == _item) return returnInfo { true, i };
	}
	return returnInfo{ false , -1 };
}


// Note, for simplicity, we covert 1.0 and 1.4 to 10 and 14. We hate decimals.
inline int heuristic(sf::Vector2f &node, sf::Vector2f &target) {
	int x_dist = abs(node.x - target.x);
	int y_dist = abs(node.y - target.y);

	int distance = 0;
	while (x_dist >= 1 && y_dist >= 1) {
		x_dist--;
		y_dist--;
		distance += 14;
	}
	distance += x_dist * 10;
	distance += y_dist * 10;
	return distance;
}


std::vector<Cell*> a_star(Grid& map, sf::Vector2f &start_node, sf::Vector2f &target_node, bool diag)
{
	
	map.at(start_node).updateG(0);
	map.at(start_node).updateH(heuristic(start_node, target_node));
	map.at(start_node).updateF();

	std::vector<Cell*> open_nodes{ &map.at(start_node) };
	std::vector<Cell*> closed_nodes{};

	std::unordered_map<Cell*, Cell*> travelMap;
	std::vector<Cell*> travelPath{};


	Cell* current_node = nullptr;

	bool quit = false;

	while (open_nodes.size() > 0 && !quit)
	{
		current_node = open_nodes.at(0);
		int current_node_index = 0;

		// Step 1: Get Lowest node, it'll be our current node.
		for (size_t i = 1; i < open_nodes.size(); ++i)
		{
			auto* temp_node = open_nodes.at(i);

			// Check for greater f_cost, if there are nodes with same f_cost, check h_cost.
			if (temp_node->F_dist < current_node->F_dist)
			{
				current_node = temp_node;
				current_node_index = i;
			}
			else if (temp_node->F_dist == current_node->F_dist)
			{
				if (temp_node->H_dist < current_node->H_dist)
				{
					current_node = temp_node;
					current_node_index = i;
				}
			}
		}

		// Erase the node from the pool.
		open_nodes.erase(open_nodes.begin() + current_node_index);

		// Add it to the list of nodes we have evaluated.
		closed_nodes.push_back(current_node);

		// Step 2: Evaluate the neighbors of the current node.
		for (auto* neighbor : map.getNeighbors(current_node->pos, diag))
		{
			// If the target node is a neighbor, it's done.  
			quit = (*neighbor == map.at(target_node));

			// Skip neighbors which have been evaluated.
			if (contatinsPtr(closed_nodes, neighbor)) continue;
		
			// new_travel_cost = cost_so_far + cost_from_current_to_neighbor
			const int new_g_cost = current_node->G_dist + heuristic(current_node->pos, neighbor->pos);
			
			// Check if neighbor exists within the pool (not evaluated)
			bool inOpen = contatinsPtr(open_nodes, neighbor);

			// If node DNE in pool, add
			// If node exists and g_cost is cheaper, update.
			if (new_g_cost < neighbor->G_dist || !inOpen)
			{
				neighbor->updateG(new_g_cost);
				neighbor->updateH(heuristic(neighbor->pos, target_node));
				neighbor->updateF();

				if (!inOpen) open_nodes.push_back(neighbor);

				// nodeTravledTo = nodeTraveledFrom, we will have a chain of these in the end.
				travelMap[neighbor] = current_node;
			
			}
		}
	}

	// BACKTRACK
	Cell* next = &map.at(target_node);
	
	// Just checking while the next parent node exists.
	while (travelMap.count(next))
	{
		next = travelMap[next];
		next->isPath = true;
		if (*next != map.at(start_node))
		{
			travelPath.push_back(next);
		}
	}
	return travelPath;
}


