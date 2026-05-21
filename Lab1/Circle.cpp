#include "Circle.hpp"
#include <iostream>

Circle::Circle() {}
Circle::Circle(Point p, double r) : Ellipse(p, r, r) {}

void Circle::render() {
	std::cout << "Circle(" << this->center << ", " << this->a << ')' << '\n';
}