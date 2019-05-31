#pragma once
#include "GUI.hpp"

class TextBox : public GUI
{
public:
	TextBox(sf::String newString)
	{
		if (!font.loadFromFile("resource/fonts/SimsLLHP.ttf"))
		{
			throw L"Cannot open font file";
		}
		text.setFont(font);
		text.setFillColor(color);
		SetText(newString);
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(text);
	}
	virtual void SetSize(const sf::Vector2f &newSize)
	{
		rect.width = newSize.x;
		rect.height = newSize.y;
		SetTextSize();
	}
	virtual void SetPosition(const sf::Vector2f &newPosition)
	{
		rect.left = newPosition.x;
		rect.top = newPosition.y;
		sf::FloatRect textSize = text.getLocalBounds();
		text.setPosition(newPosition.x - textSize.width / 2.0f, newPosition.y - textSize.height / 2.0f - textSize.top);
	}

	void SetText(sf::String newString)
	{
		string = newString;
		text.setString(string);
	}
	virtual void SetEvents(Screen *newScreen) override
	{
		parent = newScreen;
		Disable();
	}
private:
	void SetTextSize()
	{
		unsigned int characterSize = static_cast<unsigned int>(rect.height * 0.6f);
		if (characterSize > 60)
		{
			characterSize = 60;
		}
		text.setCharacterSize(characterSize);
		sf::FloatRect textSize = text.getLocalBounds();
		while (textSize.width > rect.width || textSize.height > rect.height)
		{
			characterSize -= 3;
			text.setCharacterSize(characterSize);
			textSize = text.getLocalBounds();
		}
	}
	sf::Color color = { 20, 20, 20, 230 };
	sf::Text text;
	sf::Font font;
	sf::String string;
};