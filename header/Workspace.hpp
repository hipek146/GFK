#pragma once
#include "Data.hpp"
#include "SFML/Graphics.hpp"
#include "Bezier.hpp"
#include <future>

class Workspace : public sf::Drawable {
	
	public:

		Workspace(Data *newData, sf::Vector2u *newSize, sf::Vector2f *newPosition);
		~Workspace();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;		// wywoluje wszystkie rysujace metody
		void Resize();	// todo
		bool isMouseInWorkspaceArea(double x, double y);	// sprawdza czy myszka jest w workspace, skaluje sie
		bool isMouseClicked = false;	// potem sie przyda jak bedziemy przyciski ogarniac
		void DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const;	// rysuje utworzone linie
		void DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const;	// rysuje aktualna linie (ta nie utworzana)
		void DrawDotsGroup(sf::RenderTarget& target, sf::RenderStates states) const;	// na razie nic tu nie ma, moze sie przydac
		bool AddPoint(int x, int y);	// dodaje punkt do wektora mainPoints
		void UpdateMousePosition(int x, int y);	// podobno nie trzeba tego
		inline bool CheckColision(sf::Vector2f a1, sf::Vector2f b1, sf::Vector2f a2, sf::Vector2f b2) const;	// sprawdza kolizje dwoch lini
		bool CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const;	// sprawdza wszystkie kolizje aktualnej lini
		void FillSpaceBetweenPoints(sf::RenderTarget& target, sf::RenderStates states) const;	// todo
		void PushBesierPoints();
		void DrawCurrentCurve(sf::RenderTarget& target, sf::RenderStates states) const;
		bool CheckBezierColisions()const;
		void Update(bool waterFlag = false); //Aktualizuje teksture do rysowania
		void Move(sf::Vector2i vector); //przesuwa widzialny obszar o wektor
		void AddWater(int x, int y, float value);
		void AddSimpleWater(int x, int y);
		sf::Vector2f & getLastPoint();
		Bezier *bezier;
		friend class Creator;
		bool *boolPixels;
		bool isPoints = false;
	private:
		Data *data;
		sf::Texture texture;
		
		std::vector<sf::Vector2f> visiblePoints;
		int count = 0;
		int lastGoodCount;
		std::vector<sfLine>lines;
		sf::Sprite *sprite;
		sf::Sprite *doubleBuffor;

		sf::Vector2f mousePosition;	// podobno nie trzeba
		std::vector <sf::Vector2f> mainPoints;	// grupa stworzonych punktow, na jej podstawie rysujemy linie, obliczamy kolizje itd
		sf::Vector2f startingPoint;	// punkt poczatkowy, na razie ustawiony na sztywno
		sf::Vector2f endingPoint;	// nie jest koniecznie, moze sie przydac w trybie "debug"
		sf::Vector2f lastPoint; // ostatnio narysowany punkt idac wzdloz powierzchni 
		sf::Vector2f *position;		// pozycja workspace
		sf::Vector2u *size;		// size workspace
		sf::Vector2u originalSize;	// poczatkowy rozmiar okna (po wejsciu do workspace)


		sf::FloatRect area; //Widoczny obszar
		sf::Vector2f areaOffset;
		sf::Vector2f moveVector;
		sf::Vector2f spriteRelativePosition;
		std::future<void> ans; //Watek do wczytywania
		struct myLittleWater
		{
			int x; int y;
			float value;
		};
		std::vector<myLittleWater> waterToSpill;
		sf::Vector2i simpleWater;
		bool isSimpleWater = false;
		bool async = false;
};


