#include "UserCollection.h"

#include <Shellapi.h>

#include "WinUtils.h"
#include "Settings.h"
#include "Wallpaper.h"
#include "Internet.h"
#include "BinaryIO.h"

bool UserCollection::saveSettings(BinaryWriter& file) const
{
	return file.write(m_type)
		&& file.write(m_isEnabled)
		&& file.write(settings.categoriesAndPurity)
		&& file.write(settings.username)
		&& file.write(settings.collectionID)
		&& file.write(settings.collectionName);
}

bool UserCollection::loadSettings(BinaryReader& file, std::uint16_t fileVersion)
{
	switch (fileVersion)
	{
	case 4U:
		return file.read(m_isEnabled)
			&& file.read(settings.categoriesAndPurity)
			&& file.read(settings.username)
			&& file.read(settings.collectionID)
			&& file.read(settings.collectionName)
			&& isValid();
	case 3U:
	case 2U:
	{
		struct LegacyData_V2_V3
		{
			wchar_t wsUsername[64] = L"";
			wchar_t wsCollectionID[16] = L"";
			wchar_t wsCollectionName[64] = L"";
			CategoriesAndPurity categoriesAndPurity = CAP::puritySFW;
		};
		LegacyData_V2_V3 oldData;
		if (!file.read(m_isEnabled)
			|| !file.read(oldData))
			return false;

		settings.categoriesAndPurity = oldData.categoriesAndPurity;
		settings.username = oldData.wsUsername;
		settings.collectionID = oldData.wsCollectionID;
		settings.collectionName = oldData.wsCollectionName;

		return isValid();
	}
	default:
		return false;
	}
}

void UserCollection::update()
{
	m_wallpaperCount = 0;
	Internet internet;

	if (!m_isEnabled
		|| !internet.downloadToBuffer(getURL())
		|| !internet.parse("meta")
		|| !internet.parse("total", m_wallpaperCount, true))
		return;
}

std::wstring UserCollection::getURL() const
{
	std::wstring url = L"https://wallhaven.cc/api/v1/collections/";
	url.reserve(512);

	url += settings.username;
	url += L"/";
	url += settings.collectionID;

	url += L"?purity=";
	url += (settings.categoriesAndPurity & CAP::puritySFW) ? L"1" : L"0";
	url += (settings.categoriesAndPurity & CAP::puritySketchy) ? L"1" : L"0";
	url += (settings.categoriesAndPurity & CAP::purityNSFW) ? L"1" : L"0";

	if (m_settings.isApiKeyUsed())
	{
		url += L"&apikey=";
		url += m_settings.getData().apiKey;
	}

	return url;
}

std::wstring UserCollection::getCollectionName() const
{
	std::wstring name = L" ";
	name += settings.username;
	name += L": ";
	name += settings.collectionName;
	return name;
}

CategoriesAndPurity UserCollection::getCAP() const
{
	return settings.categoriesAndPurity;
}

Wallpaper UserCollection::getWallpaper(std::size_t index) const
{
	const std::size_t page = index / s_perPage + 1;
	index %= s_perPage;

	std::wstring url = getURL();
	url += L"&page=";
	url += std::to_wstring(page);

	Internet internet;
	if (!internet.downloadToBuffer(url))
		return Wallpaper::getEmptyWallpaper();

	for (unsigned int i = 0; i < index; i++)
		if (!internet.parse("path", true))
			return Wallpaper::getEmptyWallpaper();

	std::wstring path;
	if (!internet.parse("path", path, true))
		return Wallpaper::getEmptyWallpaper();

	return Wallpaper(Collection::Type::user, path);
}

bool UserCollection::loadWallpaper(const std::wstring& url, const WinUtils& winUtils)
{
	Internet internet;
	return internet.downloadToFile(url, winUtils.getRoamingDir() / L"Loaded wallpaper.dat");
}

void UserCollection::openWallpaperExternal(std::wstring_view path)
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

void UserCollection::loadCollectionList(std::list<UserCollectionInfo>& list, const std::wstring& username, const std::wstring& apiKey)
{
	std::wstring url =  L"https://wallhaven.cc/api/v1/collections/";
	url += username;
	if (!apiKey.empty())
	{
		url += L"?apikey=";
		url += apiKey;
	}

	Internet internet;
	if (!internet.downloadToBuffer(url))
		return;

	UserCollectionInfo uci;
	while (true)
	{
		if (!internet.parse("id", uci.id, true)
			|| !internet.parse("label", uci.label, true))
			break;
		list.push_back(uci);
	}
}

bool UserCollection::isValid() const
{
	return m_type == Collection::Type::user
		&& !settings.username.empty()
		&& !settings.collectionID.empty()
		&& !settings.collectionName.empty();
}
