#include "Workspace.hpp"
#include "sfLine.hpp"
#include <iostream>
#include <vector>
#include "Triangulation.hpp"



void Workspace::draw(sf::RenderTarget& target, sf::RenderStates states) const
{

	sf::Vertex vertices[3];
	sf::Color color;
	//sf::Triangles
	//triangulation->m_triangles.size()
		for (int i = 0; i < 2; i++) {
			if (i % 2 == 0) {
				color = sf::Color::Green;
			}
			else {
				color = sf::Color::Red;
			}
			vertices[0] = sf::Vertex(sf::Vector2f(triangulation->m_triangles[i].points[0].x, triangulation->m_triangles[i].points[0].y), color);
			vertices[1] = sf::Vertex(sf::Vector2f(triangulation->m_triangles[i].points[1].x, triangulation->m_triangles[i].points[1].y), color);
			vertices[2] = sf::Vertex(sf::Vector2f(triangulation->m_triangles[i].points[2].x, triangulation->m_triangles[i].points[2].y), color);

			target.draw(vertices, 3, sf::Triangles);
		} 

	DrawLinesGroup(target, states);
	//DrawCurrentLine(target, states);
}

void PerlinNoise1D(int nCount, float *fSeed, int nOctaves, float fBias, float *fOutput)
{
	// Used 1D Perlin Noise
	for (int x = 0; x < nCount; x++)
	{
		float fNoise = 0.0f;
		float fScaleAcc = 0.0f;
		float fScale = 1.0f;

		for (int o = 0; o < nOctaves; o++)
		{
			int nPitch = nCount >> o;
			int nSample1 = (x / nPitch) * nPitch;
			int nSample2 = (nSample1 + nPitch) % nCount;
			float fBlend = (float)(x - nSample1) / (float)nPitch;
			float fSample = (1.0f - fBlend) * fSeed[nSample1] + fBlend * fSeed[nSample2];
			fScaleAcc += fScale;
			fNoise += fSample * fScale;
			fScale = fScale / fBias;
		}

		// Scale to seed range
		fOutput[x] = fNoise / fScaleAcc;
	}
}

Workspace::Workspace() 
{

	// punkty testowe
	mainPoints.push_back(sf::Vector2f(450, 480));	// 0
	mainPoints.push_back(sf::Vector2f(600, 810));	// 1
	mainPoints.push_back(sf::Vector2f(840, 540));	// 2
	mainPoints.push_back(sf::Vector2f(690, 90));	// 3
	mainPoints.push_back(sf::Vector2f(240, 510));	// 4
	mainPoints.push_back(sf::Vector2f(60, 270));	// 5
	mainPoints.push_back(sf::Vector2f(90, 720)); // 6


	triangulation = new Triangulation(mainPoints);

	//triangulation->printPoints(triangulation->inputedPoints, "punkty: ");

}

void Workspace::DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mainPoints.size() < 2) {
		return;
	}
	for (int i = 0; i < mainPoints.size() - 1; i++) 
	{
		sfLine(mainPoints[i], mainPoints[i + 1], sf::Color::Black, 3.0).draw(target, states);		
	}
}

void Workspace::DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const
{	
	if (mainPoints.size() < 1)
	{
		return;
	}
	sf::Color lineColor;
	if (CheckAllColisions(mainPoints.back(), mousePosition)) 
	{
		lineColor = sf::Color::Red;
	}
	else
	{
		lineColor = sf::Color::Green;
	}
	sfLine(mainPoints.back(), mousePosition, lineColor, 3.0).draw(target, states);
}

bool Workspace::CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const
{	
	for (int i = 0; i < mainPoints.size() - 2; i++)	
	{
		if (CheckColision(mainPoints[i], mainPoints[i + 1], a, b))
		{
			return true;
		}
	}
	return false;
}


inline bool Workspace::CheckColision(sf::Vector2f a1, sf::Vector2f b1, sf::Vector2f a2, sf::Vector2f b2) const
{	
	
	// sprawdza kolizje 2 lini:
// a1-------b1 oraz a2---------b2

	int x1 = a1.x;
	int y1 = a1.y;

	int x2 = b1.x;
	int y2 = b1.y;

	int x3 = a2.x;
	int y3 = a2.y;

	int x4 = b2.x;
	int y4 = b2.y;

	float denominator = ((x2 - x1) * (y4 - y3)) - ((y2 - y1) * (x4 - x3));
	float numerator1 = ((y1 - y3) * (x4 - x3)) - ((x1 - x3) * (y4 - y3));
	float numerator2 = ((y1 - y3) * (x2 - x1)) - ((x1 - x3) * (y2 - y1));

	if (denominator == 0) return (numerator1 == 0 && numerator2 == 0);	// sprawdza czy mianownik jest 0
	float r = numerator1 / denominator;
	float s = numerator2 / denominator;
	return (r >= 0 && r < 1) && (s >= 0 && s <= 1);
}

void Workspace::AddPoint(int x, int y) 
{	
//	if (mainPoints.size() == 0)
//	{
//		mainPoints.push_back(sf::Vector2f(1.0*x, 1.0*y));
//	}
	//else if (CheckAllColisions(mainPoints[mainPoints.size()-1], mousePosition) == false) 
	{
		mainPoints.push_back(sf::Vector2f(1.0*x, 1.0*y));
	}
}


