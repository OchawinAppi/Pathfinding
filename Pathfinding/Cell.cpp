#include "Cell.h"

Cell::Cell() 
{
	G_dist = 0;
	H_dist = 0;
	F_dist = 0;

	pos = Position(-1, -1);
	dir = Direction::NONE;

	c = ' ';
	solid = false;
	isChecked = false;
}

Cell::Cell(int g, int h, bool _solid, int _x, int _y, Direction _direction) :
	G_dist(g), H_dist(h) 
{
	F_dist = G_dist + H_dist;
	solid = _solid;
	c = ' ';
	pos = Position(_x, _y);
	dir = _direction;
	//available = false;
}

void Cell::makeSolid() 
{
	this->solid = true;
}

void Cell::updateG(int n) 
{
	G_dist = n;
}

void Cell::updateH(int n) 
{
	H_dist = n;
}

void Cell::updateF() 
{
	F_dist = H_dist + G_dist;
}

bool& Cell::getChecked()
{
	return isChecked;
}

void Cell::checked() 
{
	this->isChecked = true;
}
