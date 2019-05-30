#include "Triangulation.hpp"
#include <iostream>

Triangle::Triangle(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
{
	points[0] = p1;
	points[1] = p2;
	points[2] = p3;
}

bool Triangle::isPointInTriangle(sf::Vector2f pt)
{
	if (pt == points[0] || pt == points[1] || pt == points[2])
	{
		return false;
	}
	float A = triangleArea(points[0], points[1], points[2]);
	float A1 = triangleArea(pt, points[1], points[2]);
	float A2 = triangleArea(points[0], pt, points[2]);
	float A3 = triangleArea(points[0], points[1], pt);
	return (A == (A1 + A2 + A3));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

CompareBySmallerAngle::CompareBySmallerAngle(sf::Vector2f basePoint) : m_basePoint{ basePoint } {};

bool CompareBySmallerAngle::operator()(sf::Vector2f p1, sf::Vector2f p2) const
{
	float angle1 = atan2((p1.y - m_basePoint.y), p1.x - m_basePoint.x) * 180 / PI;
	float angle2 = atan2((p2.y - m_basePoint.y), p2.x - m_basePoint.x) * 180 / PI;
	return (angle1 > angle2);
}

float triangleArea(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3)
{
	return abs((p1.x*(p2.y - p3.y) + p2.x*(p3.y - p1.y) + p3.x*(p1.y - p2.y)) / 2.0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Triangulation::Triangulation(std::vector <sf::Vector2f> inputPoints) : m_inputedPoints{ inputPoints }, m_sortedPoints{ inputPoints } { refresh(); }

void Triangulation::refresh() {

	findLowestPointFromInput();
	CompareBySmallerAngle comparement(m_lowestPoint);
	//std::cout << "najnizszy: " << m_lowestPoint.x << ", " << m_lowestPoint.y << std::endl;
	sort(m_sortedPoints.begin(), m_sortedPoints.end(), comparement);
	m_preparedPoints = m_sortedPoints;
	printPoints(m_inputedPoints, "zwykle: ");
	printPoints(m_sortedPoints, "posortowane: ");
	findConvexAndReflexPoints();
	printPoints(m_reflexVertices, "reflex: ");
	printPoints(m_convexHullPoints, "convex: ");
}
int Triangulation::findPoint(sf::Vector2f point, std::vector<sf::Vector2f> pointsVector)
{
	int index;
	for (index = 0; index < pointsVector.size(); index++) {
		if (pointsVector[index] == point) {
			return index;
		}
	}
}

void Triangulation::findConvexAndReflexPoints()
{

	if (m_inputedPoints.size() < 4)	// jak mowi przyslowie: trojkata nawet Salomon nie podzieli na trojkaty
	{
		return;
	}
	m_convexHullPoints.push_back(m_preparedPoints[0]);
	m_convexHullPoints.push_back(m_preparedPoints[1]);
	m_convexHullPoints.push_back(m_preparedPoints[2]);

	for (int i = 3; i < m_preparedPoints.size(); i++)
	{
		while (orientation(m_convexHullPoints[m_convexHullPoints.size() - 2], m_convexHullPoints.back(), m_preparedPoints[i]) == 2)
		{
			m_reflexVertices.push_back(m_convexHullPoints.back());
			m_convexHullPoints.pop_back();
		}
		m_convexHullPoints.push_back(m_preparedPoints[i]);
	}
}


void Triangulation::findEarsPoints() 
{
	int currentIndex, previousIndex, nextIndex;

	for (int i = 0; i < m_convexHullPoints.size(); i++)
	{
		currentIndex = findPoint(m_convexHullPoints[i], m_inputedPoints);

		if (currentIndex == 0)
		{
			previousIndex = m_inputedPoints.size() - 1;
			nextIndex = currentIndex + 1;
		}
		else if (currentIndex == (m_inputedPoints.size() - 1))
		{
			previousIndex = currentIndex - 1;
			nextIndex = 0;
		}
		else
		{
			previousIndex = currentIndex - 1;
			nextIndex = currentIndex + 1;
		}

		bool canAddEar = true;

		for (int j = 0; j < m_reflexVertices.size(); j++)
		{
			Triangle currentTriangle(m_inputedPoints[previousIndex], m_inputedPoints[currentIndex], m_inputedPoints[nextIndex]);
			if (currentTriangle.isPointInTriangle(m_reflexVertices[j]) == true)
			{
				canAddEar = false;
				break;
			}
		}

		if (canAddEar) {
					/// todo
			break;
		}
	}
}

float  Triangulation::squareDistance(sf::Vector2f p1, sf::Vector2f p2)
{
	return (p1.x - p2.x)*(p1.x - p2.x) +
		   (p1.y - p2.y)*(p1.y - p2.y);
}

int Triangulation::orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		      (q.x - p.x) * (r.y - q.y);
	if (val == 0) return 0;  // leza razem
	return (val > 0) ? 1 : 2; // ruch wskazowek zegara : przeciwnie do wskazowek zegara
}

void Triangulation::printPoints(const std::vector<sf::Vector2f> pointsVector, std::string title)
{
	std::cout << title << std::endl;
	for (int i = 0; i < pointsVector.size(); i++)
	{
		std::cout << "[" << i << "]" << pointsVector[i].x << ", " << pointsVector[i].y << std::endl;
	}
}

void Triangulation::findLowestPointFromInput()
{
	m_lowestPoint = m_inputedPoints.back();
	for (int i = 0; i < m_inputedPoints.size(); i++)
	{
		if (m_lowestPoint.y < m_inputedPoints[i].y)
		{
			m_lowestPoint = m_inputedPoints[i];
		}
		else if (m_lowestPoint.y == m_inputedPoints[i].y)
		{
			if (m_lowestPoint.x > m_inputedPoints[i].x)
			{
				m_lowestPoint = m_inputedPoints[i];
			}
		}
	}
}


void Triangulation::removePointsWithSameAngle()
{
	// od i = 1 bo musimy porownywac pare punktow
	for (int i = 1; i < m_sortedPoints.size(); i++)
	{
		float previousAngle = atan2((m_sortedPoints[i - 1].y - m_lowestPoint.y), m_sortedPoints[i - 1].x - m_lowestPoint.x) * 180 / PI;
		float currentAngle = atan2((m_sortedPoints[i].y - m_lowestPoint.y), m_sortedPoints[i].x - m_lowestPoint.x) * 180 / PI;

		if (previousAngle == currentAngle)
		{
			// usuwamy ten punkt ktory lezy blizej
			if (squareDistance(m_lowestPoint, m_sortedPoints[i - 1]) > squareDistance(m_lowestPoint, m_sortedPoints[i]))
			{
				m_sortedPoints.erase(m_sortedPoints.begin() + i);
			}
			else
			{
				m_sortedPoints.erase(m_sortedPoints.begin() + (i - 1));
			}
			// musimy sie cofnac o 1 iteracje, bo moze byc kilka punktow z takimi samymi katami
			i--;
		}
	}
}


