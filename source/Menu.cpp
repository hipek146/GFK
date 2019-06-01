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
	case MenuPressed::Load:
		app->LoadDialog();
		break;
	case MenuPressed::Creator:
		app->LoadScreen(app->creator);
		break;
	case MenuPressed::Options:
		OptionsDialog();
		break;
	case MenuPressed::Exit:
		app->event->window->close();
		break;
	}
}

void Menu::OptionsDialog()
{
	options = new DialogBox(this, L"Opcje", { sizeDialog }, { static_cast<float>(size->x - sizeDialog.x) / 2.0f, static_cast<float>(size->y - sizeDialog.y) / 2.0f });
	
	VectorGUI *textWidth = new VectorGUI(sizeDialog);
	textWidth->Add(new TextBox(L"Szerokosc okna"), this, &Menu::Blank);
	options->Add(this, textWidth);
	options->SetTop(1);

	VectorGUI *inputWidth = new VectorGUI(sizeDialog);
	textInputWidth = new TextInput((*app->config)(L"width"), true);
	inputWidth->Add(textInputWidth, this, &Menu::Blank);
	options->Add(this, inputWidth);
	options->SetTop(2);

	VectorGUI *textHeight = new VectorGUI(sizeDialog);
	textHeight->Add(new TextBox(L"Wysokosc okna"), this, &Menu::Blank);
	options->Add(this, textHeight);
	options->SetTop(3);

	VectorGUI *inputHeight = new VectorGUI(sizeDialog);
	textInputHeight = new TextInput((*app->config)(L"height"), true);
	inputHeight->Add(textInputHeight, this, &Menu::Blank);
	options->Add(this, inputHeight);
	options->SetTop(4);

	VectorGUI *textAntyaliasing = new VectorGUI(sizeDialog);
	textAntyaliasing->Add(new TextBox(L"Antyaliasing"), this, &Menu::Blank);
	options->Add(this, textAntyaliasing);

	VectorGUI *antyaliasing = new VectorGUI(sizeDialog);
	antyaliasing->Add(new Checkbox(L"Off"), this, &Menu::OptionsAntyaliasingOFF);
	antyaliasing->Add(new Checkbox(L"x4"), this, &Menu::OptionsAntyaliasing4x);
	antyaliasing->Add(new Checkbox(L"x8"), this, &Menu::OptionsAntyaliasing8x);
	antyaliasing->Vertical();
	antyaliasing->EnableOneChoice();
	switch(static_cast<int>((*app->config)(L"antialiasing")))
	{
	case 0:
		antyaliasing->Active(1);
		OptionsAntyaliasingOFF();
		break;
	case 4:
		antyaliasing->Active(2);
		OptionsAntyaliasing4x();
		break;
	case 8:
		antyaliasing->Active(3);
		OptionsAntyaliasing8x();
		break;
	default:
		antyaliasing->Active(1);
		OptionsAntyaliasingOFF();
		break;
	}
	options->Add(this, antyaliasing);
	options->SetElementSizeX(6, 100.0f);
	options->SetElementSizeY(6, 180.0f);

	VectorGUI *buttons = new VectorGUI(sizeDialog);
	buttons->Add(new Button(L"Zapisz"), this, &Menu::SaveOptions);
	buttons->Add(new Button(L"Anuluj"), this, &Menu::CloseOptionsDialog);
	buttons->SetPadding(4.0f);
	buttons->SetSpace(2.0f);
	options->Add(this, buttons);
	options->SetBottom(7);
	options->SetPadding(4.0f);
}

void Menu::SaveOptions()
{
	(*app->config)(L"width", { textInputWidth->GetString() });
	(*app->config)(L"height", { textInputHeight->GetString() });

	sf::String oldAA((*app->config)(L"antialiasing"));
	needsRecreate = true;
	switch (antaliasingMode)
	{
	case AntaliasingMode::OFF:
		(*app->config)(L"antialiasing", { L"0" });
		if (oldAA == L"0") needsRecreate = false;
		break;
	case AntaliasingMode::x4:
		(*app->config)(L"antialiasing", { L"4" });
		if (oldAA == L"4") needsRecreate = false;
		break;
	case AntaliasingMode::x8:
		(*app->config)(L"antialiasing", { L"8" });
		if (oldAA == L"8") needsRecreate = false;
		break;
	default:
		(*app->config)(L"antialiasing", { L"0" });
		if (oldAA == L"0") needsRecreate = false;
		break;
	}

	if (needsRecreate)
	{
		sf::ContextSettings settings;
		settings.antialiasingLevel = (*app->config)(L"antialiasing");
		app->event->window->create(sf::VideoMode((*app->config)(L"width"), (*app->config)(L"height")), L"Tytuł", sf::Style::Default, settings);
	}
	else
	{
		app->event->window->setSize({ (*app->config)(L"width"), (*app->config)(L"height") });
	}
	app->config->Save();
	CloseOptionsDialog();
}

void Menu::CloseOptionsDialog()
{
	delete options;
	options = nullptr;
}

void Menu::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*background);
	target.draw(*layout);
	if (options != nullptr)
	{
		target.draw(*options);
	}
}

void Menu::CreateScreen()
{
	background = new sf::Sprite;
	backgroundTexture = new sf::Texture();
	if (!backgroundTexture->loadFromFile("resource/images/menu-background.png"))
	{
		throw "Cannot load menu background image";
	}
	background->setTexture(*backgroundTexture);

	ResizeScreen();
	
	layout = new MenuLayout(*size);
	layout->ADD(new Button(L"Kreator"), &Menu::PressedCreator);
	layout->ADD(new Button(L"Wczytaj teren"), &Menu::PressedLoad);
	layout->ADD(new Button(L"Opcje"), &Menu::PressedOptions);
	layout->ADD(new Button(L"Wyjscie"), &Menu::PressedExit);

}

void Menu::ClearScreen()
{
	delete layout;
	layout = nullptr;

	delete background;
	background = nullptr;

	delete backgroundTexture;
	backgroundTexture = nullptr;

	delete options;
	options = nullptr;
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
		layout->SetSize({ static_cast<float>(size->x), static_cast<float>(size->y) });
	}
	sizeDialog = defaultSizeDialog;
	if (sizeDialog.x > size->x)
	{
		sizeDialog.x = size->x;
	}
	if (sizeDialog.y > size->y)
	{
		sizeDialog.y = size->y;
	}
	if (options != nullptr)
	{
		options->SetSize({ static_cast<float>(sizeDialog.x), static_cast<float>(sizeDialog.y) });
		options->SetDialogPosition({ static_cast<float>(size->x - sizeDialog.x) / 2.0f, static_cast<float>(size->y - sizeDialog.y) / 2.0f });
	}
}