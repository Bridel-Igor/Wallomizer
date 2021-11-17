#include <windows.h>

#include "Wallpaper.h"

Wallpaper::Wallpaper(CollectionType _type)
{
	type = CollectionType::none;
	dataW = nullptr;
	dataA = nullptr;

	switch (_type)
	{
		case CollectionType::local:
		{
			dataW = new wchar_t[MAX_PATH];
			if (dataW != nullptr)
				type = _type;
		}
		break;
		case CollectionType::user:
		{
			dataA = new char[255];
			if (dataA != nullptr)
				type = _type;
		}
		break;
		case CollectionType::search:
		{
			dataA = new char[1024];
			if (dataA != nullptr)
				type = _type;
		}
		break;
	}
}

Wallpaper::~Wallpaper()
{
	type = CollectionType::none;
	if (dataW != nullptr)
	{
		delete[] dataW;
		dataW = nullptr;
	}
	if (dataA != nullptr)
	{
		delete[] dataA;
		dataA = nullptr;
	}
}

CollectionType Wallpaper::getType()
{
	if (this == nullptr)
		return CollectionType::none;
	return type;
}

char* Wallpaper::getPathA()
{
	if (this == nullptr)
		return nullptr;
	return dataA;
}

wchar_t* Wallpaper::getPathW()
{
	if (this == nullptr)
		return nullptr;
	return dataW;
}