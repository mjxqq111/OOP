#include "Point.hpp"

#include <tuple>

Point::Point() {}

Point::Point(double x, double y) : x(x), y(y) {}

std::ostream& operator<<(std::ostream& s, Point& p) {
	return s << '(' << p.x << ", " << p.y << ')';
}