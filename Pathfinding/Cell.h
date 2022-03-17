#pragma once
#include "Position.h"
#include "Directions.h"


class Cell {
public:
	int G_dist;
	int H_dist;
	int F_dist;

	Position pos;
	Direction dir;

	bool solid;
	char c;
	bool isChecked = false;

	// Constructors
	Cell();
	Cell(int g, int h, bool _solid, int _x, int _y, Direction _direction = Direction::NONE);
	
	// Methods
	void makeSolid();
	void updateG(int n);
	void updateH(int n);
	void updateF();
	bool& getChecked();
	void checked();

};

