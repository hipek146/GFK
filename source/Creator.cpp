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

	creatorTabs = new CreatorTabs(layoutSize);

	tabsLayout = new TabsLayout(layoutSize);

	layout = new MenuLayout(layoutSize);
	layout->Add(new Button("Przycisk 1")); // Niedobrze tylko go widać
	layout->Add(new Button("Przycisk 1"), this); // Dobry przycisk bez podłączonej funkcji
	layout->ADD_BLANK(new Button("Przycisk 3 o bardzo dlugiej nazwie")); // To samo co wyżej tylko "this" schowane w makro
	layout->Add(new Button(L"Let me leave..."),this, &Creator::Exit); //Przycisk z podłączoną funkcją
	layout->ADD(new Button(L"Fuck go back!"), &Creator::GoToMenu);  // To samo co wyżej tylko "this" schowane w makro

	tabsLayout->Add(new Button(L"Rysuj"), this, layout);

	layout2 = new MenuLayout(layoutSize);
	layout2->ADD(new Button(L"Pierwszy"), &Creator::GoToMenu);
	layout2->ADD(new Button(L"Drugi"), &Creator::GoToMenu);
	layout2->ADD(new Button(L"Trzeci"), &Creator::Exit);


	containerGUI = new ContainerGUI(layoutSize);
	vectorGUI_1 = new VectorGUI(layoutSize);
	vectorGUI_1->Add(new Button(L"vector"), this, &Creator::MouseClick);
	vectorGUI_1->Add(new Button(L"Vector2"), this, &Creator::MouseClick);
	vectorGUI_1->Add(new Button(L"GUI"), this, &Creator::MouseClick);
	vectorGUI_1->SetSpace(2.0f);
	vectorGUI_1->SetPadding(10.0f);
	vectorGUI_1->EnableOneChoice(2);

	vectorGUI_2 = new VectorGUI(layoutSize);
	//vectorGUI_2->Add(new Button(L"lol"), this, &Creator::MouseClick);
	//vectorGUI_2->Add(new Button(L"xD"), this, &Creator::MouseClick);
	vectorGUI_2->Add(new ScrollBar(&scrollX), this, &Creator::MouseClick);

	vectorGUI_3 = new VectorGUI(layoutSize);
	vectorGUI_3->Add(new Checkbox(L"Opcja"), this, &Creator::MouseMove);
	vectorGUI_3->Add(new Checkbox(L"Dwojeczka"), this, &Creator::MouseMove);
	vectorGUI_3->EnableOneChoice();

	vectorGUI_4 = new VectorGUI(layoutSize);
	vectorGUI_4->Add(new TextInput(L"Jakis tekst input"), this, &Creator::MouseMove);

	containerGUI->Add(this, vectorGUI_1);
	containerGUI->Add(this, vectorGUI_2);
	containerGUI->Add(this, vectorGUI_3);
	containerGUI->Add(this, vectorGUI_4);

	tabsLayout->Add(new Button(L"Edytuj"), this, containerGUI);

	creatorTabs->Add(new Button(L"Teren"), this, tabsLayout);
///////////////////////////////////////////////////////////////
	containerWater = new ContainerGUI(layoutSize);

	vectorWaterBoxes = new VectorGUI(layoutSize);
	vectorWaterBoxes->Add(new Checkbox(L"Do ustalonego poziomu"), this, &Creator::SimpleWater);
	vectorWaterBoxes->Add(new Checkbox(L"Przelewaj wode"), this, &Creator::SpillWater);
	vectorWaterBoxes->EnableOneChoice(1);
	vectorWaterBoxes->Vertical();

	containerWater->Add(this, vectorWaterBoxes);
	containerWater->SetElementSizeY(1, 120.0f);

	vectorWaterScroll = new VectorGUI(layoutSize);
	vectorWaterScroll->Add(new ScrollBar(&scrollX), this, &Creator::MouseClick);

	containerWater->Add(this, vectorWaterScroll);
	containerWater->SetElementSizeY(2, 38.0f);
	containerWater->Hide(2);

	creatorTabs->Add(new Button(L"Woda"), this, containerWater);
///////////////////////////////////////////////////////////////
	layoutOptions = new MenuLayout(layoutSize);
	layoutOptions->ADD_BLANK(new Button(L"Zapisz"));
	layoutOptions->ADD_BLANK(new Button(L"Wczytaj"));
	layoutOptions->ADD(new Button(L"Wstecz"), &Creator::GoToMenu);;
	layoutOptions->ADD(new Button(L"Wyjscie"), &Creator::Exit);

	creatorTabs->Add(new Button(L"Opcje"), this, layoutOptions);
///////////////////////////////////////////////////////////////
	creatorLayout->Add(creatorTabs);
}

void Creator::ClearScreen()
{
	delete touchpad;
	touchpad = nullptr;

	delete layout;
	layout = nullptr;

	delete layout2;
	layout2 = nullptr;

	delete layoutOptions;
	layoutOptions = nullptr;

	delete creatorLayout;
	creatorLayout = nullptr;

	delete creatorTabs;
	creatorTabs = nullptr;

	delete tabsLayout;
	tabsLayout = nullptr;

	delete containerGUI;
	containerGUI = nullptr;

	delete vectorGUI_1;
	vectorGUI_1 = nullptr;

	delete vectorGUI_2;
	vectorGUI_2 = nullptr;

	delete vectorGUI_3;
	vectorGUI_3 = nullptr;

	delete vectorGUI_4;
	vectorGUI_4 = nullptr;

	delete containerWater;
	containerWater = nullptr;

	delete vectorWaterBoxes;
	vectorWaterBoxes = nullptr;

	delete vectorWaterScroll;
	vectorWaterScroll = nullptr;
}

void Creator::ResizeScreen()
{
	layoutSize = { 400, size->y };
	layoutPosition = { size->x - 400.0f, 0 };
	
	if (layout != nullptr)
	{
		touchpad->Resize();
		creatorLayout->SetSize({ static_cast<float>(layoutSize.x), static_cast<float>(layoutSize.y) });
		creatorLayout->SetPosition(layoutPosition);
	}
}

void Creator::SimpleWater()
{
	containerWater->Hide(2);
}

void Creator::SpillWater()
{
	containerWater->Show(2);
}


void Creator::MouseMove()
{
	//std::cout << "x: " << app->event->mouse.x << "\ty: " << app->event->mouse.y << std::endl;
}

void Creator::MouseClick()
{
	std::cout << "x-> " << scrollX << std::endl;
}
void Creator::Exit()
{
	app->event->window->close();
}
void Creator::GoToMenu()
{
	app->LoadScreen(app->menu);
}
