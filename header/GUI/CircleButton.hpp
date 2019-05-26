#pragma once
#include "GUI.hpp"


class CircleButton : public GUI
{
public:
	CircleButton()
	{
		circleShape.setOutlineColor(sf::Color(236, 227, 200, 250));
		MouseOut();
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(circleShape);
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		rect.width = newSize.x;
		rect.height = newSize.y;

		circleShape.setRadius(newSize.x * 0.6f);
		circleShape.setOutlineThickness(newSize.x * 0.1f);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x - rect.width / 2.0f;
		rect.top = newPosition.y - rect.height / 2.0f;
		circleShape.setPosition(newPosition.x - rect.width * 0.6f, newPosition.y - rect.height * 0.6f);
	}
	virtual void MouseOver() override
	{
		circleShape.setFillColor(sf::Color(59, 100, 166, 250));
	}
	virtual void MouseOut() override
	{
		circleShape.setFillColor(sf::Color(90, 140, 218, 250));
	}
private:
	sf::CircleShape circleShape;
};