#include "Workspace.hpp"
#include "sfLine.hpp"

void Workspace::draw(sf::RenderTarget& target, sf::RenderStates states) const {

//	FillSpaceBetweenPoints(target, states);		todo
	DrawLinesGroup(target, states);				// rysuje utworzone linie
	DrawCurrentLine(target, states);			// rysuje linie tymczasowa, podglad gdzie sie pojawi jak nacisniemy LPM
//	DrawDotsGroup(target, states);				// puste, bedzie mozna potem zrobic jakies kropki czy co tam chcemy
	DrawWorkspaceBorder(target, states);
}

Workspace::Workspace(sf::Vector2u *newSize, sf::Vector2f *newPosition) : size(newSize), position(newPosition) {

	originalSize = *size;
	startingPoint = { 0.0, static_cast<float>(size->y / 2.0) };		// punkt poczatkowy rysowania, potem sie zrobi zeby uzytkownik mogl go wybrac
	endingPoint = mousePosition = startingPoint;					// ostatni punkt rysowania, na razie to jest ten sam
	mainPoints.push_back(startingPoint);							
	mainPoints.push_back(startingPoint);
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

void Workspace::DrawWorkspaceBorder(sf::RenderTarget& target, sf::RenderStates states) const {	// nie skaluje sie, trzeba to ogarnac

	int spaceInPixels = 0;	// grubosc obramowki workspace

	sfLine currentLine1(sf::Vector2f(spaceInPixels, spaceInPixels), sf::Vector2f(spaceInPixels, originalSize.y - spaceInPixels), sf::Color::Black, 3.0);
	currentLine1.draw(target, states);

	sfLine currentLine2(sf::Vector2f(spaceInPixels, originalSize.y - spaceInPixels), sf::Vector2f(originalSize.x - spaceInPixels, originalSize.y - spaceInPixels), sf::Color::Black, 3.0);
	currentLine2.draw(target, states);

	sfLine currentLine3(sf::Vector2f(originalSize.x - spaceInPixels, originalSize.y - spaceInPixels), sf::Vector2f(originalSize.x - spaceInPixels, spaceInPixels), sf::Color::Black, 3.0);
	currentLine3.draw(target, states);

	sfLine currentLine4(sf::Vector2f(originalSize.x - spaceInPixels, spaceInPixels), sf::Vector2f(spaceInPixels, spaceInPixels), sf::Color::Black, 3.0);
	currentLine4.draw(target, states);
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

	if (denominator == 0) return (numerator1 == 0 && numerator2 == 0);	// sprawdza czy mianownik jest 0
	float r = numerator1 / denominator;
	float s = numerator2 / denominator;
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
}

void Workspace::DrawDotsGroup(sf::RenderTarget& target, sf::RenderStates states) const {
	// moze sie przydac 
}