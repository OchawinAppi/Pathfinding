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