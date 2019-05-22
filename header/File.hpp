#pragma once
#include "Dictionary.hpp"
#include <fstream>

namespace File
{
	bool blank(const wchar_t character)
	{
		switch (character)
		{
		case L':':
			return true;
		case L' ':
			return true;
		case L'\n':
			return true;
		case L';':
			return true;
		case L'\0':
			return true;
		}
		return false;
	}
	void load(sf::String path, Dictionary *data)
	{
		std::wifstream file;
		file.open((std::string)path);
		if (file)
		{
			file.seekg(0, file.end);
			int length = static_cast<int>(file.tellg());
			file.seekg(0, file.beg);
			length -= static_cast<int>(std::count(std::istreambuf_iterator<wchar_t>(file), std::istreambuf_iterator<wchar_t>(), '\n'));
			file.seekg(0, file.beg);
			wchar_t *buffer = new wchar_t[length + 1];

			wchar_t bufforName[265];
			wchar_t bufforValue[256];
			bool flag = true;
			unsigned short i = 0, k = 0;
			int a = 0;
			file.read(buffer, length);
			buffer[length] = '\0';
		
			while (i < length - 1)
			{
				k = 0;
				while (!blank(buffer[i]) || !k)
				{
					if (!blank(buffer[i]))
					{
						bufforName[k] = buffer[i];
						++k;
					}
					++i;
				}
				bufforName[k] = L'\0';

				k = 0;
				while (!blank(buffer[i]) || !k)
				{
					if (!blank(buffer[i]))
					{
						bufforValue[k] = buffer[i];
						++k;
					}
					++i;
				}
				bufforValue[k] = L'\0';
				a++;
				data->Add(bufforName, bufforValue);
			}
			file.close();
			delete[] buffer;
		}
		else
		{
			file.close();
			// sf::String("Cannot read file");
		}
	}
}