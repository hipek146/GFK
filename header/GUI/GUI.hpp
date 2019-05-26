#pragma once
#include "SFML/Graphics.hpp"
#include "Event.hpp"
#include "Screen.hpp"

#define ADD(newElement, callback) Add(newElement, this, callback)
#define ADD_BLANK(newElement) Add(newElement, this)

#include "Layout.hpp"

class GUI : virtual public sf::Drawable
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

	virtual void Callback()
	{
		if (parentLayout != nullptr)
		{
			parentLayout->Callback(this);
		}
	}

	virtual void SetEvents(Screen *screen) {}
	virtual void MouseOver() {}
	virtual void MouseOut() {}

	virtual void Active() {}
	virtual void Inactive() {}

	virtual void SetColor(sf::Color newColor) {}
	virtual void SetActiveColor(sf::Color newColor) {}
	virtual void SetTextColor(sf::Color newColor) {}
	virtual void SetActiveTextColor(sf::Color newColor) {}

	void Disable()
	{
		if (parent != nullptr)
		{
			parent->DisableEvent(this);
		}
	}
	void Enable()
	{
		if (parent != nullptr)
		{
			parent->EnableEvent(this);
		}
	}
	friend class MenuLayout;
	friend class CreatorLayout;
	friend class CreatorTabs;
	friend class TabsLayout;
	friend class Button;
	friend class VectorGUI;

	Layout *parentLayout;
protected:
	Screen *parent;

	sf::FloatRect rect;
	std::function<void()> callbackHandle;

	void(*callback)();
};

#include "Button.hpp"

#include "Checkbox.hpp"

#include "TextInput.hpp"

#include "ScrollBar.hpp"

#include "CircleButton.hpp"

#include "MenuLayout.hpp"

#include "CreatorLayout.hpp"

#include "CreatorTabs.hpp"

#include "TabsLayout.hpp"

#include "VectorGUI.hpp"

#include "ContainerGUI.hpp"

#include "Touchpad.hpp"