#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "Config.hpp"
#include "Event.hpp"
#include "App.hpp"
#include <stdlib.h>
#include "SFML/Graphics.hpp"


int WinMain()
{
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	Config config(L"config.ini");
	srand(time(NULL));

	sf::ContextSettings settings;
	settings.antialiasingLevel = config(L"antialiasing");

	sf::RenderWindow window(sf::VideoMode(config(L"width"), config(L"height")), L"Morfologia Terenu", sf::Style::Default, settings);
	sf::View view;
	view.reset(sf::FloatRect(0.0f, 0.0f, (float)(double)config(L"width"), (float)(double)config(L"height")));
	view.zoom(2.0f);
	//window.setView(view);
	

	window.setFramerateLimit(60);

	Event event(window);
	App app(&event, &config);


	while (window.isOpen())
	{
		window.clear(sf::Color::White);

		while (window.pollEvent(event))//wywołany rzutowanie z obiektu Event na sf::Event ?
		{
			event();// zamiast switcha
		}

 		window.draw(app);
		window.display();
	}
	return 0;
}

int main()
{
	return WinMain();
}
