#pragma once
#include "Config.h"
#include <SFML/Graphics.hpp>


class Cell {
public:
	// A-Star attributes
	int G_dist;
	int H_dist;
	int F_dist;

	bool isPath = false;

	sf::Vector2f pos;

	bool solid; // <- Won't evaluate solid cells ( walls )
	char c; // <- for printing on console ( pre-sfml ) AND conditional checking!
	bool isChecked = false; // <- for A-Star

	// SFML
	sf::RectangleShape tile;

	// Constructors
	Cell();
	Cell(int g, int h, bool _solid, int _x, int _y);
	// Destructor not needed. The compiler got this. ;) ( probably )
	
	// Methods
	void makeSolid();
	void makeEmpty();
	void updateG(int n);
	void updateH(int n);
	void updateF();
	bool& getChecked();
	void checked();
	void draw(sf::RenderWindow& window); // <- SFML

	void setColor(sf::Color color);

	// Friends, because we don't want lonely classes ;)
	friend std::ostream& operator<<(std::ostream& os, const Cell& cell);
	bool operator== (const Cell& other) const;
	bool operator!= (const Cell& other) const;
};

