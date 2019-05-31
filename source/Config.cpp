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
		{	L"width",			L"600"	},
		{	L"height",			L"400"	},
		{	L"antialiasing",	L"8"		}
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

Config::Config(sf::String source) : source(source)
{
	ConfigNamespace::setDefault(&data);
	File::load(source, &data);
	Save();
}

AnyValue& Config::operator () (sf::String parameterName)
{
	return data.Find(parameterName);
}

void Config::operator () (sf::String parameterName, AnyValue value)
{
	data.Add(parameterName, value, true);
}

bool Config::Save()
{
	std::wofstream file;
	file.open((std::string)source);
	if (file)
	{
		file << L"width : " << &((sf::String)data.Find(L"width")).toWideString()[0] << std::endl;
		file << L"height : " << &((sf::String)data.Find(L"height")).toWideString()[0] << std::endl;
		file << L"antialiasing : " << &((sf::String)data.Find(L"antialiasing")).toWideString()[0] << std::endl;

		file.close();
		return true;
	}
	return false;
}
