#pragma once
#include "SFML/Window.hpp"

class AnyValue
{
public:
	AnyValue() = default;
	AnyValue(bool toSet) { Set(toSet); }
	AnyValue(int toSet) { Set(toSet); }
	AnyValue(unsigned int toSet) { Set(toSet); }
	AnyValue(double toSet) { Set(toSet); }
	AnyValue(wchar_t const *toSet) { Set((sf::String)toSet); }
	AnyValue(sf::String toSet) { Set(toSet); }

	operator bool() { return static_cast<bool>(doubleValue); }
	operator int() { return static_cast<int>(doubleValue); }
	operator unsigned int() { return abs(static_cast<int>(doubleValue)); }
	operator double() { return doubleValue; }
	operator sf::String() { return stringValue; }

	void Set(bool toSet) { doubleValue = static_cast<double>(toSet); stringValue = std::to_string(doubleValue); }
	void Set(int toSet) { doubleValue = static_cast<double>(toSet); stringValue = std::to_string(doubleValue); }
	void Set(unsigned int toSet) { doubleValue = static_cast<double>(toSet); stringValue = std::to_string(doubleValue); }
	void Set(double toSet) { doubleValue = toSet; stringValue = std::to_string(doubleValue); }
	void Set(sf::String toSet) { stringValue = toSet; try { doubleValue = std::stod((std::string)stringValue); } catch (...) {} }

private:
	double doubleValue;
	sf::String stringValue;
};

struct Element;

struct ElementList
{
	~ElementList();
	wchar_t character;
	ElementList *next = nullptr;
	Element *element = nullptr;
};

struct Element
{
	~Element();
	ElementList *list = nullptr;
	AnyValue *value = nullptr;
};

class Dictionary
{
public:
	Dictionary();
	~Dictionary();
	AnyValue& Find(sf::String name);
	void Add(sf::String name, AnyValue value, bool overwrite = true);
private:
	Element* GetElement(sf::String name);
	Element *head;
};