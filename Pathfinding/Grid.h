#pragma once

// Standard libraries
#include <vector>

#include "Cell.h"



class Grid
{
private:
	std::vector<std::vector<Cell>> grid;
	sf::Vector2f aPos;
	sf::Vector2f bPos;


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

