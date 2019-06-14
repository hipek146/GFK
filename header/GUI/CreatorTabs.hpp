#pragma once
#include "GUI.hpp"

class CreatorTabs : public Layout
{
public:
	CreatorTabs(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}
	~CreatorTabs()
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
	void Add(GUI *newButton, T* screen, Layout* newLayout, void(T::*callback)())
	{
		buttons.push_back(newButton);
		layouts.push_back(newLayout);
		++count;
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::MouseOut, EventType::MouseOut);
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::Callback, EventType::Pressed);
		newButton->SetEvents(screen);
		newButton->callbackHandle = std::bind(callback, screen);
		newButton->parentLayout = this;
		newButton->parent = screen;
		newButton->SetColor(color);
		newButton->SetActiveColor(activeColor);
		newButton->SetActiveTextColor(sf::Color(59, 59, 59));
		//callbacksTabs = std::bind(callback, screen);
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
			if (i <= active)
			{
				button->SetPosition({ newPosition.x + size.x / 2.0f, newPosition.y + buttonSize.y / 2.0f + buttonSize.y * i });
			}
			else
			{
				button->SetPosition({ newPosition.x + size.x / 2.0f, newPosition.y + buttonSize.y / 2.0f + buttonSize.y * i + layouts[active]->size.y });
			}
			++i;
		}
		for (auto layout : layouts)
		{
			layout->SetPosition({ newPosition.x + size.x * 0.02f,  newPosition.y + (active + 1) * buttonSize.y });
		}
	}
	virtual void Resize() override
	{
		buttonSize.x = static_cast<float>(size.x) * 0.98f;
		buttonSize.y = 42.0f;
		for (auto button : buttons)
		{
			button->SetSize({ buttonSize.x, buttonSize.y * 0.9f });
		}
		for (auto layout : layouts)
		{
			layout->size.x = static_cast<unsigned int>(size.x * 0.96f);
			layout->size.y = size.y - static_cast<unsigned int>(count * buttonSize.y);
			layout->Resize();
		}
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
		buttons[active]->SetTextColor(sf::Color::White);
		buttons[active]->SetActiveTextColor(sf::Color(59, 59, 59));
		active = number;
		layouts[active]->Enable();
		buttons[active]->SetColor(activeColor);
		buttons[active]->SetActiveColor(activeColor);
		buttons[active]->SetTextColor(sf::Color(59, 59, 59));
		buttons[active]->SetActiveTextColor(sf::Color(59, 59, 59));
		SetPosition(position);
	}
private:
	int active = 0;
	float count = 0;
	std::vector<GUI*> buttons;
	std::vector<Layout*> layouts;
	sf::Vector2f buttonSize;
	sf::Color color = { 121, 190, 242, 220 };
	sf::Color activeColor = { 248, 246, 41, 245 };
	std::vector<std::function<void()>> callbacksTabs;
};