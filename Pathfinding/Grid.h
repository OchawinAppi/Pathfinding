#pragma once

// Standard libraries
#include <vector>
#include "Cell.h"




class Grid
{
private:
	std::vector<std::vector<Cell>> grid;
	
	// Start node
	sf::Vector2f aPos;

	// End node
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
	void draw(sf::RenderWindow &window, std::vector<Cell*> path, sf::Color color);
	void draw(sf::RenderWindow &window, std::vector<Cell*> path, int r, int g, int b, int a);
	void drawNodes(sf::RenderWindow &window);

	// Getting Specific cell ( will need this later )
	inline Cell& at(int x, int y);
	Cell& at(const sf::Vector2f& pos);
	Cell& at(const sf::Vector2i& pos);

	bool inBounds(int x, int x_size, int y, int y_size);

	// Points

	void initA(const sf::Vector2f &pos);
	void initB(const sf::Vector2f &pos);
	bool moveA(const sf::Vector2f &pos);
	bool moveB(const sf::Vector2f &pos);
	bool canRoute();
	void resetA();
	void resetB();

	std::vector<Cell*> getNeighbors(const sf::Vector2f& pos, bool diag);

	sf::Vector2f &getB();
	sf::Vector2f &getA();

	inline sf::Vector2f offsetPosition(const sf::Vector2f& position) const
	{
		return sf::Vector2f(
			(position.x * DEFAULT_TILE_SIZE) + (static_cast<float>(WINDOW_HEIGHT) / 2) - (MAP_WIDTH / 2. * DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE,
			(position.y * DEFAULT_TILE_SIZE) + (static_cast<float>(WINDOW_HEIGHT) / 2) - (MAP_WIDTH / 2. * DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE
		);
	
	}
};

