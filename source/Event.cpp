#include "Event.hpp"


Event::Event(sf::RenderWindow &windowReference)
{
	cursor[CursorType::Arrow].loadFromSystem(sf::Cursor::Arrow);
	cursorOK[CursorType::Arrow] = true;
	if (cursor[CursorType::Hand].loadFromSystem(sf::Cursor::Hand))
	{
		cursorOK[CursorType::Hand] = true;
	}
	cursorSet = CursorType::Arrow;
	cursorActive = cursorSet;

	window = &windowReference;
	size = window->getSize();
}

Event::operator sf::Event &() 
{
	return event;
}

void Event::operator () ()
{
	if (event.type == sf::Event::Closed)
	{
		window->close();
	}
	else if (event.type == sf::Event::MouseMoved)
	{
		mouse.x = event.mouseMove.x;
		mouse.y = event.mouseMove.y;
	}
	else if (event.type == sf::Event::Resized)
	{
		sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
		window->setView(sf::View(visibleArea));
		size = window->getSize();;
		for (auto &element : handle)
		{
			if (element.isOk[EventType::Resize])
			{
				element.callbackHandle[EventType::Resize]();
			}
		}
	}
	else if (event.type == sf::Event::TextEntered)
	{
		for (auto &element : handle)
		{
			if (element.textInput && element.isParameterOk[EventParameterType::TextInput])
			{
				element.callbackParameterHandle[EventParameterType::TextInput](event.text.unicode);
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left)
		{
			mouse.left = true;
		}
		else
		{
			mouse.right = true;
		}

		if (textInputPtr != nullptr && textInputPtr->textInput)
		{
			textInputPtr->textInput = false;
			if (textInputPtr->isOk[EventType::TextOff])
			{
				textInputPtr->callbackHandle[EventType::TextOff]();
			}
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		mouse.left = false;
		mouse.right = false;
	}
	for (auto &element : handle)
	{
		if (element.disable)
		{
			continue;
		}
		if (element.rect != nullptr && element.rect->contains(mouse.x, mouse.y))
		{
			if (event.type == sf::Event::MouseButtonPressed)
			{
				if (element.isOk[EventType::Pressed])
				{
					element.callbackHandle[EventType::Pressed]();
				}
				if (element.isParameterOk[EventParameterType::TextInput])
				{
					textInputPtr = &element;
					textInputPtr->textInput = true;
					if (textInputPtr->isOk[EventType::TextOn])
					{
						textInputPtr->callbackHandle[EventType::TextOn]();
					}
				}
			}
			else if (event.type == sf::Event::MouseButtonReleased && element.isOk[EventType::Released])
			{
				element.callbackHandle[EventType::Released]();
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (element.isOk[EventType::MouseMove])
				{
					element.callbackHandle[EventType::MouseMove]();
				}
				if (!element.mouseOver && element.isOk[EventType::MouseOver])
				{
					if (cursorOK[CursorType::Hand]) cursorSet = CursorType::Hand;

					element.callbackHandle[EventType::MouseOver]();
					element.mouseOver = true;
				}
			}
			break;
		}
	}
	int length = static_cast<int>(handle.size());
	bool flag = false;
	for (auto &element : handle)
	{
		if (element.disable)
		{
			length--;
			if (length == 0)
			{
				cursorSet = CursorType::Arrow;
			}
			continue;
		}
		if (flag == false && element.rect != nullptr && element.rect->contains(mouse.x, mouse.y))
		{
			flag = true;
			if (!element.isOk[EventType::MouseOver])
			{
				--length;
				if (length == 0)
				{
					cursorSet = CursorType::Arrow;
				}
			}
			continue;
		}
		if (element.mouseOver && element.rect != nullptr && (!element.rect->contains(mouse.x, mouse.y) || flag))
		{
			if (element.isOk[EventType::MouseOut])
			{
				element.callbackHandle[EventType::MouseOut]();
			}		
			element.mouseOver = false;
		}
		--length;
		if (length == 0)
		{
			cursorSet = CursorType::Arrow;
		}
	}
	if (cursorSet != cursorActive)
	{
		window->setMouseCursor(cursor[cursorSet]);
		cursorActive = cursorSet;
	}
}

void Event::Remove(GUI *elementToRemove)
{
	FindAndSet(elementToRemove, 0);
}

void Event::Disable(GUI *elementToDisable)
{
	FindAndSet(elementToDisable, 1);
}

void Event::Enable(GUI *elementToEnable)
{
	FindAndSet(elementToEnable, 2);
}

void Event::FindAndSet(GUI *elementToSet, int setType)
{
	for (auto iterator = handle.begin(); iterator != handle.end(); )
	{
		if (iterator->elementHandle == elementToSet)
		{
			if (setType == 0)
			{
				if (iterator->mouseOver) window->setMouseCursor(cursor[CursorType::Arrow]);
				handle.erase(iterator);
			}
			else if (setType == 1)
			{
				iterator->disable = true;
			}
			else if (setType == 2)
			{
				iterator->disable = false;
			}
			return;
		}
		else
		{
			++iterator;
		}
	}
}