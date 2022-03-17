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

	// Displaying
	void print();
	friend std::ostream& operator<<(std::ostream& os, const Grid& grid);

	// Getting Specific cell ( will need this later )
	Cell& at(int x, int y);
};

