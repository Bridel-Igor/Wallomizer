#include <windows.h>

#include "Wallpaper.h"

Wallpaper::Wallpaper(CollectionType _type)
{
	type = CollectionType::none;
	charX.a = nullptr;

	switch (_type)
	{
		case CollectionType::local:
		{
			charX.w = new wchar_t[MAX_PATH];
			if (charX.w != nullptr)
				type = _type;
		}
		break;
		case CollectionType::user:
		{
			charX.a = new char[255];
			if (charX.a != nullptr)
				type = _type;
		}
		break;
		case CollectionType::search:
		{
			charX.a = new char[1024];
			if (charX.a != nullptr)
				type = _type;
		}
		break;
	}
}

Wallpaper::~Wallpaper()
{
	type = CollectionType::none;
	if (charX.w != nullptr)
	{
		delete[] charX.w;
		charX.w = nullptr;
	}
	if (charX.a != nullptr)
	{
		delete[] charX.a;
		charX.a = nullptr;
	}
}

CollectionType Wallpaper::getType() const
{
	if (this == nullptr)
		return CollectionType::none;
	return type;
}

char* Wallpaper::getPathA() const
{
	if (this == nullptr)
		return nullptr;
	return charX.a;
}

wchar_t* Wallpaper::getPathW() const
{
	if (this == nullptr)
		return nullptr;
	return charX.w;
}