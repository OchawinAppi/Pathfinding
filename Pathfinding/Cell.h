#pragma once
#include "Directions.h"
#include <SFML/Graphics.hpp>
#include "Config.h"


class Cell {
public:
	// A-Star attributes
	int G_dist;
	int H_dist;
	int F_dist;

	bool isPath = false;

	sf::Vector2f pos;
	Direction dir; // <- direction leading to parent cell ( for forming path later )

	bool solid; // <- Won't evaluate solid cells ( walls )
	char c; // <- for printing on console ( pre-sfml ) AND conditional checking!
	bool isChecked = false; // <- for A-Star

	// SFML
	sf::RectangleShape tile;

	// Constructors
	Cell();
	Cell(int g, int h, bool _solid, int _x, int _y, Direction _direction = Direction::NONE);
	// Destructor not needed. The compiler got this. ;) ( probably )
	
	// Methods
	void makeSolid();
	void makeEmpty();
	void makePath();
	void updateG(int n);
	void updateH(int n);
	void updateF();
	bool& getChecked();
	void checked();
	void draw(sf::RenderWindow& window); // <- SFML


	// Friends, because we don't want lonely classes ;)
	friend std::ostream& operator<<(std::ostream& os, const Cell& cell);
	bool operator== (const Cell& other) const;
	bool operator!= (const Cell& other) const;

	// Points
	void placeLetter(char _c);
};

