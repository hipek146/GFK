#include "App.hpp"
#include "Menu.hpp"
#include "Creator.hpp"

App::App(Event *event) : event(event)
{
	size = &event->size;

	event->Add(nullptr, this, nullptr, &App::Resize, EventType::Resize); //dodaje zdarzenie rezise dla aplikacji 

	menu = new Menu(this); //tworzy okno menu aplikacji, korzysta z dziedziczenia Screen 
	creator = new Creator(this);//tworzy okno kreatora aplikacji, korzysta z dziedziczenia Screen 
	LoadScreen(menu);//laduje okno menu 
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
	screen->CreateScreen(); //Tutaj zaczyna tworzyc sie ekran 
}

void App::Resize()
{
	screen->ResizeScreen();
}
