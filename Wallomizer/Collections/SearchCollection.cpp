#include "SearchCollection.h"

#include <Shellapi.h>

#include "WinUtils.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "Wallpaper.h"
#include "SetSearchCollectionWindow.h"
#include "Internet.h"
#include "BinaryIO.h"

uint32_t SearchCollection::s_perPage = 64;

SearchCollection::SearchCollection(Settings& settings, CollectionManager& collectionManager) :
	m_settings(settings),
	m_collectionManager(collectionManager)
{
	m_type = Collection::Type::search;
}

bool SearchCollection::saveSettings(BinaryWriter& file) const
{
	return file.write(m_type)
		&& file.write(m_isEnabled)
		&& file.write(settings.categoriesAndPurity)
		&& file.write(settings.tag)
		&& file.write(settings.resolution)
		&& file.write(settings.ratio)
		&& file.write(settings.color);
}

bool SearchCollection::loadSettings(BinaryReader& file, std::uint16_t fileVersion)
{
	switch (fileVersion)
	{
	case 4U:
		return file.read(m_isEnabled)
			&& file.read(settings.categoriesAndPurity)
			&& file.read(settings.tag)
			&& file.read(settings.resolution)
			&& file.read(settings.ratio)
			&& file.read(settings.color);
	case 3U:
	case 2U:
	{
		struct LegacyData_V3_V2
		{
			CategoriesAndPurity categoriesAndPurity = CAP::categoryGeneral | CAP::categoryAnime | CAP::categoryPeople | CAP::puritySFW;
			wchar_t wsTag[255] = L"";
			wchar_t wsResolution[255] = L"";
			wchar_t wsRatio[128] = L"";
			wchar_t wsColor[16] = L"";
		};
		LegacyData_V3_V2 oldData;
		if (!file.read(m_isEnabled)
			|| !file.read(oldData))
			return false;
		settings.categoriesAndPurity = oldData.categoriesAndPurity;
		settings.tag = oldData.wsTag;
		settings.resolution = oldData.wsResolution;
		settings.ratio = oldData.wsRatio;
		settings.color = oldData.wsColor;
		return true;
	}
	default:
		return false;
	}
}

void SearchCollection::update()
{
	if (!m_isEnabled)
		return;

	std::wstring url = getURL();

	Internet internet;
	internet.DownloadToBuffer(url.c_str(), s_perPage * 750);

	if (!internet.parse("meta"))
		return;

	wchar_t wsPerPage[4]{};
	if (!internet.parse("per_page", wsPerPage, true))
		return;
	s_perPage = wcstoul(wsPerPage, nullptr, 10);

	if (!internet.parse("total", m_number, true))
		return;
}

std::wstring SearchCollection::getURL() const
{
	std::wstring url = L"https://wallhaven.cc/api/v1/search?";
	url.reserve(1024);

	if (!settings.tag.empty())
	{
		url += L"q=";
		url += settings.tag;
		url += L"&";
	}

	url += L"categories=";
	url += (settings.categoriesAndPurity & CAP::categoryGeneral) ? L"1" : L"0";
	url += (settings.categoriesAndPurity & CAP::categoryAnime) ? L"1" : L"0";
	url += (settings.categoriesAndPurity & CAP::categoryPeople) ? L"1" : L"0";

	url += L"&purity=";
	url += (settings.categoriesAndPurity & CAP::puritySFW) ? L"1" : L"0";
	url += (settings.categoriesAndPurity & CAP::puritySketchy) ? L"1" : L"0";
	url += (settings.categoriesAndPurity & CAP::purityNSFW) ? L"1" : L"0";

	url += settings.resolution;
	url += settings.ratio;
	url += settings.color;

	if (m_settings.isApiKeyUsed())
	{
		url += L"&apikey=";
		url += m_settings.getData().apiKey;
	}

	return url;
}

std::wstring SearchCollection::getCollectionName() const
{
	std::wstring name = L" Search: ";

	if (!settings.tag.empty())
	{
		name += settings.tag;
		name += L" | ";
	}

	name += (settings.categoriesAndPurity & CAP::categoryGeneral) ? L"General " : L"";
	name += (settings.categoriesAndPurity & CAP::categoryAnime) ? L"Anime " : L"";
	name += (settings.categoriesAndPurity & CAP::categoryPeople) ? L"People " : L"";

	return name;
}

CategoriesAndPurity SearchCollection::getCAP() const
{
	return settings.categoriesAndPurity;
}

Wallpaper SearchCollection::getWallpaper(std::size_t index) const
{
	std::size_t page = index / s_perPage;
	index -= page * s_perPage;
	page++;

	std::wstring url = getURL();
	url += L"&page=";
	url += std::to_wstring(page);

	Internet internet;
	internet.DownloadToBuffer(url.c_str(), s_perPage * 750);
	for (unsigned int i = 0; i < index; i++)
		if (!internet.parse("path", true))
			return Wallpaper::getEmptyWallpaper();

	wchar_t wsPath[Collection::getMaxPathSize(Collection::Type::search) + 1] = {};
	if (!internet.parse("path", wsPath, true))
		return Wallpaper::getEmptyWallpaper();

	internet.parse("meta", true);
	wchar_t wsPerPage[4]{};
	if (internet.parse("per_page", wsPerPage, true))
		s_perPage = wcstoul(wsPerPage, nullptr, 10);

	return Wallpaper(Collection::Type::search, wsPath);
}

void SearchCollection::openCollectionSettingsWindow(HWND hCaller)
{
	SetSearchCollectionWindow setSearchCollectionWindow(hCaller, *this);
	setSearchCollectionWindow.windowLoop();
}

bool SearchCollection::loadWallpaper(std::wstring_view path, const WinUtils& winUtils)
{
	std::filesystem::path loadedPath = winUtils.getRoamingDir() / L"Loaded wallpaper.dat";
	Internet internet;
	return internet.DownloadToFile(path.data(), loadedPath.c_str());
}

void SearchCollection::openWallpaperExternal(std::wstring_view path)
{
	wchar_t wsImgUrl[255] = L"https://wallhaven.cc/w/";
	bool isDashFound = false;
	int j = (int)wcslen(wsImgUrl);
	for (int i = 0; path.data()[i]; i++)
	{
		if (isDashFound)
		{
			if (path.data()[i] == '.')
				break;
			wsImgUrl[j] = path.data()[i];
			j++;
		}
		if (path.data()[i] == '-')
			isDashFound = true;
	}
	wsImgUrl[j] = '\0';

	ShellExecuteW(0, 0, wsImgUrl, 0, 0, SW_SHOW);
}
