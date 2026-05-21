#pragma once

#include "Figure.hpp"
#include "Point.hpp"

class Ellipse : public Figure {
public:
	Point center;
	double a, b; /* радиусы */
	void render() override;

	Ellipse();
	Ellipse(Point center, double a, double b);
};