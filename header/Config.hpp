#pragma once
#include "Dictionary.hpp"
#include "SFML/Window.hpp"

namespace ConfigNamespace
{
	void setDefault(Dictionary *data);
}

class Config
{
public:
	Config();
	Config(const Config &copy) = default;
	Config(sf::String source);
	AnyValue& operator () (sf::String parameterName);
private:
	Dictionary data;
	int a;
};
