#pragma once

#include "Figure.hpp"
#include "Point.hpp"

class Triangle : public Figure {
public:
	Point p1, p2, p3;
	void render() override;

	Triangle();
	Triangle(Point p1, Point p2, Point p3);
};