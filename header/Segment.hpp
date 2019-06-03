#pragma once
#include "SFML/Graphics.hpp"
#include <iostream>

enum class SegmentType
{
	Line,
	Bezier,
};

class Segment : public sf::Drawable
{
public:
	Segment() = default;
	Segment(SegmentType type, sf::Vector2f start, sf::Vector2f end, sf::Vector2f parameter = {})
	{
		if (type == SegmentType::Line)
		{
			float length = sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2));
			int i = static_cast<int>(round(length / distance));
			count = i;
			float exactDistance = length / count;
			float angle = atan2(end.y, end.x) - atan2(start.y, end.x);

			float deltaX = cos(angle) * distance;
			float deltaY = sin(angle) * distance;
			++i;
			while (i)
			{
				points.push_back({ start.x + deltaX * i, start.y + deltaY * i });
				--i;
			}
		}
		else if (type == SegmentType::Bezier)
		{
			float delta = 0.01f;
			float length = 0.0f;
			sf::Vector2f tmpPoint = start;
			sf::Vector2f tmpPointNext;
			for(float t = delta; t < 1; t+= delta)
			{
				tmpPointNext = pow(1 - t, 2) * start + 2 * (1 - t) * t * parameter + t * t * end;
				length += sqrt(pow(tmpPointNext.x - tmpPoint.x, 2) + pow(tmpPointNext.y - tmpPoint.y, 2));
				tmpPoint = tmpPointNext;
			}
			int i = static_cast<int>(round(length / distance));
			count = i;
			float exactDistance = length / count;
			tmpPoint = start;
			float t = 0.0f, tNext, epsilo = 0.05f * distance, a;
			int flag = 0;
			int licznik;
			while (i)
			{
				points.push_back(tmpPoint);

				delta = 1.0f / count;
				a = delta / 2.0f;
				licznik = 0;
				while (true)
				{
					++licznik;
					tNext = t + delta;;
					tmpPointNext = pow(1 - tNext, 2) * start + 2 * (1 - tNext) * tNext * parameter + tNext * tNext * end;
					if (sqrt(pow(tmpPointNext.x - tmpPoint.x, 2) + pow(tmpPointNext.y - tmpPoint.y, 2)) > exactDistance + epsilo)
					{
						if (flag == 2) a /= 2.0f;
						flag = 1;
						delta -= a;
					}
					else if (sqrt(pow(tmpPointNext.x - tmpPoint.x, 2) + pow(tmpPointNext.y - tmpPoint.y, 2)) < exactDistance - epsilo)
					{
						if (flag == 1) a /= 2.0f;
						flag = 2;
						delta += a;
					}
					else
					{
						break;
					}
				}
				tmpPoint = tmpPointNext;
				t = tNext;
				--i;
			}
			points.push_back(end);
		}
		count = points.size();
		lines = sf::VertexArray(sf::LinesStrip, count);

		for (int i = 0; i < count; ++i)
		{
			lines[i].position = points[i];
			lines[i].color = sf::Color::Black;
		}
		rect = lines.getBounds();
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		
		target.draw(lines);
	}

	sf::FloatRect rect;
	std::vector<sf::Vector2f> points;
private:
	float distance = 5.0f;
	int count;
	sf::VertexArray lines;
};
