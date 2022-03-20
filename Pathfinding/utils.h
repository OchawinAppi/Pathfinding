#pragma once

#include <SFML/Graphics.hpp>

sf::Vector2i vfToVi(sf::Vector2f vec)
{
	return sf::Vector2i(static_cast<int>(vec.x), static_cast<int>(vec.y));
}

sf::Vector2f viToVf(sf::Vector2i vec)
{
	return sf::Vector2f(static_cast<float>(vec.x), static_cast<float>(vec.y));
}

bool isAnyKeyPressed()
{
	for (int k = -1; k < sf::Keyboard::KeyCount; ++k)
	{
		if (sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(k)))
			return true;
	}
	return false;
}

inline int getPositionCode(sf::Vector2f pos)
{
	return (static_cast<int>(pos.x) + static_cast<int>(pos.y) * MAP_WIDTH);
}