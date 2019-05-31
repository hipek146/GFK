#pragma once
#include "GUI.hpp"

class DialogBox : virtual public ContainerGUI, virtual public GUI
{
public:
	DialogBox(Screen *screen, sf::String string, sf::Vector2u newSize, sf::Vector2f newPosition = {})
		: ContainerGUI(newSize, { newPosition.x, newPosition.y + barHeight })
	{
		size.y -= static_cast<unsigned int>(barHeight);
		boxSize = { static_cast<float>(newSize.x), static_cast<float>(newSize.y) };
		titleBar.setFillColor(titleColor);
		background.setFillColor(backgroundColor);
		background.setOutlineThickness(4.0f);
		background.setOutlineColor(sf::Color::Black);

		parent = screen;
		rect = { newPosition, { static_cast<float>(newSize.x), static_cast<float>(newSize.y) } };
		screen->AddEvent(&rect, this, dynamic_cast<GUI*>(this), &DialogBox::BlankEvent, EventType::Blank);

		if (!font.loadFromFile("resource/fonts/SimsLLHP.ttf"))
		{
			throw L"Cannot open font file";
		}
		text.setFont(font);
		text.setFillColor(sf::Color::White);
		text.setString(string);

		Resize();
		SetDialogPosition(newPosition);
	}

	virtual ~DialogBox() = default;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(background);
		target.draw(titleBar);
		target.draw(text);

		int i = 0;
		for (auto &element : elements)
		{
			if (!hidden[i])
			{
				target.draw(*element);
			}
			++i;
		}
	}

	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		ContainerGUI::SetPosition(position);
	}
	void SetDialogPosition(const sf::Vector2f &newPosition)
	{
		ContainerGUI::SetPosition({ newPosition.x, newPosition.y + barHeight });
		titleBar.setPosition(newPosition);
		background.setPosition(newPosition);

		sf::FloatRect textSize = text.getLocalBounds();
		text.setPosition(newPosition.x + (size.x - textSize.width) / 2.0f, newPosition.y + (barHeight - textSize.height) / 2.0f - textSize.top);
	}

	virtual void SetSize(const sf::Vector2f &newSize)
	{
		size = { static_cast<unsigned int>(newSize.x), static_cast<unsigned int>(newSize.y - barHeight) };
		Resize();
	}

	virtual void Resize() override
	{
		ContainerGUI::Resize();

		boxSize = { static_cast<float>(size.x), static_cast<float>(size.y + barHeight) };

		titleBar.setSize({ boxSize.x, barHeight });
		background.setSize(boxSize);

		text.setCharacterSize(static_cast<unsigned int>(barHeight * 0.9f));

	}
	void BlankEvent() { throw 0; };
private:
	sf::Vector2f boxSize;
	sf::RectangleShape titleBar;
	sf::Text text;
	sf::Font font;
	sf::RectangleShape background;
	sf::Color titleColor = { 35, 85, 105 };
	sf::Color backgroundColor = {188, 235, 255};
	float barHeight = 30.0f;
};