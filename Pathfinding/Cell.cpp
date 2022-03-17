#include "Cell.h"

Cell::Cell() 
{
	G_dist = 0;
	H_dist = 0;
	F_dist = 0;

	pos = sf::Vector2f(-1, -1);
	dir = Direction::NONE;

	c = ' ';
	solid = false;
	isChecked = false;

	// SFML
	tile = sf::RectangleShape{};
}

Cell::Cell(int g, int h, bool _solid, int _x, int _y, Direction _direction) :
	G_dist(g), H_dist(h) 
{
	F_dist = G_dist + H_dist;
	solid = _solid;
	
	c = (solid) ? '#' : ' ';

	pos = sf::Vector2f(
		static_cast<float>(_x), 
		static_cast<float>(_y)
	);
	dir = _direction;



	tile = sf::RectangleShape(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE));

	// Note: Multiply by 
	tile.setPosition(
		(pos.x * DEFAULT_TILE_SIZE) + (WINDOW_HEIGHT / 2) - (MAP_WIDTH / 2) * DEFAULT_TILE_SIZE,
		(pos.y * DEFAULT_TILE_SIZE) + (WINDOW_HEIGHT / 2) - (MAP_HEIGHT / 2) * DEFAULT_TILE_SIZE
	);

	//available = false;
}

void Cell::makeSolid() 
{
	this->solid = true;
	this->c = '#';
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

void Cell::draw(sf::RenderWindow &window)
{
	if (solid)
	{
		tile.setFillColor(sf::Color::White);
	}
	else
	{
		tile.setFillColor(sf::Color::Black);
	}
	window.draw(tile);
}

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
	return os << cell.c;
}
