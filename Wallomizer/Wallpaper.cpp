#include "Wallpaper.h"

#include <windows.h>

Wallpaper::Wallpaper(CollectionType type)
{
	switch (type)
	{
		case CollectionType::local:
		{
			m_path = new wchar_t[MAX_PATH];
			if (m_path != nullptr)
				m_type = type;
		}
		break;
		case CollectionType::user:
		{
			m_path = new wchar_t[255];
			if (m_path != nullptr)
				m_type = type;
		}
		break;
		case CollectionType::search:
		{
			m_path = new wchar_t[1024];
			if (m_path != nullptr)
				m_type = type;
		}
		break;
	}
}

Wallpaper::~Wallpaper()
{
	m_type = CollectionType::none;
	if (m_path != nullptr)
	{
		delete[] m_path;
		m_path = nullptr;
	}
}

CollectionType Wallpaper::getType() const
{
	if (this == nullptr)
		return CollectionType::none;
	return m_type;
}

wchar_t* Wallpaper::getPathW() const
{
	if (this == nullptr)
		return nullptr;
	return m_path;
}