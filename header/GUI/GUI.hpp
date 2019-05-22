#pragma once
#include "SFML/Graphics.hpp"
#include "Event.hpp"
#include "Screen.hpp"

#define ADD(newElement, callback) Add(newElement, this, callback)
#define ADD_BLANK(newElement) Add(newElement, this)

class Layout : virtual public sf::Drawable
{
public:
	virtual ~Layout() = default;
	Layout() = default;
	Layout(sf::Vector2u &newSize, sf::Vector2f newPosition = {})
	{
		size = newSize;
		SetPosition(newPosition);
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	virtual void Callback(GUI* element) {};
	virtual void SetPosition(const sf::Vector2f &newPosition)
	{
		position = newPosition;
	}
	virtual void SetSize(const sf::Vector2f &newSize)
	{
		size = { static_cast<unsigned int>(newSize.x), static_cast<unsigned int>(newSize.y) };
		Resize();
	}
	virtual void Resize() {};
	virtual void Disable() {};
	virtual void Enable() {};

	friend class MenuLayout;
	friend class CreatorLayout;
	friend class CreatorMenu;

protected:
	sf::Vector2u size;
	sf::Vector2f position;
};

class GUI : virtual public sf::Drawable
{
public:

	virtual ~GUI()
	{
		if (parent != nullptr)
		{
			parent->RemoveEvent(this);
		}
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;
	virtual void SetSize(const sf::Vector2f &newSize) = 0;
	virtual void SetPosition(const sf::Vector2f &newPosition) = 0;

	virtual void Callback()
	{
		if (parentLayout != nullptr)
		{
			parentLayout->Callback(this);
		}
	}

	virtual void MouseOver() {}
	virtual void MouseOut() {}

	virtual void SetColor(sf::Color newColor) {}
	virtual void SetActiveColor(sf::Color newColor) {}

	void Disable()
	{
		if (parent != nullptr)
		{
			parent->DisableEvent(this);
		}
	}
	void Enable()
	{
		if (parent != nullptr)
		{
			parent->EnableEvent(this);
		}
	}
	friend class MenuLayout;
	friend class CreatorLayout;
	friend class CreatorMenu;
	friend class Button;

	Layout *parentLayout;
protected:
	Screen *parent;

	sf::FloatRect rect;

	void(*callback)();
};

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
		text.setFillColor(sf::Color::White);

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

		text.setCharacterSize(static_cast<unsigned int>(newSize.y * 0.6f));

