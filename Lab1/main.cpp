#include <iostream>
#include <vector>
#include "FigureList.hpp"
#include "Point.hpp"
#include "Line.hpp"
#include "Rectangle.hpp"
#include "Ellipse.hpp"
#include "Triangle.hpp"
#include "Square.hpp"
#include "Circle.hpp"

int main() {
	/* Заполняем список */
	FigureList::l.push_back(std::make_unique<Line>(10, 20, 30, 40));
	FigureList::l.push_back(std::make_unique<Rectangle>(11.8, 295, 22.45, 1000.1));
	FigureList::l.push_back(std::make_unique<Ellipse>(Point{ 1.1, 2.5 }, 39, 90.8));
	FigureList::l.push_back(std::make_unique<Triangle>(Point{ 1.2, 2.2 }, Point{ 4, 8 }, Point{ 19, 3.5 }));
	FigureList::l.push_back(std::make_unique<Square>(Point{ 2.7, 14 }, 5.2));
	FigureList::l.push_back(std::make_unique<Circle>(Point{ 18.12, 3.6 }, 4.8));

	/* Вызываем отрисовку */
	for (int i = 0; i < FigureList::l.size(); i++) {
		FigureList::l.at(i)->render();
	}
	return 0;
}