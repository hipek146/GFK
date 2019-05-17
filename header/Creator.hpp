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
	void MouseMove();
	void MouseClick();
	void Exit();
	void GoToMenu();

	sf::Vector2f layoutPosition;
	sf::Vector2u layoutSize;
	MenuLayout *layout;
	Touchpad *touchpad;
};
