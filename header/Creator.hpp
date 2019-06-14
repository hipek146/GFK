#pragma once
#include "Screen.hpp"
#include "GUI/GUI.hpp"
#include "Workspace.hpp"
#include "Data.hpp"

class Creator : public Screen {

	friend void Workspace::draw(sf::RenderTarget& target, sf::RenderStates states)const;
public:

	Creator(App *parent);
	~Creator();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void CreateScreen() override;
	virtual void ClearScreen() override;
	virtual void ResizeScreen() override;


private:
	enum class DrawMode
	{
		Line,
		Bezier,

	} drawMode; bool isDraw = true;
	enum class EditMode
	{
		Hill,
		Hole,

	} editMode; bool isEdit = false;
	enum class WaterMode
	{
		Simple,
		Spill,

	} waterMode; bool isWater = false;

	bool isTerrian = true;

	float editScroll = 50.0f;
	float waterScroll = 50.0f;
	bool points = false;

	void CreateInterface();
	void ClearInterface();

	void TerrianON() { isTerrian = true; isWater = false; }
//Draw Tab
	void DrawON() { isDraw = true; isEdit = false; }
	void Section() { drawMode = DrawMode::Line; }
	void Bezier() { drawMode = DrawMode::Bezier; }
	void DrawPoints() { CheckPoints(); }
	void Generator();
//Edit Tab
	void EditON() { isDraw = false; isEdit = true;  }
	void Hill(){ editMode = EditMode::Hill; }
	void Hole(){ editMode = EditMode::Hole; }
	void EditPoints() { CheckPoints(); }
//Water Tab
	void WaterON() { isWater = true; isTerrian = false; }
	void SimpleWater() { waterMode = WaterMode::Simple; containerWater->Hide(2); }
	void SpillWater() { waterMode = WaterMode::Spill; containerWater->Show(2); }
	void WaterPoints() { CheckPoints(); };

	void OptionsON() {isWater = false; isTerrian = false;}

	void MouseMove();
	void MouseClick();
	void MouseReleased();
	void Exit() { app->event->window->close(); }
	void GoToMenu() { app->LoadScreen(app->menu); }

	void CheckPoints();
	void Blank() {}

	enum drawingOptions {
		LINE,
		QUAD_BEZIER,
		CUBE_BEZIER
	};
	unsigned int drawingType = LINE;
	sf::Vector2f workspacePosition = { 0.0, 0.0 };
	sf::Vector2u workspaceSize;
	sf::Vector2f layoutPosition;
	sf::Vector2u layoutSize;

	CreatorLayout *creatorLayout;

	ContainerGUI *containerWater;
	VectorGUI *vectorDrawPoints;
	VectorGUI *vectorEditPoints;
	VectorGUI *vectorWaterPoints;

	Workspace *workspace;

	Touchpad *touchpad;

	Data *data;

	bool move = false;
	bool spill = false;
	int saveMouseX;
	int saveMouseY;
};
