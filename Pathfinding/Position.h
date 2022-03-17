#pragma once
class Position {
public:
	int x;
	int y;

	Position();
	// Destructor not needed. The compiler got this. ;) ( probably )
	Position(int _x, int _y);
};