		SetPosition({rect.left + ( oldSize.x) / 2.0f, rect.top + (oldSize.y) / 2.0f});
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
		over = true;
	}
	virtual void MouseOut() override
	{
		shape.setFillColor(color);
		over = false;
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
private:
	sf::RectangleShape shape;
	sf::Text text;
	sf::Font font;
	sf::Color color = { 12, 154, 220, 210 };
	sf::Color activeColor = { 38, 89, 111, 240 };
	bool over = false;
};

class CircleButton : public GUI
{
public:
	CircleButton()
	{
		circleShape.setOutlineColor(sf::Color(236, 227, 200, 250));
		MouseOut();
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(circleShape);
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		rect.width = newSize.x;
		rect.height = newSize.y;

		circleShape.setRadius(newSize.x * 0.6f);
		circleShape.setOutlineThickness(newSize.x * 0.1f);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x - rect.width / 2.0f;
		rect.top = newPosition.y - rect.height / 2.0f;
		circleShape.setPosition(newPosition.x - rect.width * 0.6f, newPosition.y - rect.height * 0.6f);
	}
	virtual void MouseOver() override
	{
		circleShape.setFillColor(sf::Color(59, 100, 166, 250));
	}
	virtual void MouseOut() override
	{
		circleShape.setFillColor(sf::Color(90, 140, 218, 250));
	}
private:
	sf::CircleShape circleShape;
};

class MenuLayout : public Layout
{
public:
	MenuLayout(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}
	~MenuLayout()
	{
		for (auto &element : elements)
		{
			delete element;
		}
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{

		for (auto &element : elements)
		{
			target.draw(*element);
		}
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		sf::Vector2f tmpPosition = newPosition;
		tmpPosition.x -= position.x;
		tmpPosition.y -= position.y;
		for (auto &element : elements)
		{
			element->SetPosition({ element->rect.left + (element->rect.width) / 2.0f + tmpPosition.x, element->rect.top + (element->rect.height) / 2.0f + tmpPosition.y });
		}
		position = newPosition;
	}
	void Add(GUI *newElement)
	{
		length++;

		sf::Vector2f newPosition, newSize;

		float min = 0.4f - 0.25f * (1 - 300 / size.y), lvl = 30, power = 2;
		if (size.y < 300)
		{
			min = 0.5f;
		}

		newSize.y = size.y * (1 - (1 - min)*(lvl + 1) / (lvl + pow(length, power))) / length;

		const float minX = 300.0f, powerX = 1.2f;
		newSize.x = size.x *(0.1f + 0.8f * (pow(minX, powerX) / pow(static_cast<float>(size.x), powerX)));

		if (!elements.empty())
		{
			Resize();

			newPosition.x = size.x / 2.0f + position.x;
			newPosition.y += elements.back()->rect.top + 1.5f * elements.back()->rect.height + 0.1f * newSize.y + position.y;
		}
		else
		{
			newPosition = { size.x / 2.0f + position.x, size.y / 2.0f + position.y};
		}
		elements.push_back(newElement);
		elements.back()->SetPosition(newPosition);
		elements.back()->SetSize({ newSize.x, newSize.y * 0.9f });
	}
	template <typename T>
	void Add(GUI *newElement, T *screen, void(T::*callback)())
	{
		Add(newElement, screen);
		screen->AddEvent(&elements.back()->rect, screen, newElement, callback, EventType::Pressed);
	}
	template <typename T>
	void Add(GUI *newElement, T *screen)
	{
		Add(newElement);
		elements.back()->parent = screen;
		screen->AddEvent(&elements.back()->rect, newElement, newElement, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&elements.back()->rect, newElement, newElement, &GUI::MouseOut, EventType::MouseOut);
	}
	virtual void Resize() override
	{
		sf::Vector2f newPosition, newSize;

		float min = 0.4f - 0.25f*(1-300/size.y), lvl = 30, power = 2;
		if (size.y < 300)
		{
			min = 0.5f;
		}
		newSize.y = size.y * (1 - (1 - min)*(lvl + 1) / (lvl + pow(length, power))) / length;
		const float minX = 300.0f, powerX = 1.2f;
		float sizeX = static_cast<float>(size.x);
		if (size.x < 300)
		{
			sizeX = 300.0f;
		}
		newSize.x = size.x *(0.1f + 0.8f * (pow(minX, powerX) / pow(sizeX, powerX)));

		if (!elements.empty())
		{
			int i = 0;
			for (auto element : elements)
			{
				element->SetSize({ newSize.x, newSize.y * 0.9f });
				element->SetPosition({ size.x / 2.0f + position.x,  size.y / 2.0f - (static_cast<int>(length / 2.0f) - i) * newSize.y + (1 - static_cast<int>(length) % 2) * newSize.y / 2.0f + position.y});
				i++;
			}
		}
	}
	virtual void Disable() override
	{

		for (auto &element : elements)
		{
			element->Disable();
		}
	}
	virtual void Enable() override
	{

		for (auto &element : elements)
		{
			element->Enable();
		}
	}

private:
	std::vector<GUI*> elements;
	float length = 0;
};

class CreatorLayout : virtual public Layout, virtual public GUI
{
public:
	template <typename T>
	CreatorLayout(T *screen, sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition)
	{
		shape.setFillColor(sf::Color(241, 162, 84, 230));

		screen->AddEvent(&rect, this, this, &CreatorLayout::BlankEvent, EventType::Blank);
		parent = screen;

		circleButton = new CircleButton();
		screen->AddEvent(&circleButton->rect, circleButton, circleButton, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&circleButton->rect, circleButton, circleButton, &GUI::MouseOut, EventType::MouseOut);
		screen->AddEvent(&circleButton->rect, this, circleButton, &CreatorLayout::Animation, EventType::Pressed);
		circleButton->parent = screen;

		SetSize({ static_cast<float>(newSize.x), static_cast<float>(newSize.y) });
		SetPosition(position);

	}
	virtual ~CreatorLayout()
	{
		delete circleButton;
	}
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		target.draw(shape);
		target.draw(*layout);
		target.draw(*circleButton);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		if (!setHidden)
		{
			hidden = false;
		}
		position = newPosition;
		shape.setPosition(newPosition);
		rect.left = newPosition.x;
		rect.top = newPosition.y;
		circleButton->SetPosition({ newPosition.x + circleButton->rect.width * 0.2f, size.y / 2.0f });
		if (layout != nullptr)
		{
			layout->SetPosition({ newPosition.x + circleButton->rect.width * 0.9f , newPosition.y});
		}

	}
	virtual void SetSize(const sf::Vector2f &newSize)
	{
		size = { static_cast<unsigned int>(newSize.x), static_cast<unsigned int>(newSize.y) };
		rect.width = newSize.x;
		rect.height = newSize.y;
		shape.setSize(newSize);
		circleButton->SetSize({ 50.0f, 50.0f });
		if (layout != nullptr)
		{
			layout->size.x = size.x - static_cast<unsigned int>(circleButton->rect.width * 0.9f);
			layout->size.y = size.y;
		}

	}
	void Add(Layout *newLayout) 
	{
		layout = newLayout;
		layout->size.x = size.x - static_cast<unsigned int>(circleButton->rect.width * 0.8f);
		layout->size.y = size.y;
		layout->Resize();
		SetPosition(position);
	}
	void Resize() {
		SetSize({ static_cast<float>(size.x), static_cast<float>(size.y) });
		if (layout != nullptr)
		{
			layout->Resize();
		}
	}
	void Animation()
	{
		if (hidden)
		{
			setHidden = true;
			SetPosition({ position.x - size.x + circleButton->rect.width, position.y });
			setHidden = false;
			hidden = false;
		}
		else
		{
			setHidden = true;
			SetPosition({ position.x + size.x - circleButton->rect.width, position.y });
			setHidden = false;
			hidden = true;
		}
	}
	virtual void Disable() override
	{
		layout->Disable();
		circleButton->Disable();
		GUI::Disable();
	}
	virtual void Enable() override
	{
		layout->Enable();
		circleButton->Enable();
		GUI::Enable();
	}
	void BlankEvent() { throw 0; }
protected:
	Layout *layout;
	sf::RectangleShape shape;
	GUI *circleButton;
	bool hidden = false;
	bool setHidden = false;
};

class CreatorMenu : public Layout
{
public:
	CreatorMenu(sf::Vector2u &newSize, sf::Vector2f newPosition = {}) : Layout(newSize, newPosition) {}
	~CreatorMenu()
	{
		for (auto button : buttons)
		{
			delete button;
		}
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		if (active >= 0)
		{
			for (auto button : buttons)
			{
				target.draw(*button);
			}
			target.draw(*layouts[active]);
		}
	}
	template <typename T>
	void Add(GUI *newButton, T* screen, Layout* newLayout)
	{
		buttons.push_back(newButton);
		layouts.push_back(newLayout);
		++count;
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::MouseOver, EventType::MouseOver);
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::MouseOut, EventType::MouseOut);
		screen->AddEvent(&buttons.back()->rect, newButton, newButton, &GUI::Callback, EventType::Pressed);
		newButton->parentLayout = this;
		newButton->parent = screen;
		newButton->SetColor({ 202, 90, 155, 210 });
		newButton->SetActiveColor({ 190, 62, 136, 240 });
		Resize();
		if (count > 1)
		{
			newLayout->Disable();
		}
		else
		{
			SetActive(0);
		}
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		position = newPosition;
		float i = 0;
		for (auto button : buttons)
		{
			if (i <= active)
			{
				button->SetPosition({ newPosition.x + size.x / 2.0f, newPosition.y + buttonSize.y / 2.0f + buttonSize.y * i});
			}
			else
			{
				button->SetPosition({ newPosition.x + size.x / 2.0f, newPosition.y + buttonSize.y / 2.0f + buttonSize.y * i + layouts[active]->size.y });
			}
			++i;
		}
		for (auto layout : layouts)
		{
			layout->SetPosition({ newPosition.x,  newPosition .y + (active + 1) * buttonSize.y });
		}
	}
	virtual void Resize() override
	{
		buttonSize.x = static_cast<float>(size.x) * 0.95f;
		buttonSize.y = 42.0f;
		for (auto button : buttons)
		{
			button->SetSize({ buttonSize.x, buttonSize.y * 0.9f});
		}
		for (auto layout : layouts)
		{
			layout->size.x = size.x;
			layout->size.y = size.y - static_cast<unsigned int>(count * buttonSize.y);
			layout->Resize();
		}
	}
	virtual void Callback(GUI *element) override
	{
		int i = 0;
		for (auto button : buttons)
		{
			if (button == element)
			{
				SetActive(i);
				break;
			}
			++i;
		}
	}
	void SetActive(int number)
	{
		layouts[active]->Disable();
		buttons[active]->SetColor({ 202, 90, 155, 210 });
		buttons[active]->SetActiveColor({ 190, 62, 136, 240 });
		active = number;
		layouts[active]->Enable();
		buttons[active]->SetColor({ 212, 45, 83, 210 });
		buttons[active]->SetActiveColor({ 212, 45, 83, 210 });
		SetPosition(position);
	}
