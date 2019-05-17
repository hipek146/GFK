#include "App.hpp"
#include "Menu.hpp"
#include "Creator.hpp"

App::App(Event *event) : event(event)
{
	size = &event->size;

	event->Add(nullptr, this, nullptr, &App::Resize, EventType::Resize);

	menu = new Menu(this);
	creator = new Creator(this);
	LoadScreen(menu);
}

App::~App()
{
	delete menu;
	delete creator;
}

void App::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*screen);
}

void App::LoadScreen(Screen *screenToLoaded)
{
	if (screen != nullptr)
	{
		screen->ClearScreen();
	}
	screen = screenToLoaded;
	screen->CreateScreen();
}

void App::Resize()
{
	screen->ResizeScreen();
}
