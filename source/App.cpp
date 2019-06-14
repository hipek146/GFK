#include "App.hpp"
#include "Menu.hpp"
#include "Creator.hpp"
#include <filesystem>
#include <fstream>

App::App(Event *event, Config *config) : event(event), config(config)
{
	size = &event->size;

	event->Add(nullptr, this, nullptr, &App::Resize, EventType::Resize); //dodaje zdarzenie rezise dla aplikacji 

	menu = new Menu(this); //tworzy okno menu aplikacji, korzysta z dziedziczenia Screen 
	creator = new Creator(this);//tworzy okno kreatora aplikacji, korzysta z dziedziczenia Screen 
	LoadScreen(menu);//laduje okno menu 
}

App::~App()
{
	if (screen != nullptr)
	{
		delete loadDialog;
		delete saveDialog;
		screen->ClearScreen();
	}
	delete menu;
	delete creator;
}

void App::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*screen);
	if (loadDialog != nullptr)
	{
		target.draw(*loadDialog);
	}
	else if (saveDialog != nullptr)
	{
		target.draw(*saveDialog);
	}
}

void App::LoadScreen(Screen *screenToLoaded)
{
	if (screen != nullptr)
	{
		delete loadDialog;
		loadDialog = nullptr;
		delete saveDialog;
		saveDialog = nullptr;
		screen->ClearScreen();
	}
	screen = screenToLoaded;
	screen->CreateScreen(); //Tutaj zaczyna tworzyc sie ekran 
}

void App::Resize()
{
	screen->ResizeScreen();
}

void App::LoadDialog()
{
	CloseSaveDialog();
	CloseLoadDialog();
	filenames.clear();

	loadDialog = new DialogBox(screen, L"Wczytaj teren", { sizeDialog }, { static_cast<float>(size->x - sizeDialog.x) / 2.0f, static_cast<float>(size->y - sizeDialog.y) / 2.0f });
	std::string path = "saves";
	
	layoutSave = new VectorGUI(*size);
	int i = 0;
	if (!std::filesystem::is_directory(path))
	{
		std::filesystem::create_directory(path);
	}
	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		Button *button = new Button((std::wstring)entry.path().filename());
		layoutSave->Add(button, screen, &Screen::Blank);
		event->Add(&button->rect, this, button, &App::Load, EventType::Pressed);
		button->SetEvents(screen);
		button->callbackHandle = std::bind(&App::Load, this);
		button->parent = screen;
		button->parentLayout = layoutSave;
		filenames.push_back((std::wstring)entry.path().filename());
	}
	layoutSave->Vertical();
	layoutSave->SetPadding(5.0f);
	layoutSave->SetSpace(3.0f);
	loadDialog->Add(screen, layoutSave);
	loadDialog->SetElementSizeY(1, static_cast<float>(sizeDialog.y) - 200.0f);

	VectorGUI *cancel = new VectorGUI(*size);
	Button *cancleButton = new Button(L"Anuluj");
	cancel->Add(cancleButton, screen, &Screen::Blank);
	event->Add(&cancleButton->rect, this, cancleButton, &App::CloseLoadDialog, EventType::Pressed);
	cancel->SetPadding(30.0f);
	loadDialog->Add(screen, cancel);
	loadDialog->SetBottom(2);
	loadDialog->SetElementSizeY(2, 120.0f);
}

void App::CloseLoadDialog()
{
	delete loadDialog;
	loadDialog = nullptr;
}

void App::Load()
{
	std::wifstream file;
	file.open(std::wstring(L"saves\\") + filenames[layoutSave->lastPressed]);
	
	Segment segment;
	if (file)
	{
		file.seekg(0, file.end);
		int length = static_cast<int>(file.tellg());
		file.seekg(0, file.beg);
		length -= static_cast<int>(std::count(std::istreambuf_iterator<wchar_t>(file), std::istreambuf_iterator<wchar_t>(), '\n'));
		file.seekg(0, file.beg);
		wchar_t *buffer = new wchar_t[length + 1];

		creator->ClearScreen();
		creator->CreateScreen();

		file.read(buffer, length);
		buffer[length] = '\0';
		int y;
		int x;
		x = _wtoi(&buffer[0]);
		bool flag = true;
		for (int i = 0; i < length; ++i) {
			if (buffer[i] == '\n' && flag == false) {
		
				x = _wtoi(&buffer[i]);
				flag = true;
			}
			else if (buffer[i] == '\n' && flag == true) {
				y = _wtoi(&buffer[i]);
				workspace->mainPoints.push_back(sf::Vector2f(x, y));
				std::cout << x << std::endl << y << std::endl;
				segment.points.push_back(sf::Vector2f(x, y));
				flag = false;
			}
			
		}
		segment.rect = sf::FloatRect({ workspace->mainPoints[0].x, 0 }, sf::Vector2f(size->x, size->y));
		data->Add(segment);

		workspace->Update();
		CloseLoadDialog();
		file.close();
		delete[] buffer;
	}
}

void App::SaveDialog()
{
	CloseSaveDialog();
	CloseLoadDialog();

	saveDialog = new DialogBox(screen, L"Zapisz teren", { sizeDialog }, { static_cast<float>(size->x - sizeDialog.x) / 2.0f, static_cast<float>(size->y - sizeDialog.y) / 2.0f });


	VectorGUI *input = new VectorGUI(*size);
	textInputSave = new TextInput(L"Podaj nazwe");
	input->Add(textInputSave, screen, &Screen::Blank);
	Button *saveButton = new Button(L"Zapisz");
	input->Add(saveButton, screen, &Screen::Blank);
	event->Add(&saveButton->rect, this, saveButton, &App::Save, EventType::Pressed);
	input->Vertical();
	input->SetPadding(10.0f);
	input->SetSpace(15.0f);
	saveDialog->Add(screen, input);
	saveDialog->SetElementSizeY(1, 200.0f);

	VectorGUI *cancel = new VectorGUI(*size);
	Button *cancleButton = new Button(L"Anuluj");
	cancel->Add(cancleButton, screen, &Screen::Blank);
	event->Add(&cancleButton->rect, this, cancleButton, &App::CloseSaveDialog, EventType::Pressed);
	cancel->SetPadding(30.0f);
	saveDialog->Add(screen, cancel);
	saveDialog->SetBottom(2);
	saveDialog->SetElementSizeY(2, 120.0f);
}

void App::CloseSaveDialog()
{
	delete saveDialog;
	saveDialog = nullptr;
}

void App::Save()
{
	//creator->
	std::wofstream file;
	file.open(std::wstring(L"saves\\") + textInputSave->GetString().toWideString());
	if (file)
	{
		for (auto &it : workspace->mainPoints) {

				file << it.x << "\n" << it.y << "\n";

		}
		
		//file << L"To jest plik o nazwie " << textInputSave->GetString().toWideString();
		file.close();

	}

	CloseSaveDialog();
}

