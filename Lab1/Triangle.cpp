#include "Triangle.hpp"
#include "Point.hpp"
#include <iostream>

Triangle::Triangle() {}
Triangle::Triangle(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {}

void Triangle::render() {
	std::cout << "Triangle(" << this->p1 << ", " << this->p2 << ", " << this->p3 << ')' << '\n';
}