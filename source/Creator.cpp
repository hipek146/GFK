#include "Creator.hpp"
#include "Dictionary.hpp"
#include <iostream>


Creator::Creator(App *parent) : Screen(parent) {}

Creator::~Creator() {
	ClearScreen();
}

void Creator::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	target.draw(*layout);
	workspace->draw(target, states);
}


void Creator::CreateScreen() {

	ResizeScreen();

	touchpad = new Touchpad(size);
	touchpad->onMouseMove(this, &Creator::MouseMove);
	touchpad->onClick(this, &Creator::MouseClick);
	workspace = new Workspace(&workspaceSize, &workspacePosition);
						
	layout = new MenuLayout(&layoutSize, &layoutPosition);
//	layout->Add(new Button("Przycisk 1")); // Niedobrze tylko go widać
	layout->Add(new Button("Button 1"), this); // Dobry przycisk bez podłączonej funkcji
	//layout->ADD_BLANK(new Button("Button 2")); // To samo co wyżej tylko "this" schowane w makro
	layout->Add(new Button(L"Bezier "), this, &Creator::ChangeDrawingType); //Przycisk z podłączoną funkcją
	layout->Add(new Button(L"Back to Menu"),this, &Creator::GoToMenu); //Przycisk z podłączoną funkcją
	layout->ADD(new Button(L"Exit"), &Creator::Exit);  // To samo co wyżej tylko "this" schowane w makro
}

void Creator::ClearScreen() {

	delete touchpad;
	touchpad = nullptr;

	delete layout;
	layout = nullptr;

	delete workspace;
	workspace = nullptr;
}

void Creator::ResizeScreen() {

	size = app->size;
	layoutSize = { 300, size->y };
	layoutPosition = { size->x - 300.0f, 0 };
	workspaceSize = { size->x - 300, size->y };
//	workspacePosition = { ..., ... };		// jak bedziemy chcieli zmienic polozenie, na razie jest [0,0]

	if (layout != nullptr) {
		touchpad->Resize();
		layout->Resize();
		layout->SetPosition(layoutPosition);
	}
}

void Creator::MouseMove() {

	int x = app->event->mouse.x;		
	int y = app->event->mouse.y;

	if (workspace->isMouseInWorkspaceArea(x, y)) {
		workspace->UpdateMousePosition(x, y);
	}
}

void Creator::MouseClick() {

	int x = app->event->mouse.x;		// mouse position [x, y]
	int y = app->event->mouse.y;		// edit: podobno do app mozna sie wszedzie dostac, potem sie refaktoryzacje zrobi

	if (workspace->isMouseInWorkspaceArea(x, y)) {
		switch (drawingType) {
		case LINE:
			workspace->AddPoint(x, y);

			break;
		case QUAD_BEZIER:
			if (!workspace->bezier->isControlPoint) {
				workspace->bezier->setEndPoint(1.0 * x,  1.0 * y);
				workspace->bezier->setStartPoint(workspace->getLastPoint());
				workspace->bezier->isControlPoint = true;
			}
			else {
				//workspace->RepleacePoint(workspace->bezier->endPoint.x, workspace->bezier->endPoint.y);
				workspace->bezier->setControlPoint(1.0 * x, 1.0 * y);
				workspace->PushBesierPoints();
				workspace->bezier->isControlPoint = false;
			}
			break;
		case CUBE_BEZIER:

			break;
		default:

			break;
		}
	}
}
void Creator::Exit() {
	app->event->window->close();
}
void Creator::GoToMenu() {
	app->LoadScreen(app->menu);
}
 
void Creator::ChangeDrawingType() {
	drawingType = QUAD_BEZIER;
}