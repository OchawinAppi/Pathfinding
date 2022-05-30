#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "config.h"

class Cell final
{
public:
	// A-Star attributes
	int          g_dist;
	int          h_dist;
	int          f_dist;
	bool         is_path;
	sf::Vector2f pos;
	bool         solid; ///< Won't evaluate solid cells ( walls )
	char         c;     ///< for printing on console ( pre-SFML ) AND conditional checking!

	// SFML
	sf::RectangleShape tile;

	// Constructors, Destructor
	Cell();
	Cell(int g, int h, bool solid, int x, int y);
	~Cell() = default;

	// Methods
	void makeSolid();
	void makeEmpty();
	void updateG(int n);
	void updateH(int n);
	void updateF();
	void draw(sf::RenderWindow& window); // <- SFML

	void setColor(sf::Color color);

	// Friends, because we don't want lonely classes ;)
	friend std::ostream& operator<<(std::ostream& os, const Cell& cell);
	bool                 operator==(const Cell& other) const;
	bool                 operator!=(const Cell& other) const;
};
