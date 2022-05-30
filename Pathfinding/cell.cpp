#include "Headers/cell.h"

Cell::Cell()
	: g_dist(-1)
	, h_dist(-1)
	, f_dist(-1)
	, is_path(false)
	, pos(-1.f, -1.f)
	, solid(false)
	, c(' ')
	, tile({})
{
}

Cell::Cell(int g, int h, bool solid, int x, int y)
	: g_dist(g)
	, h_dist(h)
	, f_dist(g_dist + h_dist)
	, is_path(false)
	, pos(static_cast<float>(x), static_cast<float>(y))
	, solid(solid)
	, c(solid ? '#' : ' ')
	, tile({ DEFAULT_TILE_SIZE, DEFAULT_TILE_SIZE })
{
	// Note: Multiply by 
	tile.setPosition(
		pos.x * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_WIDTH) / 2 - MAP_WIDTH / 2.f * DEFAULT_TILE_SIZE -
		DEFAULT_TILE_SIZE,
		pos.y * DEFAULT_TILE_SIZE + static_cast<float>(WINDOW_HEIGHT) / 2 - MAP_HEIGHT / 2.f * DEFAULT_TILE_SIZE -
		DEFAULT_TILE_SIZE
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

void Cell::updateG(const int n)
{
	this->g_dist = n;
}

void Cell::updateH(const int n)
{
	this->h_dist = n;
}

void Cell::updateF()
{
	this->f_dist = this->h_dist + this->g_dist;
}

void Cell::draw(sf::RenderWindow& window)
{
	if (solid)
	{
		tile.setFillColor(sf::Color(200, 200, 200));
	}
	else
	{
		switch (this->c)
		{
		case 'A': tile.setFillColor(sf::Color::Red);
			break;
		case 'B': tile.setFillColor(sf::Color::Blue);
			break;
		default: tile.setFillColor(sf::Color::Black);
		}
	}
	window.draw(tile);
}

void Cell::setColor(const sf::Color color)
{
	this->tile.setFillColor(color);
}

bool Cell::operator==(const Cell& other) const
{
	return this->pos == other.pos;
}

bool Cell::operator!=(const Cell& other) const
{
	return this != &other ;
}

std::ostream& operator<<(std::ostream& os, const Cell& cell)
{
	return os << cell.c;
}
