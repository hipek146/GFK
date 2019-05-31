#pragma once 
#include <iostream>
#include <vector>
#include "sfLine.hpp"
#include <SFML/Graphics.hpp>


class Bezier{

	public:

		void CalcQuadBezier() {

			points.clear();
			int numSegments = 50;

			points.push_back(startPoint);

			float step = 1.f / numSegments;
			float t = step;
			for (size_t i = 1; i < numSegments; i++, t += step) {
				points.push_back(((1.f - t) * (1.f - t)) * startPoint + (2.f * (1.f - t) * t * controlPoint + t * t * endPoint));
			}
		}
		
		void setControlPoint(float x, float y) {
			controlPoint.x = x;
			controlPoint.y = y;
		}

		void setEndPoint(float x, float y) {
			endPoint.x = x;
			endPoint.y = y;
		}

		void setStartPoint(sf::Vector2f & lP) {
			startPoint.x = lP.x;
			startPoint.y = lP.y;
		}
		
		void pushVerices() {
			
			for (std::vector<sf::Vector2f>::const_iterator a = points.begin(); a != points.end(); ++a)
				vertices.append(sf::Vertex(*a, sf::Color::Black));
		}

		void DrawCurvesGroup(sf::RenderTarget& target, sf::RenderStates states) const {
			target.draw(vertices, states);
		}


		sf::Vector2f startPoint;
		sf::Vector2f endPoint;
		sf::Vector2f controlPoint;
		sf::VertexArray vertices;
		bool isControlPoint = false;
		std::vector<sf::Vector2f> points;

	};