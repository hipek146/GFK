#include "Menu.hpp"
#include "Dictionary.hpp"
#include <iostream>

Menu::Menu(App *parent) : Screen(parent) {}

Menu::~Menu()
{
	ClearScreen();
}

void Menu::Pressed(MenuPressed pressed)
{
	switch (pressed)
	{
	case MenuPressed::Creator:
		app->LoadScreen(app->creator);
		break;
	case MenuPressed::Exit:
		app->event->window->close();
		break;
	}
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*background);
	target.draw(*layout);
}

void Menu::CreateScreen()
{
	background = new sf::Sprite;
	backgroundTexture = new sf::Texture();
	if (!backgroundTexture->loadFromFile("GFK/resource/images/menu-background.png"))
	{
		throw "Cannot load menu background image";
	}
	background->setTexture(*backgroundTexture);

	ResizeScreen();
	
	layout = new MenuLayout(size);
	layout->Add(new Button(L"Game"));
	layout->ADD(new Button(L"Creator"), &Menu::PressedCreator);
	layout->Add(new Button(L"Options"));
	layout->ADD(new Button(L"Exit"), &Menu::PressedExit);

}

void Menu::ClearScreen()
{
	delete layout;
	layout = nullptr;

	delete background;
	background = nullptr;

	delete backgroundTexture;
	backgroundTexture = nullptr;
}

//Dopasowuje tło do okna 
void Menu::ResizeScreen()
{
	
	float backgroundSizeX = static_cast<float>(backgroundTexture->getSize().x);
	float backgroundSizeY = static_cast<float>(backgroundTexture->getSize().y);
	float ratio = (size->x / static_cast<float>(size->y) > backgroundSizeX / backgroundSizeY) ? (size->x / backgroundSizeX) : (size->y / backgroundSizeY);
	background->setScale(ratio, ratio);
	background->setPosition((size->x - ratio * backgroundSizeX) / 2.0f, (size->y - ratio * backgroundSizeY) / 2.0f);

	if (layout != nullptr)
	{
		layout->Resize();
	}
}