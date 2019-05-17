#pragma once
#include "Screen.hpp"
#include "GUI/GUI.hpp"
#include <iostream>

enum class MenuPressed
{
	Game,
	Creator,
	Options,
	Exit
};

class Menu : public Screen
{
public:
	Menu(App *parent);
	~Menu();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	virtual void CreateScreen() override;
	virtual void ClearScreen() override;
	virtual void ResizeScreen() override;
private:
	void Pressed(MenuPressed pressed);
	void PressedCreator() { Pressed(MenuPressed::Creator); }
	void PressedExit() { Pressed(MenuPressed::Exit); }

	sf::Sprite *background;
	sf::Texture *backgroundTexture;
	MenuLayout *layout;
};