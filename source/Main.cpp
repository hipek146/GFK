#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Config.hpp"
#include "Event.hpp"
#include "App.hpp"

#include "SFML/Graphics.hpp"


//#define main WinMain


int main()
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Config config;
	
	sf::RenderWindow window(sf::VideoMode(config(L"width"), config(L"height")), L"Tytuł");
	window.setFramerateLimit(60);

	Event event(window);
	App app(&event);


	while (window.isOpen())
	{
		window.clear(sf::Color::White);

		while (window.pollEvent(event))
		{
			event();
		}

		window.draw(app);
		window.display();
	}
	return 0;
}