#include "Creator.hpp"
#include "Dictionary.hpp"


Creator::Creator(App *parent) : Screen(parent) {}

Creator::~Creator()
{
	ClearScreen();
}

void Creator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*layout);
}


void Creator::CreateScreen()
{
	ResizeScreen();

	touchpad = new Touchpad(size);
	touchpad->onMouseMove(this, &Creator::MouseMove);
	touchpad->onClick(this, &Creator::MouseClick);
	
	layout = new MenuLayout(&layoutSize, &layoutPosition);
	layout->Add(new Button("Przycisk 1")); // Niedobrze tylko go widać
	layout->Add(new Button("Przycisk 1"), this); // Dobry przycisk bez podłączonej funkcji
	layout->ADD_BLANK(new Button("Przycisk 3")); // To samo co wyżej tylko "this" schowane w makro
	layout->Add(new Button(L"Let me leave..."),this, &Creator::Exit); //Przycisk z podłączoną funkcją
	layout->ADD(new Button(L"Fuck go back!"), &Creator::GoToMenu);  // To samo co wyżej tylko "this" schowane w makro
}

void Creator::ClearScreen()
{
	delete touchpad;
	touchpad = nullptr;

	delete layout;
	layout = nullptr;
}

void Creator::ResizeScreen()
{
	layoutSize = { 300, size->y };
	layoutPosition = { size->x - 300.0f, 0 };

	if (layout != nullptr)
	{
		layout->Resize();
		layout->SetPosition(layoutPosition);
	}
}

void Creator::MouseMove()
{
	
}

void Creator::MouseClick()
{
	
}
void Creator::Exit()
{
	app->event->window->close();
}
void Creator::GoToMenu()
{
	app->LoadScreen(app->menu);
}