private:
	int active = 0;
	float count = 0;
	std::vector<GUI*> buttons;
	std::vector<Layout*> layouts;
	sf::Vector2f buttonSize;
};

class Touchpad : public GUI, public sf::Transformable
{
public:
	Touchpad(Screen *setParent, sf::Vector2u *newSize)
	{
		parent = setParent;
		size = newSize;
		SetSize({ static_cast<float>(newSize->x), static_cast<float>(newSize->y) });
	}
	virtual ~Touchpad(){}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
	}
	virtual void SetSize(const sf::Vector2f &newSize) override
	{
		rect.width = newSize.x;
		rect.height = newSize.y;
	}
	virtual void Resize()
	{
		rect.width = static_cast<float>(size->x);
		rect.height = static_cast<float>(size->y);
	}
	virtual void SetPosition(const sf::Vector2f &newPosition) override
	{
		rect.left = newPosition.x;
		rect.top = newPosition.y;
		setPosition(newPosition);
	}
	template <typename T>
	void onMouseMove(T *screen, void(T::*callback)())
	{
		screen->AddEvent(&(this->rect), screen, this, callback, EventType::MouseMove);
	}
	template <typename T>
	void onClick(T *screen, void(T::*callback)())
	{
		screen->AddEvent(&(this->rect), screen, this, callback, EventType::Pressed);
	}
protected:
	sf::Vector2u *size;
};