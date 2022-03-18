#include "Grid.h"

// Constructor
Grid::Grid(int x, int y)
{
	x += 2;
	y += 2;
	for (int i = 0; i < static_cast<int>(y); ++i) {
		grid.push_back({});
		for (int j = 0; j < static_cast<int>(x); ++j) {
			if (i == 0 || i == y - 1 || j == 0 || j == x - 1) {
				grid.at(i).emplace_back(0, 0, true, j, i);
			}
			else {
				grid.at(i).emplace_back(0, 0, false, j, i);
			}
		}
	}

	aPos = sf::Vector2f(-1, -1);
	bPos = sf::Vector2f(-1, -1);
	curPos = aPos;

	// For A-Star
	free = {};
	evaluated = {};
}


// Methods 

// Displaying
void Grid::print()
{
	for (int i = 0; i < static_cast<int>(grid.size()); i++) {
		for (int j = 0; j < static_cast<int>(grid.at(0).size()); j++) {
			std::cout << grid.at(i).at(j);

		}
		std::cout << "\n";
	}
}

// Overloading cout op
std::ostream& operator<<(std::ostream& os, const Grid& map)
{
	for (int i = 0; i < static_cast<int>(map.grid.size()); i++) {
		for (int j = 0; j < static_cast<int>(map.grid.at(0).size()); j++) {
			os << map.grid.at(i).at(j);
		}
		os << "\n";
	}
	return os;
}

void Grid::draw(sf::RenderWindow& window)
{
	for (int y = 0; y < static_cast<int>(grid.size()); y++) {
		for (int x = 0; x < static_cast<int>(grid.at(0).size()); x++) {
			grid.at(y).at(x).draw(window);
		}
	}
}

// Getting Specific cell ( will need this later )
Cell& Grid::at(int x, int y)
{
	return grid.at(y).at(x);
}

Cell& Grid::at(const sf::Vector2i& pos)
{
	return at(pos.x, pos.y);
}

Cell& Grid::at(const sf::Vector2f& pos)
{
	return at(static_cast<int>(pos.x), static_cast<int>(pos.y));
}

bool Grid::inBounds(int x, int x_size, int y, int y_size) 
{
	return (x > 0 && x < x_size) && (y > 0 && y < y_size);
}

void Grid::initA(const sf::Vector2f &pos)
{
	aPos = sf::Vector2f(pos);
}

void Grid::initB(const sf::Vector2f& pos)
{
	bPos = sf::Vector2f(pos);
}

void Grid::moveA(const sf::Vector2f &pos)
{
	// Shortcircuting
	if (!at(pos).solid && at(pos).c == ' ')
	{
		if (aPos.x == -1)
		{
			initA(pos);
			return;
		}
		at(aPos).makeEmpty();
		aPos.x = pos.x;
		aPos.y = pos.y;
		at(pos).c = 'A';
	}
}

void Grid::moveB(const sf::Vector2f &pos)
{
	// Shortcircuting
	if (!at(pos).solid && at(pos).c==' ')
	{
		if (bPos.x == -1)
		{
			initB(pos);
			return;
		}
		at(bPos).makeEmpty();
		bPos.x = pos.x;
		bPos.y = pos.y;
		at(pos).c = 'B';
	}
}


std::vector<Cell*> Grid::getNeighbors(const sf::Vector2f &pos)
{
	int x{ static_cast<int>(pos.x) };
	int y{ static_cast<int>(pos.y) };

	std::vector<Cell*> neighbors{};

	for (size_t _x = x - 1; _x < x + 2; _x++)
	{
		for (size_t _y = y - 1; _y < y + 2; _y++)
		{
			if (!(_x == x && _y == y) && this->inBounds(_x, MAP_WIDTH, _y, MAP_HEIGHT))
			{
				if (Cell* cell = &at(_x, _y); !cell->solid)
				{
					neighbors.push_back(cell);
				}
			}
		}
	}
	return neighbors;
}


