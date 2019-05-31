#include "simplerTriangulation.hpp"
#include <iostream>

float  simplerTriangulation::squareDistance(sf::Vector2f p1, sf::Vector2f p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) +
		(p1.y - p2.y)*(p1.y - p2.y);
}

int simplerTriangulation::orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0;  // leza razem
	return (val > 0) ? 1 : 2; // ruch wskazowek zegara : przeciwnie do wskazowek zegara
}

void simplerTriangulation::printPoints(const std::vector<sf::Vector2f> pointsVector, std::string title)
{
	std::cout << title << std::endl;
	for (int i = 0; i < pointsVector.size(); i++)
	{
		std::cout << "[" << i << "]" << pointsVector[i].x << ", " << pointsVector[i].y << std::endl;
	}
}