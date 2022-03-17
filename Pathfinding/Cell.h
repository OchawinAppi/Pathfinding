#pragma once
#include "Position.h"
#include "Directions.h"
#include <SFML/Graphics.hpp>


class Cell {
public:
	// A-Star attributes
	int G_dist;
	int H_dist;
	int F_dist;

	Position pos;
	Direction dir; // <- direction leading to parent cell ( for forming path later )

	bool solid; // <- Won't evaluate solid cells ( walls )
	char c; // <- for printing on console ( pre-sfml )
	bool isChecked = false; // <- for A-Star

	// Constructors
	Cell();
	Cell(int g, int h, bool _solid, int _x, int _y, Direction _direction = Direction::NONE);
	// Destructor not needed. The compiler got this. ;) ( probably )
	
	// Methods
	void makeSolid();
	void updateG(int n);
	void updateH(int n);
	void updateF();
	bool& getChecked();
	void checked();
	void draw(); // <- SFML

	friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

};

