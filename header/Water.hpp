#pragma once
#include "SFML/Graphics.hpp"

struct myLine;

class Water
{
public:
	Water() = default;
	Water(int left, int right, int deep, float value, std::vector<myLine> UP) : left(left), right(right), deep(deep), value(value), up(UP)
	{}
	int lenght;
	float value;
	int left;
	int right;
	int deep;
	std::vector <Water*> below;
	static constexpr const float unit = 2.0f;

	std::vector<Water*> parent;
	std::vector<Water*> child;
	std::vector<myLine> up{};

	bool full = false;
};
