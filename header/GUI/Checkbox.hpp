#pragma once
#include "GUI.hpp"

class Checkbox : public GUI
{
public:
	Checkbox(sf::String string)
	{
		if (!font.loadFromFile("resource/fonts/SimsLLHP.ttf"))
		{
			throw L"Cannot open font file";
		}
		text.setString(string);
		text.setFont(font);
		text.setFillColor(sf::Color::White);

		Xtext.setString(L"X");
		Xtext.setFont(font);
		Xtext.setFillColor(sf::Color::White);

		shape.setFillColor(color);
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
		if (active)
		{
			target.draw(Xtext);
		}
		target.draw(text);
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		sf::Vector2f oldSize(size.x, size.y);
		size = newSize;
		rect.width = size.y * 0.8f;
		rect.height = size.y * 0.8f;

		shape.setSize({ rect.width, rect.height });
		Xtext.setCharacterSize(static_cast<unsigned int>(rect.height * 1.3f));

		SetTextSize();

		SetPosition({ rect.left + (oldSize.x) / 2.0f, rect.top + (oldSize.y) / 2.0f });
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x - size.x / 2.0f + size.y * 0.1f;
		rect.top = newPosition.y - size.y / 2.0f + size.y * 0.1f;

		shape.setPosition(rect.left , rect.top);
		sf::FloatRect XtextSize = Xtext.getLocalBounds();
		Xtext.setPosition(rect.left + rect.height * 0.1f , rect.top - XtextSize.top + rect.height * 0.1f);
		sf::FloatRect textSize = text.getLocalBounds();
		text.setPosition(newPosition.x - textSize.width / 2.0f + size.y / 2.0f, newPosition.y - textSize.height / 2.0f - textSize.top);
	}
	void SetTextSize()
	{
		unsigned int characterSize = static_cast<unsigned int>(size.y * 0.6f);
		if (characterSize > 80)
		{
			characterSize = 80;
		}
		text.setCharacterSize(characterSize);
		sf::FloatRect textSize = text.getLocalBounds();
		while (size.x - size.y > 10 && textSize.width > size.x - size.y)
		{
			characterSize -= 3;
			text.setCharacterSize(characterSize);
			textSize = text.getLocalBounds();
		}
	}
	virtual void SetEvents(Screen *newScreen) override
	{
		screen = newScreen;
		screen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &Checkbox::OnClick, EventType::Pressed);
	}

	void OnClick()
	{
		if (active)
		{
			color = { 123, 93, 192, 230 };
			active = false;
		}
		else
		{
			color = { 50, 40, 90, 250 };
			active = true;
		}
		shape.setFillColor(color);
		Callback();
		callbackHandle();
	}
	virtual void Active() override
	{
		active = true;
		color = { 50, 40, 90, 250 };
		shape.setFillColor(color);
	}
	virtual void Inactive() override
	{
		active = false;
		color = { 123, 93, 192, 230 };
		shape.setFillColor(color);
	}
private:
	Screen *screen;
	sf::RectangleShape shape;
	sf::Color color = { 123, 93, 192, 230 };
	sf::Text text;
	sf::Text Xtext;
	sf::Font font;
	sf::Vector2f size;
	bool active = false;
};
