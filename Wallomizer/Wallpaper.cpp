#include <windows.h>

#include "Wallpaper.h"

Wallpaper::Wallpaper(CollectionType _type)
{
	type = CollectionType::none;
	m_wsParameter = nullptr;

	switch (_type)
	{
		case CollectionType::local:
		{
			m_wsParameter = new wchar_t[MAX_PATH];
			if (m_wsParameter != nullptr)
				type = _type;
		}
		break;
		case CollectionType::user:
		{
			m_wsParameter = new wchar_t[255];
			if (m_wsParameter != nullptr)
				type = _type;
		}
		break;
		case CollectionType::search:
		{
			m_wsParameter = new wchar_t[1024];
			if (m_wsParameter != nullptr)
				type = _type;
		}
		break;
	}
}

Wallpaper::~Wallpaper()
{
	type = CollectionType::none;
	if (m_wsParameter != nullptr)
	{
		delete[] m_wsParameter;
		m_wsParameter = nullptr;
	}
}

CollectionType Wallpaper::getType() const
{
	if (this == nullptr)
		return CollectionType::none;
	return type;
}

wchar_t* Wallpaper::getPathW() const
{
	if (this == nullptr)
		return nullptr;
	return m_wsParameter;
}