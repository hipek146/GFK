#pragma once
#include "GUI.hpp"

class ScrollBar : public GUI
{
public:
	ScrollBar(float *value = nullptr) : x(value)
	{
		shape.setFillColor(color);
		circleShape.setFillColor(circleColor);
		circleShape.setOutlineColor(sf::Color(236, 227, 200, 250));
		if (x == nullptr)
		{
			x = &myValue;
		}
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
		target.draw(circleShape);
	}

	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		sf::Vector2f oldSize(rect.width, rect.height);
		rect.width = newSize.x;
		rect.height = newSize.y;

		shape.setSize({ newSize.x, newSize.y / 2.0f });
		circleShape.setRadius(newSize.y * 0.37f);
		circleShape.setOutlineThickness(newSize.y * 0.03f);

		SetPosition({ rect.left + (oldSize.x) / 2.0f, rect.top + (oldSize.y) / 2.0f });
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x - rect.width / 2.0f;
		rect.top = newPosition.y - rect.height / 2.0f;

		shape.setPosition(rect.left, rect.top + rect.height / 2.0f - rect.height / 4.0f);
		if (x == nullptr)
		{
			circleShape.setPosition(newPosition.x - circleShape.getRadius(), newPosition.y - circleShape.getRadius());
		}
		else
		{
			circleShape.setPosition(newPosition.x - circleShape.getRadius() + (rect.width - 2 * circleShape.getRadius()) * ((*x - 50.0f) / 100.0f), newPosition.y - circleShape.getRadius());
		}
	}

	virtual void SetEvents(Screen *newScreen) override
	{
		screen = newScreen;
		screen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &ScrollBar::OnMove, EventType::MouseMove);
		screen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &ScrollBar::OnClick, EventType::Pressed);
		screen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &ScrollBar::OnReleased, EventType::Released);
	}
	void GetValue(float *value)
	{
		x = value;
	}
	void OnMove()
	{
		Update();
	}
	void OnReleased()
	{
		clicked = false;
	}
	void OnClick()
	{
		clicked = true;
		Update();
	}
private:
	void Update()
	{
		if (clicked && x != nullptr)
		{
			*x = 100.0f * (screen->app->event->mouse.x - rect.left - circleShape.getRadius()) / (rect.width - 2 * circleShape.getRadius());
			if (*x < 0.0f)
			{
				*x = 0.0f;
			}
			else if (*x > 100.0f)
			{
				*x = 100.0f;
			}
			SetPosition({ rect.left + rect.width / 2.0f, rect.top + rect.height / 2.0f });
			callbackHandle();
		}
	}
	bool clicked = false;
	Screen *screen;
	float *x;
	float myValue = 50.0f;
	sf::RectangleShape shape;
	sf::Color color = { 34, 218, 215, 230 };
	sf::CircleShape circleShape;
	sf::Color circleColor = { 139, 46, 59, 240 };
};