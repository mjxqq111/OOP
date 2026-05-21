#pragma once

#include "Rectangle.hpp"

class Square : public Rectangle {
public:
	double a = 0; /* сторона */
	void render() override;

	Square();
	Square(Point p, double a);
};