#include "Creator.hpp"
#include "Dictionary.hpp"
#include <iostream>


Creator::Creator(App *parent) : Screen(parent) {}

Creator::~Creator()
{
}

void Creator::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*workspace);
	target.draw(*creatorLayout);
}


void Creator::CreateScreen() {

	ResizeScreen();

	touchpad = new Touchpad(this, size);
	touchpad->onMouseMove(this, &Creator::MouseMove);
	touchpad->onClick(this, &Creator::MouseClick);
	
	CreateInterface();

	workspace = new Workspace(&workspaceSize, &workspacePosition);

}

void Creator::ClearScreen() {

	delete touchpad;
	touchpad = nullptr;

	delete workspace;
	workspace = nullptr;

	ClearInterface();
}

void Creator::ResizeScreen()
{
	layoutSize = { 400, size->y };
	layoutPosition = { size->x - 400.0f, 0 };
	
	workspaceSize = { size->x, size->y };
//	workspacePosition = { ..., ... };		// jak bedziemy chcieli zmienic polozenie, na razie jest [0,0]

	if (creatorLayout != nullptr) {
		touchpad->Resize();
		creatorLayout->SetSize({ static_cast<float>(layoutSize.x), static_cast<float>(layoutSize.y) });
		creatorLayout->SetPosition(layoutPosition);
	}
}


void Creator::Generator()
{
}

void Creator::CheckPoints()
{
	if (points) points = false; else points = true;

	if (points)
	{
		vectorDrawPoints->Active(1);
		vectorEditPoints->Active(1);
		vectorWaterPoints->Active(1);
	}
	else
	{
		vectorDrawPoints->Inactive(1);
		vectorEditPoints->Inactive(1);
		vectorWaterPoints->Inactive(1);
	}

}


