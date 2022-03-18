#pragma once

#include <math.h>
#include "Grid.h"

template <typename T>
auto contains(std::vector<T>& vec, const T &_item)
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
int heuristic(sf::Vector2f node, sf::Vector2f target) {
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


std::vector<sf::Vector2f> a_star(Grid& map, sf::Vector2f start, sf::Vector2f target)
{
	sf::Vector2f curPos{start};			// current node

	std::vector<Cell> toEval{};			// Cells not evaluated
	std::vector<Cell> evaluated{};    // Cells evaluated
	std::vector<Cell> free{};

	std::vector<sf::Vector2f> travelPath{};

	int count = 0; // Safety precaution
	while (count < 10 && curPos != target)
	{
		for (const auto& neighbor : map.getNeighbors(curPos))
		{
			neighbor->updateH(heuristic(neighbor->pos, target));
			neighbor->updateG(map.at(curPos).G_dist + heuristic(neighbor->pos, start));
			neighbor->updateF();
			toEval.push_back(*neighbor);
		}

		for (const auto& cell : toEval)
		{
			if (auto [hasCell, index] = contains(free, cell); hasCell)
			{
				if (cell.F_dist < free.at(index).F_dist)
				free.at(index).H_dist = cell.H_dist;
				free.at(index).G_dist = cell.G_dist;
			}
			else
			{
				free.push_back(cell);
			}
		}


		if (!free.empty())
		{
			std::sort(free.begin(), free.end(), [](Cell& val1, Cell& val2)
				{ 
					return (val1.F_dist != val2.F_dist) ? (val1.F_dist < val2.F_dist) : (val1.G_dist < val2.G_dist);
				}
			);

			Cell lowest = free.at(0);
			travelPath.push_back(lowest.pos);
		}
		count++;
	}
	return travelPath;
}


