#include "Config.hpp"
#include "File.hpp"

namespace ConfigNamespace
{
	void setDefault(Dictionary *data)
	{
		struct parameter {
			sf::String name;
			AnyValue value;
			parameter(sf::String name, AnyValue value) : name(name), value(value){};
		};

		parameter defaultParameter[] = {
		{	L"width",			600		},
		{	L"height",			400		},
		{	L"antialiasing",	8		}
		};

		int size = sizeof(defaultParameter) / sizeof(parameter);
		for (int i = 0; i < size; ++i)
		{
			data->Add(defaultParameter[i].name, defaultParameter[i].value, false);
		}
	}

}

Config::Config()
{
	ConfigNamespace::setDefault(&data);
}

Config::Config(sf::String source)
{
	ConfigNamespace::setDefault(&data);
	File::load(source, &data);
}

AnyValue& Config::operator () (sf::String parameterName)
{
	return data.Find(parameterName);
}