#pragma once
#include "Screen.hpp"
#include "GUI/GUI.hpp"
#include "Workspace.hpp"

class Creator : public Screen
{
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
		Section,
		Bezier,

	} drawMode;
	enum class EditMode
	{
		Hill,
		Hole,

	} editMode;
	enum class WaterMode
	{
		Simple,
		Spill,

	} waterMode;

	float editScroll = 50.0f;
	float waterScroll = 50.0f;
	bool points = false;

	void CreateInterface();
	void ClearInterface();

//Draw Tab
	void Section() { drawMode = DrawMode::Section; }
	void Bezier() { drawMode = DrawMode::Bezier; }
	void DrawPoints() { CheckPoints(); }
	void Generator();
//Edit Tab
	void Hill(){ editMode = EditMode::Hill; }
	void Hole(){ editMode = EditMode::Hole; }
	void EditPoints() { CheckPoints(); }
//Water Tab
	void SimpleWater() { waterMode = WaterMode::Simple; containerWater->Hide(2); }
	void SpillWater() { waterMode = WaterMode::Spill; containerWater->Show(2); }
	void WaterPoints() { CheckPoints(); };

	void MouseMove();
	void MouseClick();
	void Exit() { app->event->window->close(); }
	void GoToMenu() { app->LoadScreen(app->menu); }

	void CheckPoints();
	void Blank() {}

	sf::Vector2f layoutPosition;
	sf::Vector2u layoutSize;

	Touchpad *touchpad;

	CreatorLayout *creatorLayout;


	CreatorTabs *creatorTabs;
	TabsLayout *tabsLayout;

	ContainerGUI *containerEdit;
	VectorGUI *vectorEditMode;
	VectorGUI *vectorEditScroll;
	VectorGUI *vectorEditPoints;

	ContainerGUI *containerDraw;
	VectorGUI *vectorDrawMode;
	VectorGUI *vectorDrawPoints;
	VectorGUI *vectorDrawGenerator;

	ContainerGUI *containerWater;
	VectorGUI *vectorWaterBoxes;
	VectorGUI *vectorWaterScroll;
	VectorGUI *vectorWaterPoints;

	MenuLayout *layoutOptions;

	Workspace workspace;
};
