#include "UserCollection.h"
#include "Internet.h"
#include "SetUserCollectionWindow.h"
#include "Settings.h"
#include "Filesystem.h"

bool UserCollection::saveSettings(FILE* pFile) const
{
	if (pFile == NULL)
		return false;
	const CollectionType collType = getCollectionType();
	fwrite(&collType, sizeof(collType), 1, pFile);
	fwrite(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fwrite(&settings, sizeof(UserCollection::UserCollectionSettings), 1, pFile);
	return true;
}

bool UserCollection::loadSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;

	fread(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fread(&settings, sizeof(UserCollection::UserCollectionSettings), 1, pFile);

	// Forming collection URL
	strcpy_s(m_sCollectionUrl, "https://wallhaven.cc/api/v1/collections/");
	strcat_s(m_sCollectionUrl, settings.sUsername);
	strcat_s(m_sCollectionUrl, "/");
	strcat_s(m_sCollectionUrl, settings.sCollectionID);

	strcat_s(m_sCollectionUrl, "?purity=");
	strcat_s(m_sCollectionUrl, settings.categoriesAndPurity & CAP::puritySFW ? "1" : "0");
	strcat_s(m_sCollectionUrl, settings.categoriesAndPurity & CAP::puritySketchy ? "1" : "0");
	strcat_s(m_sCollectionUrl, settings.categoriesAndPurity & CAP::purityNSFW ? "1" : "0");

	if (Settings::isApiKeyUsed())
	{
		strcat_s(m_sCollectionUrl, "&apikey=");
		strcat_s(m_sCollectionUrl, Settings::getApiKey());
	}

	if (!m_isEnabled)
		return true;

	// Getting the META
	char* pBuffer = Internet::buffer;
	Internet::bufferAccess.lock();
	if (!Internet::URLDownloadToBuffer(m_sCollectionUrl))
	{
		Internet::bufferAccess.unlock();
		return false;
	}
	if ((pBuffer = Internet::parse(pBuffer, "\"meta\"", nullptr)) == nullptr)
	{
		Internet::bufferAccess.unlock();
		return false;
	}
	if (Internet::parse(pBuffer, "\"total\":", &m_uiNumber) == nullptr)
	{
		Internet::bufferAccess.unlock();
		return false;
	}
	Internet::bufferAccess.unlock();
	return true;
}

void UserCollection::getCollectionName(wchar_t* pwsName, size_t size) const
{
	wchar_t wsUsername[64]{ 0 };
	mbstowcs_s(nullptr, wsUsername, settings.sUsername, 64);

	wchar_t wsCollectionName[64]{ 0 };
	mbstowcs_s(nullptr, wsCollectionName, settings.sCollectionName, 64);

	wcscpy_s(pwsName, size, L" ");
	wcscat_s(pwsName, size, wsUsername);
	wcscat_s(pwsName, size, L": ");
	wcscat_s(pwsName, size, wsCollectionName);
}

CategoriesAndPurity UserCollection::getCAP() const
{
	return settings.categoriesAndPurity;
}

Wallpaper* UserCollection::getWallpaperInfo(unsigned int index) const
{
	Wallpaper* pWallpaper = nullptr;
	int page = int(index / s_nPerPage);
	index -= page * s_nPerPage;
	page++;
	char sPageUrl[255];
	strcpy_s(sPageUrl, m_sCollectionUrl);
	strcat_s(sPageUrl, "&page=");
	char sCurrentPage[15] = "";
	_itoa_s(page, sCurrentPage, 10);
	strcat_s(sPageUrl, sCurrentPage);

	Internet::bufferAccess.lock();

	if (!Internet::URLDownloadToBuffer(sPageUrl))
	{
		Internet::bufferAccess.unlock();
		return pWallpaper;
	}

	char *pBuffer = Internet::buffer;
	for (unsigned int i = 1; i < index; i++)
		if ((pBuffer = Internet::parse(pBuffer, "\"path\":", nullptr)) == nullptr)
		{
			Internet::bufferAccess.unlock();
			return pWallpaper;
		}
	pWallpaper = new Wallpaper(CollectionType::user);
	if (Internet::parse(pBuffer, "\"path\":", pWallpaper->getPathA()) == nullptr)
	{
		Internet::bufferAccess.unlock();
		delete pWallpaper;
		pWallpaper = nullptr;
		return pWallpaper;
	}
	
	Internet::bufferAccess.unlock();
	return pWallpaper;
}

void UserCollection::openCollectionSettingsWindow(HWND hCaller)
{
	SetUserCollectionWindow setUserCollectionWindow(hCaller, m_pCollectionManager, this);
	setUserCollectionWindow.windowLoop();
}

bool UserCollection::loadWallpaper(const Wallpaper* pWallpaper)
{
	wchar_t wsImgPath[MAX_PATH];
	Filesystem::getRoamingDir(wsImgPath);
	wcscat_s(wsImgPath, MAX_PATH, L"Loaded wallpaper.dat");
	return Internet::URLDownloadToFile(pWallpaper->getPathA(), wsImgPath);
}

void UserCollection::openWallpaperExternal(const Wallpaper* pWallpaper)
{
	char sImgUrl[255] = "https://wallhaven.cc/w/";
	bool isDashFound = false;
	int j = (int)strlen(sImgUrl);
	for (int i = 0; i < strlen(pWallpaper->getPathA()); i++)
	{
		if (isDashFound)
		{
			if (pWallpaper->getPathA()[i] == '.')
				break;
			sImgUrl[j] = pWallpaper->getPathA()[i];
			j++;
		}
		if (pWallpaper->getPathA()[i] == '-')
			isDashFound = true;
	}
	sImgUrl[j] = '\0';

	ShellExecute(0, 0, sImgUrl, 0, 0, SW_SHOW);
}

std::vector<UserCollection::UserCollectionInfo> UserCollection::loadCollectionList(const char* sUsername, const char* sApiKey)
{
	char sCollectionInfoURL[255];
	strcpy_s(sCollectionInfoURL, "https://wallhaven.cc/api/v1/collections/");
	strcat_s(sCollectionInfoURL, sUsername);

	if (strlen(sApiKey))
	{
		strcat_s(sCollectionInfoURL, "?apikey=");
		strcat_s(sCollectionInfoURL, Settings::getApiKey());
	}

	Internet::bufferAccess.lock();
	std::vector<UserCollectionInfo> list;
	char* pBuffer = Internet::buffer;

	if (!Internet::URLDownloadToBuffer(sCollectionInfoURL))
	{
		Internet::bufferAccess.unlock();
		return list;
	}

	while (true)
	{
		UserCollectionInfo* uci = new UserCollectionInfo;
		if ((pBuffer = Internet::parse(pBuffer, "\"id\":", &uci->id)) == nullptr)
		{
			delete uci;
			break;
		}
		if ((pBuffer = Internet::parse(pBuffer, "\"label\":", uci->sLabel)) == nullptr)
		{
			delete uci;
			break;
		}
		list.push_back(*uci);
	}

	Internet::bufferAccess.unlock();
	return list;
}