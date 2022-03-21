#pragma once

#include <math.h>
#include "Grid.h"
#include <unordered_map>
#include "utils.h"

inline bool contatinsPtr(const std::vector<Cell*>& vec, Cell*& item)
{
	for (int i = 0; i < static_cast<int>(vec.size()); i++)
	{
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

auto indiscriminateSearch(Grid& map, sf::Vector2f& start_node1, char target_letter1, sf::Vector2f& start_node2, char target_letter2, bool diag)
{
	struct returnInfo
	{
		Cell* node;
		char target;
		std::vector<Cell*> room;
	};

	std::vector<Cell*> collectedNodes1{ &map.at(start_node1) };
	std::vector<Cell*> collectedNodes2{ &map.at(start_node2) };

	Cell* current_node1 = nullptr;
	Cell* current_node2 = nullptr;

	int count = 0;
	while (collectedNodes1.size() > count && collectedNodes2.size()>count)
	{
		current_node1 = collectedNodes1.at(count);
		current_node2 = collectedNodes2.at(count);

		if (current_node1->c == target_letter1) return returnInfo{ &map.at(start_node1), target_letter1, collectedNodes1 };
		if (current_node2->c == target_letter2) return returnInfo{ &map.at(start_node2), target_letter2, collectedNodes2 };


		// First one
		const auto& neighbors1 = map.getNeighbors(current_node1->pos, diag);
		if (neighbors1.size() == 0) returnInfo{ &map.at(start_node1), target_letter1, collectedNodes1 };
		
		// Second one
		const auto& neighbors2 = map.getNeighbors(current_node2->pos, diag);
		if (neighbors2.size() == 0) returnInfo{ &map.at(start_node2), target_letter2, collectedNodes2 };

		for (auto* neighbor : neighbors1)
		{
			if (contatinsPtr(collectedNodes1, neighbor)) continue;
			collectedNodes1.push_back(neighbor);
		}

		for (auto* neighbor : neighbors2)
		{
			if (contatinsPtr(collectedNodes2, neighbor)) continue;
			collectedNodes2.push_back(neighbor);
		}
		++count;
	}

	if (collectedNodes1.size() <= count)
		return returnInfo{ &map.at(start_node1), target_letter1, collectedNodes1 };
	else
		return returnInfo{ &map.at(start_node2), target_letter2, collectedNodes2 };

}

auto indiscriminateSearch(Grid& map, sf::Vector2f& start_node, char target_letter, bool diag)
{
	log("SEARCHING", target_letter);
	struct returnInfo
	{
		bool blocked;
		int size;
	};

	std::vector<Cell*> collectedNodesFromStart{ &map.at(start_node) };

	Cell* current_node = nullptr;
	int count = 0;
	while (collectedNodesFromStart.size() > count)
	{
		current_node = collectedNodesFromStart.at(count);
		log(current_node->c, target_letter);
		if (current_node->c == target_letter) return returnInfo{ false, count };

		const auto& neighbors = map.getNeighbors(current_node->pos, diag);

		if (neighbors.size() == 0)
		{
			returnInfo{ true, count };
		}

		for (auto* neighbor : neighbors)
		{
			if (contatinsPtr(collectedNodesFromStart, neighbor)) continue;
			collectedNodesFromStart.push_back(neighbor);
		}
		++count;
	}
	log("DONE");
	return returnInfo{ true, count };
	
}


// Note, for simplicity, we covert 1.0 and 1.4 to 10 and 14. We hate decimals.
inline int heuristic(sf::Vector2f &node, sf::Vector2f &target) {
	int x_dist = static_cast<int>(abs(node.x - target.x));
	int y_dist = static_cast<int>(abs(node.y - target.y));

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
	map.resetSearched();
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
		map.addSearched(current_node);
	
		if ((current_node->pos.x == target_node.x) && (current_node->pos.y == target_node.y))
		{
			break;
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
			//log(neighbor->pos.x, neighbor->pos.y, neighbor->F_dist, neighbor->G_dist);
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


