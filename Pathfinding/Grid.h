#pragma once

// Standard libraries
#include <vector>

#include "Cell.h"
//#include "A-Star-Functions.h"



class Grid
{
private:
	std::vector<std::vector<Cell>> grid;
	
	// Start node
	sf::Vector2f aPos;

	// End node
	sf::Vector2f bPos;

	// Current node
	sf::Vector2f curPos;


	// For A-Star
	std::vector<Cell> free;			// Cells not evaluated
	std::vector<Cell> evaluated;    // Cells evaluated

	
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
	Cell& at(sf::Vector2f pos);
	Cell& at(sf::Vector2i pos);

	bool inBounds(int y, int size);

	// Points

	void initA(sf::Vector2f pos);
	void initB(sf::Vector2f pos);
	void moveA(sf::Vector2f pos);
	void moveB(sf::Vector2f pos);

};

