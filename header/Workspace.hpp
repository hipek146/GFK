#pragma once
#include "SFML/Graphics.hpp"
#include "Triangulation.hpp"

class Workspace : public sf::Drawable {
	
	public:

		Workspace();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;		// wywoluje wszystkie rysujace metody
		void Resize();	// todo
		bool isMouseClicked = false;	// potem sie przyda jak bedziemy przyciski ogarniac
		void DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const;	// rysuje utworzone linie
		void DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const;	// rysuje aktualna linie (ta nie utworzana)
		void AddPoint(int x, int y);	// dodaje punkt do wektora mainPoints
		inline bool CheckColision(sf::Vector2f a1, sf::Vector2f b1, sf::Vector2f a2, sf::Vector2f b2) const;	// sprawdza kolizje dwoch lini
		bool CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const;	// sprawdza wszystkie kolizje aktualnej lini
		
	private:

		sf::Vector2f mousePosition;	// podobno nie trzeba
		std::vector <sf::Vector2f> mainPoints;	// grupa stworzonych punktow, na jej podstawie rysujemy linie, obliczamy kolizje itd
		sf::Vector2f startingPoint;	// punkt poczatkowy, na razie ustawiony na sztywno
		sf::Vector2f endingPoint;	// nie jest koniecznie, moze sie przydac w trybie "debug"
		sf::Vector2f *position;		// pozycja workspace
		sf::Vector2u *size;		// size workspace
		sf::Vector2u originalSize;	// poczatkowy rozmiar okna (po wejsciu do workspace)
		Triangulation *triangulation;
};