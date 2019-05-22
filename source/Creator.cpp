#include "Creator.hpp"
#include "Dictionary.hpp"
#include <iostream>


Creator::Creator(App *parent) : Screen(parent) {}

Creator::~Creator()
{
	ClearScreen();
}

void Creator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*creatorLayout);
}


void Creator::CreateScreen()
{
	ResizeScreen();

	touchpad = new Touchpad(this, size);
	touchpad->onMouseMove(this, &Creator::MouseMove);
	touchpad->onClick(this, &Creator::MouseClick);
	
	creatorLayout = new CreatorLayout(this, layoutSize, layoutPosition);

	menu1 = new CreatorMenu(layoutSize, layoutPosition);

	layout = new MenuLayout(layoutSize, layoutPosition);
	layout->Add(new Button("Przycisk 1")); // Niedobrze tylko go widać
	layout->Add(new Button("Przycisk 1"), this); // Dobry przycisk bez podłączonej funkcji
	layout->ADD_BLANK(new Button("Przycisk 3")); // To samo co wyżej tylko "this" schowane w makro
	layout->Add(new Button(L"Let me leave..."),this, &Creator::Exit); //Przycisk z podłączoną funkcją
	layout->ADD(new Button(L"Fuck go back!"), &Creator::GoToMenu);  // To samo co wyżej tylko "this" schowane w makro

	layout2 = new MenuLayout(layoutSize, layoutPosition);
	layout2->ADD(new Button(L"Pierwszy"), &Creator::GoToMenu);
	layout2->ADD(new Button(L"Drugi"), &Creator::GoToMenu);
	layout2->ADD(new Button(L"Trzeci"), &Creator::Exit);

	layout3 = new MenuLayout(layoutSize, layoutPosition);
	layout3->ADD(new Button(L"Kolejny"), &Creator::GoToMenu);
	layout3->ADD_BLANK(new Button(L"Pusty"));
	layout3->ADD(new Button(L"I Wyjscie"), &Creator::Exit);

	menu1->Add(new Button(L"kurde no"), this, layout);
	menu1->Add(new Button(L"ta druga opcja"), this, layout2);
	menu1->Add(new Button(L"Jeszcze jedna"), this, layout3);

	creatorLayout->Add(menu1);
}

void Creator::ClearScreen()
{
	delete touchpad;
	touchpad = nullptr;

	delete layout;
	layout = nullptr;

	delete layout2;
	layout2 = nullptr;

	delete layout3;
	layout3 = nullptr;

	delete creatorLayout;
	creatorLayout = nullptr;

	delete menu1;
	menu1 = nullptr;
}

void Creator::ResizeScreen()
{
	layoutSize = { 350, size->y };
	layoutPosition = { size->x - 350.0f, 0 };
	
	if (layout != nullptr)
	{
		touchpad->Resize();
		creatorLayout->SetSize({ static_cast<float>(layoutSize.x), static_cast<float>(layoutSize.y) });
		creatorLayout->Resize();
		creatorLayout->SetPosition(layoutPosition);
	}
}

void Creator::MouseMove()
{
	std::cout << "x: " << app->event->mouse.x << "\ty: " << app->event->mouse.y << std::endl;
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
