#include "UserCollection.h"

#include "Internet.h"
#include "SetUserCollectionWindow.h"
#include "App.h"

UserCollection::UserCollection(App& app) :
	m_app(app)
{
}

bool UserCollection::saveSettings(FILE* pFile) const
{
	if (pFile == NULL)
		return false;
	const Collection::Type collType = getCollectionType();
	fwrite(&collType, sizeof(collType), 1, pFile);
	fwrite(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fwrite(&settings, sizeof(UserCollection::UserCollectionSettings), 1, pFile);
	return true;
}

bool UserCollection::loadSettings(FILE* pFile, unsigned short fileVersion)
{
	if (pFile == NULL)
		return false;

	fread(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);

	if (fileVersion >= 2U && fileVersion <= CollectionManager::COLLECTION_MANAGER_FILE_VERSION)
		fread(&settings, sizeof(UserCollection::UserCollectionSettings), 1, pFile);
	
	// Forming collection URL
	wcscpy_s(m_wsCollectionUrl, L"https://wallhaven.cc/api/v1/collections/");
	wcscat_s(m_wsCollectionUrl, settings.wsUsername);
	wcscat_s(m_wsCollectionUrl, L"/");
	wcscat_s(m_wsCollectionUrl, settings.wsCollectionID);

	wcscat_s(m_wsCollectionUrl, L"?purity=");
	wcscat_s(m_wsCollectionUrl, settings.categoriesAndPurity & CAP::puritySFW ? L"1" : L"0");
	wcscat_s(m_wsCollectionUrl, settings.categoriesAndPurity & CAP::puritySketchy ? L"1" : L"0");
	wcscat_s(m_wsCollectionUrl, settings.categoriesAndPurity & CAP::purityNSFW ? L"1" : L"0");

	if (m_app.getSettings().isApiKeyUsed())
	{
		wcscat_s(m_wsCollectionUrl, L"&apikey=");
		wcscat_s(m_wsCollectionUrl, m_app.getSettings().apiKey);
	}

	if (!m_isEnabled)
		return true;

	// Getting the META
	Internet internet;
	internet.DownloadToBuffer(m_wsCollectionUrl);
	if (!internet.parse("meta"))
		return false;
	if (!internet.parse("total", m_uiNumber, true))
		return false;
	return true;
}

void UserCollection::getCollectionName(wchar_t* wsName, size_t size) const
{
	wcscpy_s(wsName, size, L" ");
	wcscat_s(wsName, size, settings.wsUsername);
	wcscat_s(wsName, size, L": ");
	wcscat_s(wsName, size, settings.wsCollectionName);
}

CategoriesAndPurity UserCollection::getCAP() const
{
	return settings.categoriesAndPurity;
}

Wallpaper UserCollection::getWallpaper(unsigned int index) const
{
	int page = int(index / s_nPerPage);
	index -= page * s_nPerPage;
	page++;
	wchar_t wsPageUrl[255];
	wcscpy_s(wsPageUrl, m_wsCollectionUrl);
	wcscat_s(wsPageUrl, L"&page=");
	wchar_t wsCurrentPage[15] = L"";
	_itow_s(page, wsCurrentPage, 10);
	wcscat_s(wsPageUrl, wsCurrentPage);

	Internet internet;
	internet.DownloadToBuffer(wsPageUrl);
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
	SetUserCollectionWindow setUserCollectionWindow(hCaller, m_app, this);
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
		wcscat_s(wsCollectionInfoURL, app.getSettings().apiKey);
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
