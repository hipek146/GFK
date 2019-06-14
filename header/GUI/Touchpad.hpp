#pragma once
#include "GUI.hpp"

class Touchpad : public GUI, public sf::Transformable
{
public:
	Touchpad(Screen *setParent, sf::Vector2u *newSize)
	{
		parent = setParent;
		size = newSize;
		SetSize({ static_cast<float>(newSize->x), static_cast<float>(newSize->y) });
	}
	virtual ~Touchpad() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		rect.width = newSize.x;
		rect.height = newSize.y;
	}
	virtual void Resize()
	{
		rect.width = static_cast<float>(size->x);
		rect.height = static_cast<float>(size->y);
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
	template <typename T>
	void onReleased(T *screen, void(T::*callback)())
	{
		screen->AddEvent(&(this->rect), screen, this, callback, EventType::Released);
	}
	
protected:
	sf::Vector2u *size;
};