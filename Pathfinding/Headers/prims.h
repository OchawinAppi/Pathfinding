#pragma once
#include "mazeAlgo.h"
class Prims :
    public MazeAlgo
{
    // Self defined constructor
    Prims(int x, int y) :
        MazeAlgo(x, y) {}

    // Inherited via MazeAlgo
    virtual void generate() override;
};

