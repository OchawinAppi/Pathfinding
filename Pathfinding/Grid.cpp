#include "Grid.h"

// Constructor
Grid::Grid(int x, int y)
{
	x += 2;
	y += 2;
	for (int i = 0; i < y; i++) {
		grid.push_back({});
		for (int j = 0; j < x; j++) {
			if (i == 0 || i == y - 1 || j == 0 || j == x - 1) {
				grid.at(i).emplace_back(0, 0, true, j, i);
			}
			else {
				grid.at(i).emplace_back(0, 0, false, j, i);
			}

		}
	}
}


// Methods 

// Displaying
void Grid::print()
{
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid.at(0).size(); j++) {
			std::cout << grid.at(i).at(j);

		}
		std::cout << "\n";
	}
}

// Overloading cout op
std::ostream& operator<<(std::ostream& os, const Grid& map)
{
	for (int i = 0; i < map.grid.size(); i++) {
		for (int j = 0; j < map.grid.at(0).size(); j++) {
			os << map.grid.at(i).at(j);
		}
		os << "\n";
	}
	return os;
}

// Getting Specific cell ( will need this later )
Cell& Grid::at(int x, int y)
{
	return grid.at(y).at(x);
}



