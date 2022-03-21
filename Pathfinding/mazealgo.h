#pragma once
#include <vector>

class MazeAlgo
{
private:
	MazeAlgo();
protected:
	int x, y;
	MazeAlgo(int _x, int _y) :
		x(_x), y(_y) {}
public:
	virtual void generate() = 0;
};

