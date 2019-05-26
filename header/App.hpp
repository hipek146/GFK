#pragma once
#include "Event.hpp"
#include "SFML/Graphics.hpp"

class Screen;

class App : public sf::Drawable
{
public:
	App(Event *event);
	~App();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void LoadScreen(Screen *screenToLoaded);
	void Resize();

	friend class Screen;
	friend class Menu;
	friend class Creator;

	sf::Vector2u *size;
	Event *event;

private:
	Screen *menu;
	Screen *creator;
	
	Screen *screen;
};