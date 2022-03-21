#pragma once
#include "Cell.h"


class Grid
{
private:
	std::vector<std::vector<Cell>> grid;
	std::vector<Cell*> searched;
	sf::Vector2f aPos;
	sf::Vector2f bPos;
public:
	// Constructor
	Grid(int x, int y);

	// Destructor not needed. The compiler got this. ;) ( probably )

	// Displaying - Console
	void print();
	friend std::ostream& operator<<(std::ostream& os, const Grid& grid);

	// Displaying - SFML
	void draw(sf::RenderWindow &window);
	void draw(sf::RenderWindow &window, std::vector<Cell*> path, sf::Color color , sf::Shape&& shape, int drawCount, float scale = 1.f);
	void draw(sf::RenderWindow &window, std::vector<Cell*> path, int r, int g, int b, int a, sf::Shape&& shape, int drawCount, float scale = 1.f);


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
	void resetGrid();
	void resetPath();

	std::vector<Cell*> getNeighbors(const sf::Vector2f& pos, bool diag);
	sf::Vector2f &getB();
	sf::Vector2f &getA();

	static inline sf::Vector2f offsetPosition(const sf::Vector2f& position);

	std::vector<Cell*>& getSearched();
	void addSearched(Cell* cell);
	void resetSearched();
};

