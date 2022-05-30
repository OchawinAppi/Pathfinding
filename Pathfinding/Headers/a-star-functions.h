#pragma once

#include "grid.h"
#include <unordered_map>
#include "utils.h"

inline bool contatinsPtr(const std::vector<Cell*>& vec, Cell*& item)
{
	return std::any_of(vec.begin(), vec.end(), [&](const auto& cell) { return cell == item; });
}

// Check if item exists within the vector
template <typename T>
auto contains(const std::vector<T>& vec, const T& item)
{
	struct returnInfo
	{
		bool contains;
		int  index;
	};

	for (int i = 0; i < static_cast<int>(vec.size()); i++)
	{
		if (&vec.at(i) == &item) return returnInfo{ true, i };
	}
	return returnInfo{ false, -1 };
}

inline auto indiscriminateSearch(const Grid&         map,
                                 const sf::Vector2f& startNode1,
                                 const char          targetLetter1,
                                 const sf::Vector2f& startNode2,
                                 const char          targetLetter2,
                                 const bool          diag)
{
	struct ReturnInfo
	{
		Cell*              node;
		char               target;
		std::vector<Cell*> room;
	};

	std::vector collectedNodes1{ &map.at(startNode1) };
	std::vector collectedNodes2{ &map.at(startNode2) };

	int count = 0;
	while (static_cast<int>(collectedNodes1.size()) > count && collectedNodes2.size() > static_cast<unsigned>(count))
	{
		const Cell* current_node1 = collectedNodes1.at(count);
		const Cell* current_node2 = collectedNodes2.at(count);

		if (current_node1->c == targetLetter1)
			return ReturnInfo{
				&map.at(startNode1), targetLetter1, collectedNodes1
			};
		if (current_node2->c == targetLetter2)
			return ReturnInfo{
				&map.at(startNode2), targetLetter2, collectedNodes2
			};


		// First one
		const auto& neighbors1 = map.getNeighbors(current_node1->pos, diag);

		if (neighbors1.size() == static_cast<unsigned>(0))
			return ReturnInfo{
				&map.at(startNode1), targetLetter1, collectedNodes1
			};

		// Second one
		const auto& neighbors2 = map.getNeighbors(current_node2->pos, diag);

		if (neighbors2.size() == static_cast<unsigned>(0))
			return ReturnInfo{
				&map.at(startNode2), targetLetter2, collectedNodes2
			};

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

	if (collectedNodes1.size() <= static_cast<unsigned>(count))
		return ReturnInfo{ &map.at(startNode1), targetLetter1, collectedNodes1 };
	return ReturnInfo{ &map.at(startNode2), targetLetter2, collectedNodes2 };
}


// Note, for simplicity, we covert 1.0 and 1.4 to 10 and 14. We hate decimals.
inline int heuristic(const sf::Vector2f& node, const sf::Vector2f& target)
{
	int x_dist = static_cast<int>(abs(node.x - target.x));
	int y_dist = static_cast<int>(abs(node.y - target.y));

	int distance = 0;
	while (x_dist >= 1 && y_dist >= 1)
	{
		x_dist--;
		y_dist--;
		distance += 14;
	}
	distance += x_dist * 10;
	distance += y_dist * 10;
	return distance;
}

inline std::vector<Cell*> aStar(Grid&      map, const sf::Vector2f& startNode, const sf::Vector2f& targetNode,
                                const bool diag)
{
	map.resetSearched();
	map.at(startNode).updateG(0);
	map.at(startNode).updateH(heuristic(startNode, targetNode));
	map.at(startNode).updateF();

	std::vector        open_nodes{ &map.at(startNode) };
	std::vector<Cell*> closed_nodes{};

	std::unordered_map<Cell*, Cell*> travel_map;
	std::vector<Cell*>               travel_path{};

	Cell* current_node = nullptr;

	bool quit = false;

	while (!open_nodes.empty() && !quit)
	{
		current_node = open_nodes.at(0);
		int current_node_index = 0;

		// Step 1: Get Lowest node, it'll be our current node.
		for (size_t i = 1; i < open_nodes.size(); ++i)
		{
			auto* temp_node = open_nodes.at(i);

			// Check for greater f_cost, if there are nodes with same f_cost, check h_cost.
			if (temp_node->f_dist < current_node->f_dist)
			{
				current_node = temp_node;
				current_node_index = static_cast<int>(i);
			}
			else if (temp_node->f_dist == current_node->f_dist)
			{
				if (temp_node->h_dist < current_node->h_dist)
				{
					current_node = temp_node;
					current_node_index = static_cast<int>(i);
				}
			}
		}
		map.addSearched(current_node);

		if (current_node->pos == targetNode)
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
			quit = *neighbor == map.at(targetNode);

			// Skip neighbors which have been evaluated.
			if (contatinsPtr(closed_nodes, neighbor)) continue;

			// new_travel_cost = cost_so_far + cost_from_current_to_neighbor
			const int new_g_cost = current_node->g_dist + heuristic(current_node->pos, neighbor->pos);

			// Check if neighbor exists within the pool (not evaluated)
			const bool inOpen = contatinsPtr(open_nodes, neighbor);

			// If node DNE in pool, add
			// If node exists and g_cost is cheaper, update.
			if (new_g_cost < neighbor->g_dist || !inOpen)
			{
				neighbor->updateG(new_g_cost);
				neighbor->updateH(heuristic(neighbor->pos, targetNode));
				neighbor->updateF();

				if (!inOpen) open_nodes.push_back(neighbor);

				// nodeTravledTo = nodeTraveledFrom, we will have a chain of these in the end.
				travel_map[neighbor] = current_node;
			}
			//log(neighbor->pos.x, neighbor->pos.y, neighbor->F_dist, neighbor->G_dist);
		}
	}

	// BACKTRACK
	Cell* next = &map.at(targetNode);

	// Just checking while the next parent node exists.
	while (travel_map.count(next))
	{
		next = travel_map[next];
		next->is_path = true;
		if (*next != map.at(startNode))
		{
			travel_path.push_back(next);
		}
	}
	return travel_path;
}
