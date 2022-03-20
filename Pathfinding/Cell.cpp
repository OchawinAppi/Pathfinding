#include "Cell.h"

Cell::Cell() 
{
	G_dist = -1;
	H_dist = -1;
	F_dist = -1;

	pos = sf::Vector2f(-1, -1);
	dir = Direction::NONE;

	c = ' ';
	solid = false;
	isChecked = false;

	// SFML
	tile = sf::RectangleShape{};
}

Cell::Cell(int g, int h, bool _solid, int _x, int _y, Direction _direction) :
	G_dist(g), H_dist(h), solid(_solid)
{
	F_dist = G_dist + H_dist;
	
	c = (solid) ? '#' : ' ';

	pos = sf::Vector2f(
		static_cast<float>(_x), 
		static_cast<float>(_y)
	);
	dir = _direction;

	tile = sf::RectangleShape(sf::Vector2f(DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE));

	// Note: Multiply by 
	tile.setPosition(
		(pos.x * DEFAULT_TILE_SIZE) + ((float)WINDOW_WIDTH / 2) - (MAP_WIDTH/2.*DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE,
		(pos.y * DEFAULT_TILE_SIZE) + ((float)WINDOW_HEIGHT / 2) - (MAP_HEIGHT / 2. * DEFAULT_TILE_SIZE) - DEFAULT_TILE_SIZE
	);
}

void Cell::makeSolid() 
{
	this->solid = true;
	this->c = '#';
}

void Cell::makeEmpty()
{
	this->solid = false;
	this->c = ' ';
}

void Cell::makePath()
{
	c = '.';
}

void Cell::updateG(int n) 
{
	this->G_dist = n;
}

void Cell::updateH(int n) 
{
	this->H_dist = n;
}

void Cell::updateF() 
{
	this->F_dist = this->H_dist + this->G_dist;
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
		switch (this->c)
		{
		case 'A': tile.setFillColor(sf::Color::Red); break;
		case 'B': tile.setFillColor(sf::Color::Blue); break;
		case '.': tile.setFillColor(sf::Color::Yellow); break;
		default: tile.setFillColor(sf::Color::Black);
		}
	}

	window.draw(tile);

}

void Cell::setColor(sf::Color color)
{
	this->tile.setFillColor(color);
}

bool Cell::operator==(const Cell& other) const
{
	return this->pos.x == other.pos.x && this->pos.y == other.pos.y;
}

bool Cell::operator!=(const Cell& other) const
{
	return this->pos.x != other.pos.x || this->pos.y != other.pos.y;;
}

void Cell::placeLetter(char _c)
{
	this->c = _c;
}

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
	return os << cell.c;
}

