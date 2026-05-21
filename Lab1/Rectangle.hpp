#pragma once

#include "Point.hpp"
#include "Figure.hpp"

class Rectangle : public Figure {
public:
	Point p1, p2;

	void render() override;

	Rectangle();
	Rectangle(double x1, double y1, double x2, double y2);
};