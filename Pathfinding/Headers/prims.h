#pragma once
#include "mazeAlgo.h"

class Prims final :
    public MazeAlgo
{
public:
    using IntVec = std::vector<int>;
    // Self defined constructor
    Prims(int x, int y);

    // Inherited via MazeAlgo
    std::vector<sf::Vector2i> generate() override;
};