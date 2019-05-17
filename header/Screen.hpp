#pragma once
#include "App.hpp"

class Screen : public sf::Drawable
{
public:
	Screen() = default;
	Screen(App *parent) { app = parent; size = app->size; }

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	virtual void CreateScreen() = 0;
	virtual void ClearScreen() = 0;
	virtual void ResizeScreen() = 0;

	template <typename T>
	void AddEvent(sf::FloatRect *rect, T *screen, GUI *element, void(T::*callback)(), int type)
	{
		app->event->Add(rect, screen, element, callback, type);
	}

	void RemoveEvent(GUI *element)
	{
		app->event->Remove(element);
	}

	sf::Vector2u *size;
protected:
	App *app;
};