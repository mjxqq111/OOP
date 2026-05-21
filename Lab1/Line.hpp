#pragma once

#include "Figure.hpp"
#include "Point.hpp"

class Line : public Figure {
public:
	Point p1, p2;

	void render() override;

	Line();
	Line(double x1, double y1, double x2, double y2);
};