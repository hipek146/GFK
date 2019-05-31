#pragma once
#include "SFML/Graphics.hpp"
#include <vector>


class simplerTriangulation
{

public:
	void printPoints(const std::vector<sf::Vector2f> pointsVector, std::string title);	// do debugowania
	float squareDistance(sf::Vector2f p1, sf::Vector2f p2);
	int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);



private:

}