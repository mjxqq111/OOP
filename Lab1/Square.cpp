#include "Square.hpp"
#include <iostream>

Square::Square() {}
Square::Square(Point p, double a) : Rectangle(p.x, p.y, p.x+a, p.y+a), a(a) {}

void Square::render() {
	std::cout << "Square(" << this->p1 << ", " << this->a << ')' << '\n';
}