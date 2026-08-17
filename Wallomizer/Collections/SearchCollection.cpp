#include "SearchCollection.h"

#include <Windows.h>
#include <Shellapi.h>

#include "WinUtils.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "Wallpaper.h"
#include "Internet.h"
#include "BinaryIO.h"

std::size_t SearchCollection::s_perPage = 24;

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
			&& file.read(settings.color)
			&& isValid();
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
		return isValid();
	}
	default:
		return false;
	}
}

void SearchCollection::update()
{
	m_wallpaperCount = 0;
	Internet internet;

	if (!m_isEnabled
		|| !internet.downloadToBuffer(getURL())
		|| !parseMeta(internet))
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

Wallpaper SearchCollection::getWallpaper(std::size_t index)
{
	const std::size_t page = index / s_perPage + 1;
	const std::size_t pageIndex = index % s_perPage;

	std::wstring url = getURL();
	url += L"&page=";
	url += std::to_wstring(page);

	Internet internet;
	if (!internet.downloadToBuffer(url))
		return Wallpaper::getEmptyWallpaper();

	for (std::size_t i = 0; i < pageIndex; i++)
		if (!internet.parse("path", true))
			return Wallpaper::getEmptyWallpaper();

	std::wstring path;
	if (!internet.parse("path", path, true))
		return Wallpaper::getEmptyWallpaper();

	parseMeta(internet);

	return Wallpaper(CollectionType::search, path);
}

bool SearchCollection::loadWallpaper(const std::wstring& url, const WinUtils& winUtils)
{
	Internet internet;
	return internet.downloadToFile(url, winUtils.getRoamingDir() / L"Loaded wallpaper.dat");
}

void SearchCollection::openWallpaperExternal(std::wstring_view path)
{
	const std::size_t dash = path.find_last_of(L"-");
	const std::size_t dot = path.find(L'.', dash);

	if (dash == std::wstring_view::npos
		|| dot == std::wstring_view::npos
		|| dash + 1 >= dot)
		return;

	const std::wstring_view id = path.substr(dash + 1, dot - dash - 1);

	std::wstring url = L"https://wallhaven.cc/w/";
	url += id;

	ShellExecuteW(nullptr, nullptr, url.c_str(), nullptr, nullptr, SW_SHOW);
}

bool SearchCollection::isValid() const
{
	return m_type == CollectionType::search;
}

bool SearchCollection::parseMeta(Internet& internet)
{
	if (!internet.parse("meta")
		|| !internet.parse("total", m_wallpaperCount, true))
		return false;

	// In case per_page came as a number.
	if (internet.parse("meta")
		&& internet.parse("per_page", s_perPage, true))
		return true;

	// In case per_page came as a string.
	std::wstring perPage;
	if (internet.parse("meta")
		&& internet.parse("per_page", perPage, true))
	{
		s_perPage = std::stoull(perPage);
		return true;
	}

	// Fallback.
	s_perPage = 24;
	return true;
}
