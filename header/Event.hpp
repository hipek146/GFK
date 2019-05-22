﻿#pragma once
#include "SFML/Graphics.hpp"
#include <functional>
#include <deque>
#include <iostream>

class Screen;
class Menu;
class GUI;
class App;

namespace EventType
{
	enum : int
	{
		Blank,
		Pressed,
		MouseOver,
		MouseOut,
		MouseMove,
		Resize,

		count
	};
}

class EventHandle
{
public:
	friend class Event;

	template <typename T>
	EventHandle(sf::FloatRect *rect, T *screen, GUI *elementHandle, void(T::*callback)(), int type)
		: rect(rect), elementHandle(elementHandle)
	{
		Add(screen, callback, type);
	}
	template <typename T>
	void Add(T *screen, void(T::*callback)(), int type)
	{
		callbackHandle[type] = std::bind(callback, screen);
		isOk[type] = true;
	}
private:
	sf::FloatRect *rect;
	std::function<void()> callbackHandle[EventType::count];
	bool isOk[EventType::count] = {};
	GUI *elementHandle;
	bool mouseOver = false;
	bool disable = false;
};

class Event
{
public:
	/////////////////////////////// Event /////////////////////////////////
	/// \brief Konstruktor, wczytuje kursory
	///
	/// \param windowReference Wskaźnik do okna dla którego obsługuje się zdarzenia
	///
	///////////////////////////////////////////////////////////////////////
	Event(sf::RenderWindow &windowReference);

	///////////////// operator rzutownania  sf::Event & ///////////////////
	/// \brief Pozwala na dostęp do wbudowanego Event w SFML
	///
	/// \return połączony obiekt biblioteki SFML do obsługi zdarzeń
	///
	///////////////////////////////////////////////////////////////////////
	operator sf::Event &();

	/////////////////////////////// operator () ////////////////////////////////
	/// \brief	Funkcja, obsługuje zdarzenia SFML dal obiektu event
	///
	///////////////////////////////////////////////////////////////////////
	void operator () ();

	/////////////////////////////// Add ///////////////////////////////////
	/// \brief Dodaje zdarzenie do pojemnika (handle)
	///
	/// \param rect Współżędne elementu GUI
	/// \param screen Wskażnik do ekranu (Screen) zawierającego funkcję callback
	/// \param element Wskaźnik do elementu GUI, dla którego inicjuje się zdarzenie
	/// \param callback Wskaźnik do funkcji callback
	///
	//////////////////////////////////////////////////////////////////////
	template <typename T>
	void Add(sf::FloatRect *rect, T *screen, GUI *newElement, void(T::*callback)(), int type)
	{
		for (auto &element : handle)
		{
			if (element.elementHandle == newElement)
			{
				element.Add(screen, callback, type);
				return;
			}
		}
		handle.push_front({rect, screen, newElement, callback, type});
	}

	///////////////////////////// Disable /////////////////////////////////
	/// \brief Ustawia zdarzenia jako nieaktywne
	///
	/// \param elementToRemove Wskaźnik na element GUI, którego zdarzenia mają zostać uznane za nieaktywne
	///
	//////////////////////////////////////////////////////////////////////
	void Disable(GUI *elementToDisable);

	///////////////////////////// Disable /////////////////////////////////
	/// \brief Ustawia zdarzenia jako aktywne (domyślnie)
	///
	/// \param elementToRemove Wskaźnik na element GUI, którego zdarzenia mają zostać uznane za aktywne
	///
	//////////////////////////////////////////////////////////////////////
	void Enable(GUI *elementToEnable);

	///////////////////////////// Remove /////////////////////////////////
	/// \brief Usuwa zdarzenia z pojemnika (handle)
	///
	/// \param elementToRemove Wskaźnik na element GUI, którego zdarzenia mają zostać usunięte
	///
	//////////////////////////////////////////////////////////////////////
	void Remove(GUI *elementToRemove);

	//////////////////////////////window//////////////////////////////////
	/// \brief Wskaźnik do okna dla którego obsługuje się darzenia
	///
	//////////////////////////////////////////////////////////////////////
	sf::RenderWindow *window;
	sf::Vector2u size;


	struct {
		int x = 0;
		int y = 0;
		bool leftClick;
	}mouse;

private:
	void FindAndSet(GUI *elementToSet, int setType);

	sf::Event event;
	sf::Cursor cursor;
	sf::Cursor cursorHand;
	bool cursorHandOK = false;
	std::deque<EventHandle> handle;

};
