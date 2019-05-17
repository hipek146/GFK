#pragma once
#include "SFML/Graphics.hpp"
#include "Event.hpp"
#include "Screen.hpp"

#define ADD(newElement, callback) Add(newElement, this, callback)
#define ADD_BLANK(newElement) Add(newElement, this)

class GUI : public sf::Drawable
{
public:

	virtual ~GUI()
	{
		if (parent != nullptr)
		{
			parent->RemoveEvent(this);
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	virtual void SetSize(const sf::Vector2f &newSize) = 0;
	virtual void SetPosition(const sf::Vector2f &newPosition) = 0;

	virtual void MouseOver() {}
	virtual void MouseOut() {}

	friend class MenuLayout;
	friend class Button;

protected:
	Screen *parent;

	sf::FloatRect rect;

	void(*callback)();
};

class Button : public GUI
{
public:
	Button(sf::String string)
	{
		if (!font.loadFromFile("resource/fonts/SimsLLHP.ttf"))
		{
			throw L"Cannot open font file";
		}
		text.setString(string);
		text.setFont(font);
		text.setFillColor(sf::Color::White);

		shape.setFillColor(sf::Color(12, 154, 220, 210));
	}
	~Button()
	{
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
		target.draw(text);
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		sf::Vector2f oldSize(rect.width, rect.height);
		rect.width = newSize.x;
		rect.height = newSize.y;

		shape.setSize(newSize);

		text.setCharacterSize(static_cast<unsigned int>(newSize.y * 0.6f));

		SetPosition({rect.left + ( oldSize.x) / 2.0f, rect.top + (oldSize.y) / 2.0f});
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x - rect.width / 2.0f;
		rect.top = newPosition.y - rect.height / 2.0f;

		shape.setPosition(rect.left, rect.top);
		sf::FloatRect textSize = text.getLocalBounds();
		text.setPosition(newPosition.x - textSize.width / 2.0f, newPosition.y - textSize.height / 2.0f - textSize.top);
	}
	virtual void MouseOver() override
	{
		shape.setFillColor(sf::Color(38, 89, 111, 240));
	}
	virtual void MouseOut() override
	{
		shape.setFillColor(sf::Color(12, 154, 220, 210));
	}
private:
	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;
};

class MenuLayout : public sf::Drawable
{
public:
	MenuLayout(sf::Vector2u *newSize, sf::Vector2f *newPosition = nullptr)
	{
		size = newSize;
		if (newPosition != nullptr)
		{
			SetPosition(*newPosition);
		}
	}
	~MenuLayout()
	{
		for (auto &element : elements)
		{
			delete element;
		}
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{

		for (auto &element : elements)
		{
			target.draw(*element);
		}
	}
	void SetPosition(sf::Vector2f newPosition)
	{
		newPosition.x -= position.x;
		newPosition.y -= position.y;
		for (auto &element : elements)
		{
			element->SetPosition({ element->rect.left + (element->rect.width) / 2.0f + newPosition.x, element->rect.top + (element->rect.height) / 2.0f + newPosition.y });
		}
		position = newPosition;
	}
	void Add(GUI *newElement)
	{
		length++;

		sf::Vector2f newPosition, newSize;

		float min = 0.4f - 0.25f * (1 - 300 / size->y), lvl = 30, power = 2;
		if (size->y < 300)
		{
			min = 0.5f;
		}

		newSize.y = size->y * (1 - (1 - min)*(lvl + 1) / (lvl + pow(length, power))) / length;

		const float minX = 300.0f, powerX = 1.2f;
		newSize.x = size->x *(0.1f + 0.8f * (pow(minX, powerX) / pow(static_cast<float>(size->x), powerX)));

		if (!elements.empty())
		{
			Resize();

			newPosition.x = size->x / 2.0f + position.x;
			newPosition.y += elements.back()->rect.top + 1.5f * elements.back()->rect.height + 0.1f * newSize.y + position.y;
		}
		else
		{
			newPosition = { size->x / 2.0f + position.x, size->y / 2.0f + position.y};
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
	void Resize()
	{
		sf::Vector2f newPosition, newSize;

		float min = 0.4f - 0.25f*(1-300/size->y), lvl = 30, power = 2;
		if (size->y < 300)
		{
			min = 0.5f;
		}
		newSize.y = size->y * (1 - (1 - min)*(lvl + 1) / (lvl + pow(length, power))) / length;
		const float minX = 300.0f, powerX = 1.2f;
		newSize.x = size->x *(0.1f + 0.8f * (pow(minX, powerX) / pow(static_cast<float>(size->x), powerX)));

		if (!elements.empty())
		{
			int i = 0;
			for (auto element : elements)
			{
				element->SetSize({ newSize.x, newSize.y * 0.9f });
				element->SetPosition({ size->x / 2.0f + position.x,  size->y / 2.0f - (static_cast<int>(length / 2.0f) - i) * newSize.y + (1 - static_cast<int>(length) % 2) * newSize.y / 2.0f + position.y});
				i++;
			}
		}
	}

private:
	sf::Vector2u *size;
	sf::Vector2f position;
	std::vector<GUI*> elements;
	float length = 0;
};

class Touchpad : public GUI, public sf::Transformable
{
public:
	Touchpad(sf::Vector2u *newSize)
	{
		SetSize({ static_cast<float>(newSize->x), static_cast<float>(newSize->y) });
	}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		rect.width = newSize.x;
		rect.height = newSize.y;
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x;
		rect.top = newPosition.y;
		setPosition(newPosition);
	}
	template <typename T>
	void onMouseMove(T *screen, void(T::*callback)())
	{
		screen->AddEvent(&(this->rect), screen, this, callback, EventType::MouseMove);
	}
	template <typename T>
	void onClick(T *screen, void(T::*callback)())
	{
		screen->AddEvent(&(this->rect), screen, this, callback, EventType::Pressed);
	}
};