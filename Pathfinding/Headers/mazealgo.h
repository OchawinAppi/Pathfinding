#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class MazeAlgo
{
private:
	MazeAlgo();
protected:
	int width, height;
	MazeAlgo(int _width, int _height) :
		width(_width), height(_height) {}
public:
	virtual std::vector<sf::Vector2f> generate() = 0;
};




