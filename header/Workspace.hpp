#pragma once
#include "Data.hpp"
#include "SFML/Graphics.hpp"
#include "Bezier.hpp"
#include <future>

class Workspace : public sf::Drawable {
	
	friend class App;

	public:

		Workspace(Data *newData, sf::Vector2u *newSize, sf::Vector2f *newPosition);
		~Workspace();
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;		
		bool isMouseInWorkspaceArea(double x, double y);	
		bool isMouseClicked = false;	
		void DrawLinesGroup(sf::RenderTarget& target, sf::RenderStates states) const;
		void DrawCurrentLine(sf::RenderTarget& target, sf::RenderStates states) const;	
		bool AddPoint(int x, int y);	
		void UpdateMousePosition(int x, int y);
		inline bool CheckColision(sf::Vector2f a1, sf::Vector2f b1, sf::Vector2f a2, sf::Vector2f b2) const;	
		bool CheckAllColisions(sf::Vector2f a, sf::Vector2f b) const;	
		void PushBesierPoints();
		void DrawCurrentCurve(sf::RenderTarget& target, sf::RenderStates states) const;
		bool CheckBezierColisions()const;
		void Update(bool waterFlag = false);
		void Move(sf::Vector2i vector); 
		void AddWater(int x, int y, float value);
		void AddSimpleWater(int x, int y);
		sf::Vector2f & getLastPoint();
		Bezier *bezier;
		friend class Creator;
		bool *boolPixels;
		bool isPoints = false;
		void perlinNoise();
		void PerlinNoise1D(int nCount, double *noiseSeed, int nOctaves, double *surfacePixels);

	private:
		Data *data;
		sf::Texture texture;
		
		std::vector<sf::Vector2f> visiblePoints;
		sf::Uint8 *mapPixels;
		int count = 0;
		int lastGoodCount;
		std::vector<sfLine>lines;
		sf::Sprite *sprite;
		sf::Sprite *doubleBuffor;

		sf::Vector2f mousePosition;	
		std::vector <sf::Vector2f> mainPoints;	
		sf::Vector2f startingPoint;
		sf::Vector2f endingPoint;	
		sf::Vector2f lastPoint;
		sf::Vector2f *position;		
		sf::Vector2u *size;		
		sf::Vector2u originalSize;	


		sf::FloatRect area; 
		sf::Vector2f areaOffset;
		sf::Vector2f moveVector;
		sf::Vector2f spriteRelativePosition;
		std::future<void> ans; 

		struct myLittleWater
		{
			int x; int y;
			float value;
		};
		std::vector<myLittleWater> waterToSpill;
		sf::Vector2i simpleWater;
		bool isSimpleWater = false;
		bool async = false;
		bool *isDraw;
};


