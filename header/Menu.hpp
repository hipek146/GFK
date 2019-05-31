#pragma once
#include "Screen.hpp"
#include "GUI/GUI.hpp"
#include <iostream>

enum class MenuPressed
{
	Load,
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
	void PressedLoad() { Pressed(MenuPressed::Load); }
	void PressedCreator() { Pressed(MenuPressed::Creator); }
	void PressedOptions() { Pressed(MenuPressed::Options); }
	void PressedExit() { Pressed(MenuPressed::Exit); }


	enum class AntaliasingMode
	{
		OFF,
		x4,
		x8,

	} antaliasingMode;
	void OptionsDialog();
	void CloseOptionsDialog();
	void OptionsAntyaliasingOFF() { antaliasingMode = AntaliasingMode::OFF; }
	void OptionsAntyaliasing4x() { antaliasingMode = AntaliasingMode::x4; }
	void OptionsAntyaliasing8x() { antaliasingMode = AntaliasingMode::x8; }
	void SaveOptions();
	void Blank() {}

	sf::Sprite *background;
	sf::Texture *backgroundTexture;
	MenuLayout *layout;
	DialogBox *options;
	TextInput *textInputWidth;
	TextInput *textInputHeight;
	bool needsRecreate = false;

	sf::Vector2u defaultSizeDialog{ 500, 700 };
	sf::Vector2u sizeDialog{ defaultSizeDialog };
};