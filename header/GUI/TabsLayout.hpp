#pragma once
#include "GUI.hpp"

class TabsLayout : public Layout
{
public:
	TabsLayout(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) 
	{
		connectShape.setFillColor(bgColor);
		shape.setFillColor(bgColor);
	}
	~TabsLayout()
	{
		for (auto button : buttons)
		{
			delete button;
		}
		for (auto layout : layouts)
		{
			delete layout;
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(connectShape);
		target.draw(shape);
		if (active >= 0)
		{
			for (auto button : buttons)
			{
				target.draw(*button);
			}
			target.draw(*layouts[active]);
		}
	}
	template <typename T>
	void Add(GUI *newButton, T* screen, Layout* newLayout)
	{
		buttons.push_back(newButton);
		layouts.push_back(newLayout);
		++count;
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::MouseOut, EventType::MouseOut);
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::Callback, EventType::Pressed);
		newButton->parentLayout = this;
		newButton->parent = screen;
		newButton->SetColor(color);
		newButton->SetActiveColor(activeColor);
		Resize();
		if (count > 1)
		{
			newLayout->Disable();
		}
		else
		{
			SetActive(0);
		}
		SetPosition(position);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		position = newPosition;
		float i = 0;
		for (auto button : buttons)
		{
			button->SetPosition({ newPosition.x + buttonSize.x / 2.0f + buttonSize.x * i, newPosition.y + buttonSize.y / 2.0f });
			++i;
		}
		for (auto layout : layouts)
		{
			layout->SetPosition({ newPosition.x,  newPosition.y + buttonSize.y * 1.1f });
		}
		connectShape.setPosition(newPosition.x + active * buttonSize.x, newPosition.y + buttonSize.y * 0.9f);
		shape.setPosition(newPosition.x, newPosition.y + buttonSize.y * 1.1f);
	}
	virtual void Resize() override
	{
		if (count > 0)
		{
			buttonSize.x = static_cast<float>(size.x) / count;
		}
		buttonSize.y = 60.0f;
		for (auto button : buttons)
		{
			button->SetSize({ buttonSize.x, buttonSize.y * 0.9f });
		}
		for (auto layout : layouts)
		{
			layout->size.x = size.x;
			layout->size.y = size.y - static_cast<unsigned int>(buttonSize.y * 1.2f);
			layout->Resize();
		}
		connectShape.setSize({ buttonSize.x, buttonSize.y * 0.2f });
		shape.setSize({ static_cast<float>(layouts[active]->size.x), static_cast<float>(layouts[active]->size.y) });
	}
	virtual void Callback(GUI *element) override
	{
		int i = 0;
		for (auto button : buttons)
		{
			if (button == element)
			{
				SetActive(i);
				break;
			}
			++i;
		}
	}
	void SetActive(int number)
	{
		layouts[active]->Disable();
		buttons[active]->SetColor(color);
		buttons[active]->SetActiveColor(activeColor);
		active = number;
		layouts[active]->Enable();
		buttons[active]->SetColor(activeColor);
		buttons[active]->SetActiveColor(activeColor);
		SetPosition(position);
	}
	virtual void Disable() override
	{
		for (auto &element : buttons)
		{
			element->Disable();
		}
		for (auto &element : layouts)
		{
			element->Disable();
		}
	}
	virtual void Enable() override
	{

		for (auto &element : buttons)
		{
			element->Enable();
		}
		layouts[active]->Enable();
	}
private:
	sf::RectangleShape shape;
	sf::RectangleShape connectShape;
	sf::Color bgColor = { 36, 38, 59, 100 };
	sf::Color color = { 214, 206, 170, 230 };
	sf::Color activeColor = { 123, 189, 102, 245 };
	int active = 0;
	float count = 0;
	std::vector<GUI*> buttons;
	std::vector<Layout*> layouts;
	sf::Vector2f buttonSize;
};
