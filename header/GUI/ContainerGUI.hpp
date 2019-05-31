#pragma once
#include "GUI.hpp"

class ContainerGUI : public Layout
{
public:
	ContainerGUI(sf::Vector2u newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}

	virtual ~ContainerGUI()
	{
		for (auto &element : elements)
		{
			delete element;
		}
	}

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
		elementSize.push_back({0.0f, 70.0f});
		hidden.push_back(false);
		elementAlign.push_back(Align::Center);
		percent.push_back({ 100.0f, 100.0f });
		count++;
		Resize();
		SetPosition(position);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		position = newPosition;
		int i = 0;
		float toTop = 0.0f, toCentre = 0.0f, toBottom = 0.0f;
		for (auto element : elements)
		{
			if (!hidden[i])
			{
				switch (elementAlign[i])
				{
				case Align::Top:
					toTop += elementSize[i].y + space;
					break;
				case Align::Center:
					toCentre += elementSize[i].y + space;
					break;
				case Align::Bottom:
					toBottom += elementSize[i].y + space;
					break;
				}
			}
			++i;
		}
		toCentre = (static_cast<float>(size.y) - toBottom - toTop - toCentre) / 2.0f + toTop;
		toBottom = (static_cast<float>(size.y) - toBottom);

		float sizeToSkip;
		Align align;
		i = 0;
		for (auto element : elements)
		{
			align = elementAlign[i];
			sizeToSkip = 0;
			for (int k = 0; k < i; ++k)
			{
				if (!hidden[k] && elementAlign[k] == align)
				{
					sizeToSkip += elementSize[k].y + space;
				}
			}
			switch (align)
			{
			case Align::Top:
				element->SetPosition({ newPosition.x + (size.x - element->size.x) / 2.0f, newPosition.y + padding + sizeToSkip});
				break;
			case Align::Center:
				element->SetPosition({ newPosition.x + (size.x- element->size.x) / 2.0f, newPosition.y + padding + sizeToSkip + toCentre });
				break;
			case Align::Bottom:
				element->SetPosition({ newPosition.x + (size.x - element->size.x) / 2.0f, newPosition.y + padding + sizeToSkip + toBottom });
				break;
			}
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
			if (elementSize[i].x > 0)
			{
				element->SetSize({ elementSize[i].x - 2 * padding, elementSize[i].y - 2 * padding});
			}
			else
			{
				element->SetSize({ (static_cast<float>(size.x) - 2 * padding) * percent[i].x / 100.0f, elementSize[i].y - 2 * padding});
			}
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
	void SetElementSizeX(int elementSet, float newSizeY)
	{
		--elementSet;
		if (elementSet >= 0)
		{
			elementSize[elementSet].x = newSizeY;
		}
		Resize();
		SetPosition(position);
	}
	void SetPercentX(int index, float newPercentX)
	{
		--index;
		percent[index].x = newPercentX;
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
	void SetTop(int element)
	{
		--element;
		elementAlign[element] = Align::Top;
		SetPosition(position);
	}
	void SetBottom(int element)
	{
		--element;
		elementAlign[element] = Align::Bottom;
		SetPosition(position);
	}

protected:
	enum Align
	{
		Top,
		Center,
		Bottom
	};
	float count = 0;
	float padding = 0;
	float space = 0;
	bool center = true;
	std::vector<bool> hidden;
	std::vector<sf::Vector2f> elementSize;
	std::vector<sf::Vector2f> percent;
	std::vector<Layout*> elements;
	std::vector<Align> elementAlign;
};