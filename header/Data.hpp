#pragma once
#include "Segment.hpp"
#include "Water.hpp"
#include <iostream>
#include <list>
#include "Data.hpp"

struct myLine
{
	int left;
	int right;
};

class Data
{
	friend class App;
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
	
	std::vector<myLine> CreateWater(int left, int right, int deep, float &value, std::vector<myLine> UP)
	{

		water.push_back({ left, right, deep, value, UP });
		int i = 0;
		Water *ptr = &water.back();
		Water *tmpPtr;
		for (auto it = water.begin(); it !=water.end(); ++ it)
		{
			tmpPtr = &(*it);
			if (tmpPtr->deep == deep + 1 && tmpPtr->left <= right && tmpPtr->right >= left)
			{
				ptr->parent.push_back(tmpPtr);
				tmpPtr->child.push_back(ptr);
			}
		}
		
		std::vector<myLine> toFill;
		fillWater(ptr, &toFill, deep);
		return toFill;
	}
	void RemoveWater()
	{
	/*	for (auto &element : water.back().parent)
		{
			for (auto it = element->child.begin(); it != element->child.end(); ++it)
			{
				if (*it == &water.back())
				{
					element->child.erase(it);
					break;
				}
			}
			
		}*/
	}
	void fillWater(Water *myWater, std::vector<myLine> *toFill, int toDeep)
	{
		
		if (myWater->parent.size())
		{
			for (auto &water : myWater->parent)
			{
				fillWater(water, toFill, toDeep);
			}
		}
		else if (myWater->child.size())
		{
			onSurface(myWater, toFill, toDeep);
		}
	}
	void onSurface(Water *myWater, std::vector<myLine> *toFill, int toDeep)
	{
		if (myWater->child.size())
		{
			for (auto &water : myWater->child)
			{
				onSurface(water, toFill, toDeep);
			}
		}
		if(myWater->deep == toDeep + 1)
		{
			for (auto up : myWater->up)
			{
				for (int i = up.left; i <= up.right; ++i)
				{
					
					if (!CheckWater(i, myWater->deep - 1))
					{
						toFill->push_back({ up.left, up.right });
						break;
					}
				}
			}
			myWater->full = true;
		}
	}
	bool CheckWater(int x, int y)
	{
		for (auto &element : water)
		{
			if (element.deep == y && element.left <= x && element.right >= x)
			{
				return true;
			}
		}
		return false;
	}

	friend class Workspace;
private:
	std::vector<Segment> segments;
	std::list<Water> water;
};
