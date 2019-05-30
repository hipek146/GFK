#pragma once
#include "SFML/Graphics.hpp"
#include <vector>

#define PI 3.1415926535
float triangleArea(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);

struct Triangle 
{
	sf::Vector2f points[3];
	Triangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);


	bool isPointInTriangle(sf::Vector2f point);
};

///////////////////////////////////


// taki trick zeby do funkcji przyjmujacej 2 parametry dalo sie przekazac 3. nie wiem co robie, mindur nie bij plz
class CompareBySmallerAngle {

private:
	sf::Vector2f m_basePoint;

public:
	CompareBySmallerAngle(sf::Vector2f basePoint);

	bool operator()(sf::Vector2f p1, sf::Vector2f p2) const;
};



float triangleArea(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);
////////////////////////////////////////////////
class Triangulation
{

	public:

		Triangulation(std::vector <sf::Vector2f> );
		void refresh();
	private:
		
		void findConvexAndReflexPoints();
		void findEarsPoints();
		void findLowestPointFromInput();
		void removePointsWithSameAngle();
		void printPoints(const std::vector<sf::Vector2f> pointsVector, std::string title);	// do debugowania
		float squareDistance(sf::Vector2f p1, sf::Vector2f p2);
		int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);
		int findPoint(sf::Vector2f point, std::vector<sf::Vector2f> pointsVector);

		std::vector <sf::Vector2f> m_inputedPoints;
		std::vector <sf::Vector2f> m_sortedPoints;
		std::vector <sf::Vector2f> m_preparedPoints;
		std::vector <Triangle> m_triangles;
		std::vector <sf::Vector2f> m_convexHullPoints;
		std::vector <sf::Vector2f> m_reflexVertices;

		sf::Vector2f m_lowestPoint;

};


