#include "Line.hpp"

#include <iostream>

Line::Line() {}
Line::Line(double x1, double y1, double x2, double y2) : p1({ x1, y1 }), p2({ x2, y2 }) {}

void Line::render() {
	std::cout << "Line(" << this->p1 << ", " << this->p2 << ')' << '\n';
}