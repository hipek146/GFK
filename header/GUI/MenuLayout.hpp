#pragma once
#include "GUI.hpp"


class MenuLayout : public Layout
{
public:
	MenuLayout(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}
	~MenuLayout()
	{
		for (auto &element : elements)
		{
			delete element;
			element = nullptr;
		}
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{

		for (auto &element : elements)
		{
			target.draw(*element);
		}
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		sf::Vector2f tmpPosition = newPosition;
		tmpPosition.x -= position.x;
		tmpPosition.y -= position.y;
		for (auto &element : elements)
		{
			element->SetPosition({ element->rect.left + (element->rect.width) / 2.0f + tmpPosition.x, element->rect.top + (element->rect.height) / 2.0f + tmpPosition.y });
		}
		position = newPosition;
	}
	void Add(GUI *newElement)
	{
		length++;

		sf::Vector2f newPosition, newSize;

		float min = 0.4f - 0.25f * (1 - 300 / size.y), lvl = 30, power = 2;
		if (size.y < 300)
		{
			min = 0.5f;
		}

		newSize.y = size.y * (1 - (1 - min)*(lvl + 1) / (lvl + pow(length, power))) / length;

		const float minX = 300.0f, powerX = 1.2f;
		newSize.x = size.x *(0.1f + 0.8f * (pow(minX, powerX) / pow(static_cast<float>(size.x), powerX)));

		if (!elements.empty())
		{
			Resize();

			newPosition.x = size.x / 2.0f + position.x;
			newPosition.y += elements.back()->rect.top + 1.5f * elements.back()->rect.height + 0.1f * newSize.y + position.y;
		}
		else
		{
			newPosition = { size.x / 2.0f + position.x, size.y / 2.0f + position.y };
		}
		elements.push_back(newElement);
		elements.back()->SetPosition(newPosition);
		elements.back()->SetSize({ newSize.x, newSize.y * 0.9f });
	}
	template <typename T>
	void Add(GUI *newElement, T *screen, void(T::*callback)())
	{
		Add(newElement, screen);
		screen->AddEvent(&elements.back()->rect, screen, newElement, callback, EventType::Pressed);
	}
	template <typename T>
	void Add(GUI *newElement, T *screen)
	{
		Add(newElement);
		elements.back()->parent = screen;
		screen->AddEvent(&elements.back()->rect, newElement, newElement, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&elements.back()->rect, newElement, newElement, &GUI::MouseOut, EventType::MouseOut);
	}
	virtual void Resize() override
	{
		sf::Vector2f newPosition, newSize;

		float min = 0.4f - 0.25f*(1 - 300 / size.y), lvl = 30, power = 2;
		if (size.y < 300)
		{
			min = 0.5f;
		}
		newSize.y = size.y * (1 - (1 - min)*(lvl + 1) / (lvl + pow(length, power))) / length;
		const float minX = 300.0f, powerX = 1.2f;
		float sizeX = static_cast<float>(size.x);
		if (size.x < 300)
		{
			sizeX = 300.0f;
		}
		newSize.x = size.x *(0.1f + 0.8f * (pow(minX, powerX) / pow(sizeX, powerX)));

		if (!elements.empty())
		{
			int i = 0;
			for (auto element : elements)
			{
				element->SetSize({ newSize.x, newSize.y * 0.9f });
				element->SetPosition({ size.x / 2.0f + position.x,  size.y / 2.0f - (static_cast<int>(length / 2.0f) - i) * newSize.y + (1 - static_cast<int>(length) % 2) * newSize.y / 2.0f + position.y });
				i++;
			}
		}
	}
	virtual void Disable() override
	{

		for (auto &element : elements)
		{
			element->Disable();
		}
	}
	virtual void Enable() override
	{

		for (auto &element : elements)
		{
			element->Enable();
		}
	}

private:
	std::vector<GUI*> elements;
	float length = 0;
};