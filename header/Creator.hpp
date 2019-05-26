#pragma once
#include "Screen.hpp"
#include "GUI/GUI.hpp"

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
	void SimpleWater();
	void SpillWater();
	void MouseMove();
	void MouseClick();
	void Exit();
	void GoToMenu();

	float scrollX;
	sf::Vector2f layoutPosition;
	sf::Vector2u layoutSize;
	MenuLayout *layout;
	MenuLayout *layout2;
	MenuLayout *layoutOptions;
	Touchpad *touchpad;
	CreatorLayout *creatorLayout;
	CreatorTabs *creatorTabs;
	TabsLayout *tabsLayout;
	ContainerGUI *containerGUI;
	ContainerGUI *containerWater;
	VectorGUI *vectorGUI_1;
	VectorGUI *vectorGUI_2;
	VectorGUI *vectorGUI_3;
	VectorGUI *vectorGUI_4;
	VectorGUI *vectorWaterBoxes;
	VectorGUI *vectorWaterScroll;
};
