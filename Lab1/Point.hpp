#pragma once

#include <ostream>

struct Point {
	double x = 0;
	double y = 0;
	
	Point();
	Point(double x, double y);

	friend std::ostream& operator<<(std::ostream& s, Point& p);
};