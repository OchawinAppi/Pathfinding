#pragma once

#include <math.h>

constexpr int aStarDist(int x2, int y2, int x1, int y1) {
	int x_dist = abs(x2 - x1);
	int y_dist = abs(y2 - y1);

	int distance = 0;
	while (x_dist >= 1 && y_dist >= 1) {
		x_dist--;
		y_dist--;
		distance += 14;
	}
	distance += x_dist * 10;
	distance += y_dist * 10;
	
	return distance;
}


