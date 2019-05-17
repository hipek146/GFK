#include "Dictionary.hpp"

Dictionary::Dictionary()
{
	head = new Element;
}

Dictionary::~Dictionary()
{
	delete head;
}
Element* Dictionary::GetElement(sf::String name) 
{
	ElementList *ptrList = head->list;
	size_t nameSize = name.getSize();
	int i = 0;
	while (ptrList != nullptr)
	{
		if (ptrList->character == name[i])
		{
			++i;
			if (i >= nameSize)
			{
				if (ptrList->element->value == nullptr)
				{
					throw sf::String(L"Element doesn't exist");
				}
				else
				{
					return ptrList->element;
				}
			}
			ptrList = ptrList->element->list;
			continue;
		}
		ptrList = ptrList->next;
	}
	throw sf::String(L"Element doesn't exist");
}
AnyValue& Dictionary::Find(sf::String name)
{
	Element *ptr;
	try 
	{
		ptr = GetElement(name);
		return *(ptr->value);
	}
	catch(sf::String error)
	{
		throw L"Can't find value: \"" + name + "\" " + error;
	}
}
void Dictionary::Add(sf::String name, AnyValue value, bool overwrite)
{
	Element *ptr = head;
	ElementList *ptrList = head->list;
	ElementList *lastList = nullptr;
	size_t nameSize = name.getSize();
	int i = 0;
	while(ptrList != nullptr)
	{
		lastList = ptrList;
		if (ptrList->character == name[i])
		{
			++i;
			if (i >= nameSize)
			{
				if (ptrList->element->value != nullptr)
				{
					if (overwrite)
					{
						delete ptrList->element->value;
						ptrList->element->value = new AnyValue(value);
						return;
					}
					else
					{
						throw sf::String(L"Element already exists");
					}
				}
				else
				{
					ptrList->element->value = new AnyValue(value);
					return;
				}
			}
			ptr = ptrList->element;
			ptrList = ptrList->element->list;
			continue;
		}
		ptrList = ptrList->next;
	}
	if (lastList && i < nameSize)
	{
		lastList->next = new ElementList;
		lastList->next->character = name[i];
		lastList->next->element = new Element;
		ptr = lastList->next->element;
		++i;
	}
	while (i < nameSize)
	{
		ptr->list = new ElementList;
		ptr->list->character = name[i];
		ptr->list->element = new Element;
		ptr = ptr->list->element;
		++i;
	}
	ptr->value = new AnyValue(value);
}


ElementList::~ElementList() { delete element; delete next; };
Element::~Element() { delete value; delete list; };