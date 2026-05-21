#pragma once

#include "Ellipse.hpp"

class Circle : public Ellipse {
public:
	void render() override;
	Circle();
	Circle(Point p, double r);
};