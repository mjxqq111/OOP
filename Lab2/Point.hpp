#pragma once

// 2D point
struct Point {
	double x = 0;
	double y = 0;

	Point();	// Default consturctor
	Point(double x, double y);	// Constructor with coordinates
};