#pragma once
#include "Segment.hpp"
#include <iostream>

class Data
{
public:
	std::vector<sf::Vector2f> GetVisiblePoints(sf::FloatRect rect)
	{
		std::vector<sf::Vector2f> visiblePoints;
		for (auto &segment : segments)
		{
			if (rect.intersects(segment.rect))
			{
				visiblePoints.insert(visiblePoints.end(), segment.points.begin(), segment.points.end());
			}
		}
		return visiblePoints;
	}
	void Add(Segment newSegment)
	{
		segments.push_back(newSegment);
	}

	friend class Workspace;
private:
	std::vector<Segment> segments;
};
