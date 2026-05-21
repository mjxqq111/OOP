#include "Rectangle.hpp"
#include <iostream>

Rectangle::Rectangle() {}
Rectangle::Rectangle(double x1, double y1, double x2, double y2) : p1({ x1,y1 }), p2({ x2, y2 }) {}

void Rectangle::render() {
	std::cout << "Rectangle(" << this->p1 << ", " << this->p2 << ')' << '\n';
}