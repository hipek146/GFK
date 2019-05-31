#pragma once
#include "GUI.hpp"

class TextInput : public GUI
{
public:
	TextInput(sf::String newString, bool newText = false)
	{
		if (!font.loadFromFile("resource/fonts/SimsLLHP.ttf"))
		{
			throw L"Cannot open font file";
		}
		text.setString(newString);
		text.setFont(font);
		text.setFillColor(sf::Color::White);

		if (newText)
		{
			string = newString;
		}

		shape.setFillColor(color);
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
		target.draw(text);
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		sf::Vector2f oldSize(rect.width, rect.height);
		rect.width = newSize.x;
		rect.height = newSize.y;

		shape.setSize(newSize);

		SetTextSize();

		SetPosition({ rect.left + (oldSize.x) / 2.0f, rect.top + (oldSize.y) / 2.0f });
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x - rect.width / 2.0f;
		rect.top = newPosition.y - rect.height / 2.0f;

		shape.setPosition(rect.left, rect.top);
		sf::FloatRect textSize = text.getLocalBounds();
		text.setPosition(newPosition.x - textSize.width / 2.0f, newPosition.y - textSize.height / 2.0f - textSize.top);
	}
	virtual void SetEvents(Screen *newScreen) override
	{
		newScreen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &TextInput::TextUpdate, EventParameterType::TextInput);
		newScreen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &TextInput::TextOn, EventType::TextOn);
		newScreen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &TextInput::TextOff, EventType::TextOff);
	}
	void TextUpdate(sf::Uint32 param)
	{
		std::cout << param << std::endl;
		if (param == 8)
		{
			if (string.getSize() > 0)
			{
				string.erase(string.getSize() - 1);
			}
		}
		else if (param > 31 && param < 127)
		{
			string += param;
		}
		if (active)
		{
			text.setString(string + sf::String(L" ") + sf::String(static_cast<wchar_t>(94)));
		}
		else
		{
			text.setString(string);
		}
		SetTextSize();
		SetPosition({ rect.left + (rect.width) / 2.0f, rect.top + (rect.height) / 2.0f });
	}
	void TextOn()
	{
		active = true;
		text.setString(string + sf::String(L" ") + sf::String(static_cast<wchar_t>(94)));
		SetTextSize();
		SetPosition({ rect.left + (rect.width) / 2.0f, rect.top + (rect.height) / 2.0f });
	}
	void TextOff()
	{
		active = false;
		text.setString(string);
		SetTextSize();
		SetPosition({ rect.left + (rect.width) / 2.0f, rect.top + (rect.height) / 2.0f });
	}
	sf::String GetString() const
	{
		return string;
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
		while (textSize.width > rect.width)
		{
			characterSize -= 3;
			text.setCharacterSize(characterSize);
			textSize = text.getLocalBounds();
		}
	}
	sf::RectangleShape shape;
	sf::Color color = { 20, 20, 20, 230 };
	sf::Text text;
	sf::Font font;
	sf::String string;
	bool active = false;
};