void Creator::CreateInterface()
{
	CreatorTabs *creatorTabs;
	TabsLayout *tabsLayout;

	ContainerGUI *containerEdit;
	VectorGUI *vectorEditMode;
	VectorGUI *vectorEditScroll;

	ContainerGUI *containerDraw;
	VectorGUI *vectorDrawMode;
	VectorGUI *vectorDrawGenerator;

	VectorGUI *vectorWaterBoxes;
	VectorGUI *vectorWaterScroll;

	MenuLayout *layoutOptions;

	creatorLayout = new CreatorLayout(this, layoutSize, layoutPosition);

	creatorTabs = new CreatorTabs(layoutSize);

	tabsLayout = new TabsLayout(layoutSize);

	/////////////////////////////Rysuj//////////////////////////////
	containerDraw = new ContainerGUI(layoutSize);

	vectorDrawMode = new VectorGUI(layoutSize);
	vectorDrawMode->Add(new Button(L"Odcinki"), this, &Creator::Section);
	vectorDrawMode->Add(new Button(L"Krzywe Beziera"), this, &Creator::Bezier);
	vectorDrawMode->EnableOneChoice(1);
	vectorDrawMode->Vertical();
	vectorDrawMode->SetSpace(2.0f);
	vectorDrawMode->SetPadding(10.0f);

	vectorDrawPoints = new VectorGUI(layoutSize);
	vectorDrawPoints->Add(new Checkbox(L"Pokaz punkty"), this, &Creator::DrawPoints);

	vectorDrawGenerator = new VectorGUI(layoutSize);
	vectorDrawGenerator->Add(new Button(L"Generuj teren"), this, &Creator::Generator);
	vectorDrawGenerator->SetPadding(8.0f);


	containerDraw->Add(this, vectorDrawMode);
	containerDraw->SetElementSizeY(1, 140.0f);
	containerDraw->Add(this, vectorDrawPoints);
	containerDraw->SetElementSizeY(2, 50.0f);
	containerDraw->SetBottom(2);
	containerDraw->Add(this, vectorDrawGenerator);
	containerDraw->SetBottom(3);
	tabsLayout->Add(new Button(L"Rysuj"), this, containerDraw);


	///////////////////////////Edytuj///////////////////////////////
	containerEdit = new ContainerGUI(layoutSize);
	vectorEditMode = new VectorGUI(layoutSize);
	vectorEditMode->Add(new Button(L"Gorki"), this, &Creator::Hill);
	vectorEditMode->Add(new Button(L"Dolki"), this, &Creator::Hole);
	vectorEditMode->SetSpace(2.0f);
	vectorEditMode->SetPadding(10.0f);
	vectorEditMode->EnableOneChoice(2);

	vectorEditScroll = new VectorGUI(layoutSize);
	vectorEditScroll->Add(new ScrollBar(&editScroll), this, &Creator::Blank);

	vectorEditPoints = new VectorGUI(layoutSize);
	vectorEditPoints->Add(new Checkbox(L"Pokaz punkty"), this, &Creator::EditPoints);


	containerEdit->Add(this, vectorEditMode);
	containerEdit->Add(this, vectorEditScroll);
	containerEdit->SetElementSizeY(2, 42.0f);
	containerEdit->Add(this, vectorEditPoints);
	containerEdit->SetElementSizeY(3, 50.0f);
	containerEdit->SetBottom(3);

	tabsLayout->Add(new Button(L"Edytuj"), this, containerEdit);

	creatorTabs->Add(new Button(L"Teren"), this, tabsLayout);
	///////////////////////////Woda///////////////////////////////
	containerWater = new ContainerGUI(layoutSize);

	vectorWaterBoxes = new VectorGUI(layoutSize);
	vectorWaterBoxes->Add(new Checkbox(L"Do ustalonego poziomu"), this, &Creator::SimpleWater);
	vectorWaterBoxes->Add(new Checkbox(L"Przelewaj wode"), this, &Creator::SpillWater);
	vectorWaterBoxes->EnableOneChoice(1);
	vectorWaterBoxes->Vertical();

	containerWater->Add(this, vectorWaterBoxes);
	containerWater->SetElementSizeY(1, 120.0f);

	vectorWaterScroll = new VectorGUI(layoutSize);
	vectorWaterScroll->Add(new ScrollBar(&waterScroll), this, &Creator::Blank);

	containerWater->Add(this, vectorWaterScroll);
	containerWater->SetElementSizeY(2, 42.0f);
	containerWater->Hide(2);

	vectorWaterPoints = new VectorGUI(layoutSize);
	vectorWaterPoints->Add(new Checkbox(L"Pokaz punkty"), this, &Creator::WaterPoints);

	containerWater->Add(this, vectorWaterPoints);
	containerWater->SetElementSizeY(3, 50.0f);
	containerWater->SetBottom(3);

	creatorTabs->Add(new Button(L"Woda"), this, containerWater);

	/////////////////////////Opcje/////////////////////////////////
	layoutOptions = new MenuLayout(layoutSize);
	Button *saveButton = new Button(L"Zapisz");
	layoutOptions->Add(saveButton, this, &Creator::Blank);
	app->event->Add(&saveButton->rect, app, saveButton, &App::SaveDialog, EventType::Pressed);
	Button *loadButton = new Button(L"Wczytaj");
	layoutOptions->Add(loadButton, this, &Creator::Blank);
	app->event->Add(&loadButton->rect, app, loadButton, &App::LoadDialog, EventType::Pressed);
	layoutOptions->ADD(new Button(L"Wstecz"), &Creator::GoToMenu);
	layoutOptions->ADD(new Button(L"Wyjscie"), &Creator::Exit);

	creatorTabs->Add(new Button(L"Opcje"), this, layoutOptions);
	///////////////////////////////////////////////////////////////
	creatorLayout->Add(creatorTabs);
}
void Creator::ClearInterface()
{
	delete creatorLayout;
	creatorLayout = nullptr;
}

void Creator::MouseMove() {

	int x = app->event->mouse.x;		
	int y = app->event->mouse.y;
	std::cout << x << " " << y << std::endl;
	if (workspace->isMouseInWorkspaceArea(x, y)) {
		workspace->UpdateMousePosition(x, y);
	}
}

void Creator::MouseClick() {

	int x = app->event->mouse.x;		// mouse position [x, y]
	int y = app->event->mouse.y;		// edit: podobno do app mozna sie wszedzie dostac, potem sie refaktoryzacje zrobi

	if (workspace->isMouseInWorkspaceArea(x, y)) {
		switch (drawMode) {
		case DrawMode::Section:
			workspace->AddPoint(x, y);

			break;
		case DrawMode::Bezier:
			if (!workspace->bezier->isControlPoint) {

				if (!workspace->CheckAllColisions(workspace->getLastPoint(), sf::Vector2f(x, y))) {
					workspace->bezier->setStartPoint(workspace->getLastPoint());
					workspace->bezier->setEndPoint(1.0 * x, 1.0 * y);
					workspace->bezier->isControlPoint = true;
				}
			}
			else if(!workspace->CheckBezierColisions()) {
				workspace->bezier->setControlPoint(1.0 * x, 1.0 * y);
				workspace->PushBesierPoints();
				workspace->bezier->isControlPoint = false;
			}
			break;
		default:

			break;
		}
	}
}
