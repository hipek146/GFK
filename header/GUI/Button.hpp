#pragma once
#include "GUI.hpp"

class Button : public GUI
{
public:
	Button(sf::String string)
	{
		if (!font.loadFromFile("resource/fonts/SimsLLHP.ttf"))
		{
			throw L"Cannot open font file";
		}
		text.setString(string);
		text.setFont(font);
		text.setFillColor(textColor);

		MouseOut();
	}
	~Button()
	{
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
	virtual void MouseOver() override
	{
		shape.setFillColor(activeColor);
		text.setFillColor(activeTextColor);
		over = true;
	}
	virtual void MouseOut() override
	{
		if (!active)
		{
			shape.setFillColor(color);
			text.setFillColor(textColor);
		}
		over = false;
	}
	virtual void Active() override
	{
		active = true;
		MouseOver();
	}
	virtual void Inactive() override
	{
		active = false;
		MouseOut();
	}
	void SetColor(sf::Color newColor)
	{
		color = newColor;
		if (over) MouseOver();
		else MouseOut();
	}
	void SetActiveColor(sf::Color newColor)
	{
		activeColor = newColor;
		if (over) MouseOver();
		else MouseOut();
	}
	virtual void SetTextColor(sf::Color newColor) 
	{
		textColor = newColor;
		if (over) MouseOver();
		else MouseOut();
	}
	virtual void SetActiveTextColor(sf::Color newColor)
	{
		activeTextColor = newColor;
		if (over) MouseOver();
		else MouseOut();
	}
	virtual void SetEvents(Screen *newScreen) override
	{
		parent = newScreen;
		parent->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &Button::OnClick, EventType::Pressed);
	}

	void OnClick()
	{
		Callback();
		callbackHandle();
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
	sf::Text text;
	sf::Font font;
	sf::Color color = { 12, 154, 220, 210 };
	sf::Color activeColor = { 38, 89, 111, 240 };
	sf::Color textColor = sf::Color::White;
	sf::Color activeTextColor = sf::Color::White;
	bool over = false;
	bool active = false;
};