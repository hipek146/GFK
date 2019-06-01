#include "Workspace.hpp"

void Workspace::draw(sf::RenderTarget& target, sf::RenderStates states)const {

//	FillSpaceBetweenPoints(target, states);		todo
	DrawLinesGroup(target, states);				// rysuje utworzone linie
	//if
	if (bezier->isControlPoint) {
		bezier->CalcQuadBezier();
		DrawCurrentCurve(target, states);
	}
	else
		DrawCurrentLine(target, states);	// rysuje linie tymczasowa, podglad gdzie sie pojawi jak nacisniemy LPM
//	DrawDotsGroup(target, states);				// puste, bedzie mozna potem zrobic jakies kropki czy co tam chcemy

}

Workspace::Workspace(sf::Vector2u *newSize, sf::Vector2f *newPosition) : size(newSize), position(newPosition) {

	bezier = new Bezier();
	originalSize = *size;
	startingPoint = { 0.0, static_cast<float>(size->y / 2.0) };		// punkt poczatkowy rysowania, potem sie zrobi zeby uzytkownik mogl go wybrac
	endingPoint = mousePosition = startingPoint;					// ostatni punkt rysowania, na razie to jest ten sam
	mainPoints.push_back(startingPoint);							
	mainPoints.push_back(startingPoint);
}

Workspace::~Workspace()
{
	delete bezier;
}

bool Workspace::isMouseInWorkspaceArea(double x, double y) {		// sprawdza czy myszka jest w workspace, skaluje sie, bo size trzyma wskaznik na aktualny rozmiar

	if (position->x <= x && x <= position->x + size->x) 
		if (position->y <= y && y <= position->y + size->y) 
			return true;
	return false;
}

void Workspace::Resize() {
	//todo
}


void Workspace::DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const {

	for (int i = 0; i < mainPoints.size() - 1; i++) {
		sfLine currentLine(mainPoints[i], mainPoints[i + 1], sf::Color::Black, 3.0);	// wlasna klasa sfLine, wzialem ja z forum SFML, podaje sie punkty i grubosc lini do narysowania
		currentLine.draw(target, states);		// rysuja ta linie
	}
}


void Workspace::FillSpaceBetweenPoints(sf::RenderTarget& target, sf::RenderStates states) const {	// todo

	
}

void Workspace::DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const {	// rysuje utworzone linie

	sf::Color lineColor;
	if (CheckAllColisions(mainPoints.back(), mousePosition)) {
		lineColor = sf::Color::Red;
	}
	else {
		lineColor = sf::Color::Green;
	}
	sfLine currentLine(mainPoints.back(), mousePosition, lineColor, 3.0);
	currentLine.draw(target, states);
}

bool Workspace::CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const {	// sprawdza kolizje lini z wszystkimi pozostalymi liniami

	for (int i = 1; i < mainPoints.size() - 1; i++)	{
		if (CheckColision(mainPoints[i], mainPoints[i + 1], a, b)) {
			return true;
		}
	}
	return false;
}

void Workspace::DrawCurrentCurve(sf::RenderTarget& target, sf::RenderStates states) const {

	sf::Color lineColor;

	if (CheckBezierColisions()) {
		lineColor = sf::Color::Red;
	}
	else {
		lineColor = sf::Color::Green;
	}
	for (int i = 0; i < bezier->points.size() - 1; i++) {
		sfLine currentLine(bezier->points[i], bezier->points[i + 1], lineColor, 3.0);
		currentLine.draw(target, states);
	}
}

bool Workspace::CheckBezierColisions()const{
	
	for (int i = 1; i < bezier->points.size() - 2; i++) {
		for (int j = mainPoints.size() - 1; j >  0; j--) {
			if (CheckColision(mainPoints[j], mainPoints[j - 1], bezier->points[i], bezier->points[i+1])) {
				std::cout << "Bezier colision detected at " << mainPoints[j].x << " " << mainPoints[j].y << " " << 
					mainPoints[j - 1].x << " " << mainPoints[j - 1].y << std::endl;
				return true;
			}
		}
	}
	return false;
}

inline bool Workspace::CheckColision(sf::Vector2f a1, sf::Vector2f b1, sf::Vector2f a2, sf::Vector2f b2) const {	
	
	// sprawdza kolizje 2 lini:
// a1-------b1 oraz a2---------b2

	int x1 = a1.x;
	int y1 = a1.y;

	int x2 = b1.x;
	int y2 = b1.y;

	int x3 = a2.x;
	int y3 = a2.y;

	int x4 = b2.x;
	int y4 = b2.y;

	float denominator = ((x2 - x1) * (y4 - y3)) - ((y2 - y1) * (x4 - x3));
	float numerator1 = ((y1 - y3) * (x4 - x3)) - ((x1 - x3) * (y4 - y3));
	float numerator2 = ((y1 - y3) * (x2 - x1)) - ((x1 - x3) * (y2 - y1));

	if (denominator == 0) {
		//std::cout << "Denominator equals 0" << std::endl;
		return false;
	}

	//if (denominator == 0) return (numerator1 == 0 && numerator2 == 0);	// sprawdza czy mianownik jest 0

	float r = numerator1 / denominator;
	float s = numerator2 / denominator;
	if ((r >= 0 && r < 1) && (s >= 0 && s <= 1)) {
		/*std::cout << "Colision detected between points : " << x1 << " " << y1 << " " << x2 << " " << y2 << " "
			<< x3 << " " << y3 << " " << x4 << " " << y4 << std::endl;*/
	}
	return (r >= 0 && r < 1) && (s >= 0 && s <= 1);
}

void Workspace::AddPoint(int x, int y) {	// dodaje punkt jesli mozna (nie ma kolizji)

	if (CheckAllColisions(mainPoints.back(), mousePosition) == false) {
		mainPoints.push_back(sf::Vector2f(1.0*x, 1.0*y));
	}
}

void Workspace::UpdateMousePosition(int x, int y) {			// podobno nie potrzebna, potem sie zrobi refaktoryzacje

	mousePosition.x = static_cast<double>(x);
	mousePosition.y = static_cast<double>(y);
	bezier->setControlPoint(x, y);
}

void Workspace::DrawDotsGroup(sf::RenderTarget& target, sf::RenderStates states) const {
	// moze sie przydac 
}

sf::Vector2f & Workspace::getLastPoint() {
	return mainPoints.back();
}

void Workspace::PushBesierPoints() {
	for (std::vector<sf::Vector2f>::const_iterator a = bezier->points.begin(); a != bezier->points.end(); ++a) {
		mainPoints.push_back(*a);
	}
}


