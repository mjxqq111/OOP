#pragma once

#include "Figure.hpp"
#include <vector>
#include <memory>

class FigureList {
public:
	static std::vector<std::unique_ptr<Figure>> l;
};