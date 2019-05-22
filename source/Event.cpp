#include "Event.hpp"


Event::Event(sf::RenderWindow &windowReference)
{
	cursor.loadFromSystem(sf::Cursor::Arrow);
	if (cursorHand.loadFromSystem(sf::Cursor::Hand))
	{
		cursorHandOK = true;
	}
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
	for (auto &element : handle)
	{
		if (element.rect != nullptr && element.rect->contains(mouse.x, mouse.y))
		{
			if (element.disable)
			{
				continue;
			}
			if (event.type == sf::Event::MouseButtonPressed && element.isOk[EventType::Pressed])
			{
				element.callbackHandle[EventType::Pressed]();
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (element.isOk[EventType::MouseMove])
				{
					element.callbackHandle[EventType::MouseMove]();
				}
				if (!element.mouseOver && element.isOk[EventType::MouseOver])
				{
					if (cursorHandOK) window->setMouseCursor(cursorHand);

					element.callbackHandle[EventType::MouseOver]();
					element.mouseOver = true;
				}
			}
			break;
		}
	}
	for (auto &element : handle)
	{
		if (element.mouseOver && element.rect != nullptr && !element.rect->contains(mouse.x, mouse.y))
		{
			if (element.isOk[EventType::MouseOut])
			{
				element.callbackHandle[EventType::MouseOut]();
			}
			window->setMouseCursor(cursor);

			element.mouseOver = false;
		}
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
				if (iterator->mouseOver) window->setMouseCursor(cursor);
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