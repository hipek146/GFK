#pragma once
#include "Screen.hpp"
#include "GUI/GUI.hpp"
#include "Workspace.hpp"

class Creator : public Screen {

public:

	Creator(App *parent);
	~Creator();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void CreateScreen() override;
	virtual void ClearScreen() override;
	virtual void ResizeScreen() override;


private:

	void MouseMove();
	void MouseClick();
	void Exit();
	void GoToMenu();
	void ChangeDrawingType();

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

	MenuLayout *layout;
	Workspace *workspace;
	Touchpad *touchpad;
	
};
