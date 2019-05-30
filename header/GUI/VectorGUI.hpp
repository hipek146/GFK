#pragma once
#include "GUI.hpp"

class VectorGUI : public Layout
{
public:
	VectorGUI(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}
	~VectorGUI()
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

	template <typename T>
	void Add(GUI *newElement, T *screen, void(T::*callback)())
	{
		elements.push_back(newElement);
		screen->AddEvent(&newElement->rect, screen, newElement, callback, EventType::Pressed);
		screen->AddEvent(&newElement->rect, newElement, newElement, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&newElement->rect, newElement, newElement, &GUI::MouseOut, EventType::MouseOut);
		screen->AddEvent(&newElement->rect, newElement, newElement, &GUI::Callback, EventType::Pressed);
		newElement->SetEvents(screen);
		newElement->callbackHandle = std::bind(callback, screen);
		newElement->parent = screen;
		newElement->parentLayout = this;
		count++;
		Resize();
		SetPosition(position);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		position = newPosition;
		int i = 0;
		for (auto element : elements)
		{
			if (horizontal)
			{
				element->SetPosition({ newPosition.x + padding + elementSize.x / 2.0f + (elementSize.x + space) * i, newPosition.y + padding + elementSize.y / 2.0f });
			}
			else
			{
				element->SetPosition({ newPosition.x + padding + elementSize.x / 2.0f, newPosition.y + padding + elementSize.y / 2.0f + (elementSize.y + space) * i });
			}
			++i;
		}
	}
	virtual void Resize() override
	{
		if (horizontal)
		{
			if (count > 0)
			{
				elementSize.x = (size.x - 2 * padding - (count - 1) * space) / count;
			}
			elementSize.y = size.y - 2 * padding;
		}
		else
		{
			if (count > 0)
			{
				elementSize.y = (size.y - 2 * padding - (count - 1) * space) / count;
			}
			elementSize.x = size.x - 2 * padding;
		}
		for (auto element : elements)
		{
			element->SetSize({ elementSize.x, elementSize.y });
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

	void SetSpace(float newSpace)
	{
		space = newSpace;
		Resize();
		SetPosition(position);
	}

	void SetPadding(float newPadding)
	{
		padding = newPadding;
		Resize();
		SetPosition(position);
	}
	void EnableOneChoice(int newActive = 0)
	{
		--newActive;
		oneChoice = true;
		if (newActive >= 0 && newActive < count)
		{
			active = newActive;
			Callback(elements[active]);
		}
	}
	void DisableOneChoice()
	{
		oneChoice = false;
	}
	virtual void Callback(GUI* elementCallback)
	{
		if (!oneChoice)
		{
			return;
		}
		int i = 0;
		for (auto element : elements)
		{
			if (element == elementCallback)
			{
				if (active != i && active >= 0)
				{
					elements[active]->Inactive();
				}
				element->Active();
				active = i;
			}
			++i;
		}
	}
	void Active(int newActive)
	{
		--newActive;
		if (oneChoice)
		{
			elements[active]->Inactive();
		}
		elements[newActive]->Active();
	}
	void Inactive(int newInactive)
	{
		--newInactive;
		if (!oneChoice)
		{
			elements[newInactive]->Inactive();
		}
	}
	void Horizontal()
	{
		horizontal = true;
		Resize();
		SetPosition(position);
	}
	void Vertical()
	{
		horizontal = false;
		Resize();
		SetPosition(position);
	}

private:
	int active = -1;
	float count = 0;
	float padding = 0;
	float space = 0;
	sf::Vector2f elementSize;
	std::vector<GUI*> elements;

	bool oneChoice = false;
	bool horizontal = true;

};