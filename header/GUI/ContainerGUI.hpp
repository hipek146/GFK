#pragma once
#include "GUI.hpp"

class ContainerGUI : public Layout
{
public:
	ContainerGUI(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		int i = 0;
		for (auto &element : elements)
		{
			if (!hidden[i])
			{
				target.draw(*element);
			}
			++i;
		}
	}

	template <typename T>
	void Add(T *screen, Layout *newElement)
	{
		elements.push_back(newElement);
		elementSize.push_back({static_cast<float>(size.x), 70.0f});
		hidden.push_back(false);
		count++;
		Resize();
		SetPosition(position);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		position = newPosition;
		int i = 0;
		float toCentre = 0.0f;
		for (auto element : elements)
		{
			if (!hidden[i])
			{
				toCentre += elementSize[i].y + space;
			}
			++i;
		}
		toCentre = (static_cast<float>(size.y) - toCentre) / 2.0f;
		float sizeToSkip;
		i = 0;
		for (auto element : elements)
		{
			sizeToSkip = 0;
			for (int k = 0; k < i; ++k)
			{
				if (!hidden[k])
				{
					sizeToSkip += elementSize[k].y + space;
				}
			}
			element->SetPosition({ newPosition.x + padding, newPosition.y + padding + sizeToSkip + toCentre});
			++i;
		}
	}
	virtual void Resize() override
	{
		/*if (count > 0)
		{
			elementSize.y = (size.y - 2 * padding - (count - 1) * space) / count;
			if (elementSize.y > 50)
			{
				elementSize.y = 70;
			}
		}
		elementSize.x = size.x - 2 * padding;*/
		int i = 0;
		for (auto element : elements)
		{
			elementSize[i].x = size.x;
			element->SetSize({ elementSize[i].x, elementSize[i].y });
			++i;
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
		int i = 0;
		for (auto &element : elements)
		{
			if (!hidden[i])
			{
				element->Enable();
			}
			++i;
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
	void SetElementSizeY(int elementSet, float newSizeY)
	{
		--elementSet;
		if (elementSet >= 0)
		{
			elementSize[elementSet].y = newSizeY;
		}
		Resize();
		SetPosition(position);
	}
	void Hide(int element)
	{
		--element;
		hidden[element] = true;
		elements[element]->Disable();
		SetPosition(position);
	}
	void Show(int element)
	{
		--element;
		hidden[element] = false;
		elements[element]->Enable();
		SetPosition(position);
	}
private:
	float count = 0;
	float padding = 0;
	float space = 0;
	bool center = true;
	std::vector<bool> hidden;
	std::vector<sf::Vector2f> elementSize;
	std::vector<Layout*> elements;
};