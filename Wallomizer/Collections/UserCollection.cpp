#include "UserCollection.h"

#include <Shellapi.h>

#include "Internet.h"
#include "SetUserCollectionWindow.h"
#include "App.h"
#include "BinaryIO.h"

UserCollection::UserCollection(App& app) :
	m_app(app)
{
	m_type = Collection::Type::user;
}

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
			&& file.read(settings.collectionName);
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

		return true;
	}
	default:
		return false;
	}
}

void UserCollection::update()
{
	if (!m_isEnabled)
		return;

	// Getting the META
	std::wstring url = getURL();
	Internet internet;
	internet.DownloadToBuffer(url.c_str());
	if (!internet.parse("meta"))
		return;
	if (!internet.parse("total", m_number, true))
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

	if (m_app.getSettings().isApiKeyUsed())
	{
		url += L"&apikey=";
		url += m_app.getSettings().getData().apiKey;
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
	std::size_t page = index / s_perPage;
	index -= page * s_perPage;
	page++;

	std::wstring url = getURL();
	url += L"&page=";
	url += std::to_wstring(page);

	Internet internet;
	internet.DownloadToBuffer(url.c_str());
	for (unsigned int i = 0; i < index; i++)
		if (!internet.parse("path", true))
			return Wallpaper::getEmptyWallpaper();

	wchar_t wsPath[Collection::getMaxPathSize(Collection::Type::user) + 1] = {};
	if (!internet.parse("path", wsPath, true))
		return Wallpaper::getEmptyWallpaper();

	return Wallpaper(Collection::Type::user, wsPath);
}

void UserCollection::openCollectionSettingsWindow(HWND hCaller)
{
	SetUserCollectionWindow setUserCollectionWindow(hCaller, m_app, *this);
	setUserCollectionWindow.windowLoop();
}

bool UserCollection::loadWallpaper(std::wstring_view path, const WinUtils& winUtils)
{
	std::filesystem::path loadedPath = winUtils.getRoamingDir() / L"Loaded wallpaper.dat";
	Internet internet;
	return internet.DownloadToFile(path.data(), loadedPath.c_str());
}

void UserCollection::openWallpaperExternal(std::wstring_view path)
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

void UserCollection::loadCollectionList(App& app, std::list<UserCollectionInfo>& list, const wchar_t* wsUsername, const wchar_t* wsApiKey)
{
	wchar_t wsCollectionInfoURL[255];
	wcscpy_s(wsCollectionInfoURL, L"https://wallhaven.cc/api/v1/collections/");
	wcscat_s(wsCollectionInfoURL, wsUsername);
	if (wsApiKey[0])
	{
		wcscat_s(wsCollectionInfoURL, L"?apikey=");
		wcscat_s(wsCollectionInfoURL, app.getSettings().getData().apiKey);
	}

	Internet internet;
	internet.DownloadToBuffer(wsCollectionInfoURL);
	UserCollectionInfo uci;
	while (true)
	{
		if (!internet.parse("id", uci.id, true))
			break;
		if (!internet.parse("label", uci.wsLabel, true))
			break;
		list.push_back(uci);
	}
}
