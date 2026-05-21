#include "Ellipse.hpp"
#include <iostream>

Ellipse::Ellipse() {}
Ellipse::Ellipse(Point center, double a, double b) : center(center), a(a), b(b) {}

void Ellipse::render() {
	std::cout << "Ellipse(" << this->center << ", " << this->a << ", " << this->b << ')' << '\n';
}