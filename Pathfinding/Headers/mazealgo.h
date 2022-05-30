#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class MazeAlgo
{
private:
	MazeAlgo();
protected:
	int width, height;

	MazeAlgo(const int width,
	         const int height)
		: width(width)
		, height(height)
	{
	}

public:
	virtual                           ~MazeAlgo() = default;
	virtual std::vector<sf::Vector2i> generate() = 0;
};
