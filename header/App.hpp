#pragma once
#include "Config.hpp"
#include "Event.hpp"
#include "SFML/Graphics.hpp"
#include "Workspace.hpp"
class Screen;
class DialogBox;
class VectorGUI;
class TextInput;
class Data;
class Workspace;

class App : public sf::Drawable
{
public:
	App(Event *event, Config *config);
	~App();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void LoadScreen(Screen *screenToLoaded);
	void Resize();

	friend class Screen;
	friend class Menu;
	friend class Creator;

	void LoadDialog();
	void CloseLoadDialog();
	void WaterInfoDialog();
	void CloseWaterInfoDialog();
	void Load();
	void SaveDialog();
	void CloseSaveDialog();
	void Save();

	sf::Vector2u *size;
	Event *event;

private:
	void Blank() {}

	Data *data;
	Workspace *workspace;
	Screen *menu;
	Screen *creator;
	
	Screen *screen;

	Config *config;

	VectorGUI *layoutSave;
	TextInput *textInputSave;
	DialogBox *loadDialog;
	DialogBox *waterInfoDialog;
	DialogBox *saveDialog;
	sf::Vector2u defaultSizeDialog{ 500, 600 };
	sf::Vector2u sizeDialog{ defaultSizeDialog };
	std::vector<std::wstring> filenames;
};