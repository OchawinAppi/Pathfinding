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
	Cell& at(const sf::Vector2f& pos);
	Cell& at(const sf::Vector2i& pos);

	bool inBounds(int x, int x_size, int y, int y_size);

	// Points

	void initA(const sf::Vector2f &pos);
	void initB(const sf::Vector2f &pos);
	void moveA(const sf::Vector2f &pos);
	void moveB(const sf::Vector2f &pos);

	std::vector<Cell*> getNeighbors(const sf::Vector2f& pos);


};

