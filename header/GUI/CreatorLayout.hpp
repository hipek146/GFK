#pragma once
#include "GUI.hpp"

class CreatorLayout : virtual public Layout, virtual public GUI
{
public:
	template <typename T>
	CreatorLayout(T *screen, sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition)
	{
		shape.setFillColor(sf::Color(13, 45, 81, 200));

		screen->AddEvent(&rect, this, this, &CreatorLayout::BlankEvent, EventType::Blank);
		parent = screen;

		circleButton = new CircleButton();
		screen->AddEvent(&circleButton->rect, circleButton, circleButton, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&circleButton->rect, circleButton, circleButton, &GUI::MouseOut, EventType::MouseOut);
		screen->AddEvent(&circleButton->rect, this, circleButton, &CreatorLayout::Animation, EventType::Pressed);
		circleButton->parent = screen;

		SetSize({ static_cast<float>(newSize.x), static_cast<float>(newSize.y) });
		SetPosition(position);

	}
	virtual ~CreatorLayout()
	{
		delete circleButton;
		delete layout;
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
		target.draw(*layout);
		target.draw(*circleButton);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		if (!setHidden)
		{
			hidden = false;
		}
		position = newPosition;
		shape.setPosition(newPosition);
		rect.left = newPosition.x;
		rect.top = newPosition.y;
		circleButton->SetPosition({ newPosition.x + circleButton->rect.width * 0.2f, size.y / 2.0f });
		if (layout != nullptr)
		{
			layout->SetPosition({ newPosition.x + circleButton->rect.width * 0.9f , newPosition.y });
		}

	}
	virtual void SetSize(const sf::Vector2f &newSize)
	{
		size = { static_cast<unsigned int>(newSize.x), static_cast<unsigned int>(newSize.y) };
		rect.width = newSize.x;
		rect.height = newSize.y;
		shape.setSize(newSize);
		circleButton->SetSize({ 50.0f, 50.0f });
		if (layout != nullptr)
		{
			layout->size.x = size.x - static_cast<unsigned int>(circleButton->rect.width * 0.9f);
			layout->size.y = size.y;
			layout->Resize();
		}

	}
	void Add(Layout *newLayout)
	{
		layout = newLayout;
		layout->size.x = size.x - static_cast<unsigned int>(circleButton->rect.width * 0.9f);
		layout->size.y = size.y;
		layout->Resize();
		SetPosition(position);
	}
	void Resize() {
		SetSize({ static_cast<float>(size.x), static_cast<float>(size.y) });
		if (layout != nullptr)
		{
			layout->Resize();
		}
	}
	void Animation()
	{
		if (hidden)
		{
			setHidden = true;
			SetPosition({ position.x - size.x + circleButton->rect.width * 0.9f, position.y });
			setHidden = false;
			hidden = false;
		}
		else
		{
			setHidden = true;
			SetPosition({ position.x + size.x - circleButton->rect.width * 0.9f, position.y });
			setHidden = false;
			hidden = true;
		}
	}
	virtual void Disable() override
	{
		layout->Disable();
		circleButton->Disable();
		GUI::Disable();
	}
	virtual void Enable() override
	{
		layout->Enable();
		circleButton->Enable();
		GUI::Enable();
	}
	void BlankEvent() { throw 0; }
protected:
	Layout *layout;
	sf::RectangleShape shape;
	GUI *circleButton;
	bool hidden = false;
	bool setHidden = false;
};