#pragma once
#include "SFML/Graphics.hpp"

class GUI;

class Layout : virtual public sf::Drawable
{
public:
	virtual ~Layout() = default;
	Layout() = default;
	Layout(sf::Vector2u &newSize, sf::Vector2f newPosition = {})
	{
		size = newSize;
		SetPosition(newPosition);
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	virtual void SetPosition(const sf::Vector2f &newPosition)
	{
		position = newPosition;
	}
	virtual void SetSize(const sf::Vector2f &newSize)
	{
		size = { static_cast<unsigned int>(newSize.x), static_cast<unsigned int>(newSize.y) };
		Resize();
	}
	virtual void Callback(GUI* element) {};
	virtual void Resize() {};
	virtual void Disable() {};
	virtual void Enable() {};

	friend class MenuLayout;
	friend class CreatorLayout;
	friend class CreatorTabs;
	friend class TabsLayout;
	friend class ContainerGUI;

protected:
	sf::Vector2u size;
	sf::Vector2f position;
};