#include "Headers/prims.h"
#include <iostream>

inline int getNeighbors(std::vector<std::vector<int>> &map, int x, int y, int width, int height) {    // Gets the neighbors of a given cell, if a given neighbor cell is empty, count goes up by one. (Not a wall)
    int count = 0;
    if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) return 0; // Make sure it isn't out of bounds.
    if (map.at(y + 1).at(x) == 0) count++;
    if (map.at(y - 1).at(x) == 0) count++;
    if (map.at(y).at(x + 1) == 0) count++;
    if (map.at(y).at(x - 1) == 0) count++;
    return count;
}

int clamp(int x, int min, int max) {
    if (x > max) return max;
    if (x < min) return min;
    return x;
}

Prims::Prims(int width, int height) :
    MazeAlgo(width, height) {}

std::vector<sf::Vector2f> Prims::generate()
{
    int& height = this->height;
    int& width = this->width;
    struct Pos { int x, y; };
    std::vector<sf::Vector2f> returnPath{};
	std::vector<intVec> map(height, intVec(width, 1));

    // Choose a random point
    srand((int)time(0));
    int x = rand() % width;
    srand((int)time(0));
    int y = rand() % height;
    x = clamp(x, 1, width);
    y = clamp(y, 1, height);

    map.at(y).at(x) = 0;

    // Create a vector containing the points of all neighboring cells (addresses)
    std::vector<Pos> walls;
    walls.push_back(Pos{ x + 1, y });
    walls.push_back(Pos{ x - 1, y });
    walls.push_back(Pos{ x,     y + 1 });
    walls.push_back(Pos{ x,     y - 1 });
    returnPath.emplace_back(x, y);

    while (walls.size() > 0) {
        srand((int)time(0));
        int curWall = (rand() % walls.size()); // Get access to a random address and this becomes Current Wall

        x = walls.at(curWall).x;    // Re-assign x and y, pulling them from the address. (This is why the class Map (vector2i) was created)
        y = walls.at(curWall).y;
        walls.erase(walls.begin() + curWall);   // Remove address of the current wall.

        if (getNeighbors(map, x, y, width, height) == 1) { // If current cell has only one alive neighbor, a passage is possible.
            map.at(y).at(x) = 0;       // Make the passage.
            returnPath.emplace_back(x, y);
            walls.push_back(Pos{ x + 1, y });
            walls.push_back(Pos{ x - 1, y });
            walls.push_back(Pos{ x, y + 1 });
            walls.push_back(Pos{ x, y - 1 });
        }
    }
    return returnPath;
}




