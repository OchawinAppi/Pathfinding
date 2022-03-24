#pragma once
#include "mazeAlgo.h"

class Prims :
    public MazeAlgo
{
public:
    using intVec = std::vector<int>;
    // Self defined constructor
    Prims(int x, int y);

    // Inherited via MazeAlgo
    virtual std::vector<sf::Vector2f> generate() override;
};