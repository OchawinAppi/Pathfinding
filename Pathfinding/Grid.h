#pragma once

// Standard libraries
#include <vector>

#include "Cell.h"



class Grid
{
private:
	std::vector<std::vector<Cell>> grid;
public:
	// Constructor
	Grid(int x, int y);

	// Destructor not needed. The compiler got this. ;) ( probably )

	// Methods 

	// Displaying - Console
	void print();
	friend std::ostream& operator<<(std::ostream& os, const Grid& grid);

	// Displaying - SFML
	void draw(sf::RenderWindow &window);

	// Getting Specific cell ( will need this later )
	Cell& at(int x, int y);

	bool inBounds(int y, int size);
};

