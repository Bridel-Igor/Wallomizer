#include "SearchCollection.h"
#include "Internet.h"
#include "SetSearchCollectionWindow.h"
#include "Settings.h"
#include "Filesystem.h"

bool SearchCollection::saveSettings(FILE* pFile) const
{
	if (pFile == NULL)
		return false;
	const CollectionType collType = getCollectionType();
	fwrite(&collType, sizeof(collType), 1, pFile);
	fwrite(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fwrite(&settings, sizeof(SearchCollection::SearchCollectionSettings), 1, pFile);
	return true;
}

bool SearchCollection::loadSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;

	fread(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fread(&settings, sizeof(SearchCollection::SearchCollectionSettings), 1, pFile);

	strcpy_s(m_sSearchUrl, "https://wallhaven.cc/api/v1/search?");

	if (strlen(settings.sTag))
	{
		strcat_s(m_sSearchUrl, "q=");
		strcat_s(m_sSearchUrl, settings.sTag);
		strcat_s(m_sSearchUrl, "&");
	}

	strcat_s(m_sSearchUrl, "categories=");
	strcat_s(m_sSearchUrl, settings.categoriesAndPurity & S_CATEGORY_GENERAL ? "1" : "0");
	strcat_s(m_sSearchUrl, settings.categoriesAndPurity & S_CATEGORY_ANIME ? "1" : "0");
	strcat_s(m_sSearchUrl, settings.categoriesAndPurity & S_CATEGORY_PEOPLE ? "1" : "0");

	strcat_s(m_sSearchUrl, "&purity=");
	strcat_s(m_sSearchUrl, settings.categoriesAndPurity & S_PURITY_SFW ? "1" : "0");
	strcat_s(m_sSearchUrl, settings.categoriesAndPurity & S_PURITY_SKETCHY ? "1" : "0");
	strcat_s(m_sSearchUrl, settings.categoriesAndPurity & S_PURITY_NSFW ? "1" : "0");

	strcat_s(m_sSearchUrl, settings.sResolution);
	strcat_s(m_sSearchUrl, settings.sRatio);
	strcat_s(m_sSearchUrl, settings.sColor);

	if (Settings::isApiKeyUsed())
	{
		strcat_s(m_sSearchUrl, "&apikey=");
		strcat_s(m_sSearchUrl, Settings::getApiKey());
	}

	if (!m_isEnabled)
		return true;

	char* pBuffer = Internet::buffer;
	Internet::bufferAccess.lock();
	if (!Internet::URLDownloadToBuffer(m_sSearchUrl))
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

void SearchCollection::getCollectionName(wchar_t* pwsName, size_t size) const
{
	wcscpy_s(pwsName, 255, L" Search: ");

	if (strlen(settings.sTag))
	{
		wchar_t wsTag[255]{ 0 };
		for (int i = 0; i < strlen(settings.sTag); i++)
			wsTag[i] = settings.sTag[i];
		wcscat_s(pwsName, 255, wsTag);
		wcscat_s(pwsName, 255, L" | ");
	}

	wcscat_s(pwsName, 255, settings.categoriesAndPurity & S_CATEGORY_GENERAL ? L"General " : L"");
	wcscat_s(pwsName, 255, settings.categoriesAndPurity & S_CATEGORY_ANIME ? L"Anime " : L"");
	wcscat_s(pwsName, 255, settings.categoriesAndPurity & S_CATEGORY_PEOPLE ? L"People " : L"");
}

CategoriesAndPurity SearchCollection::getCAP() const
{
	return settings.categoriesAndPurity;
}

Wallpaper* SearchCollection::getWallpaperInfo(unsigned int index) const
{
	Wallpaper* pWallpaper = nullptr;
	int page = int(index / s_nPerPage);
	index -= page * s_nPerPage;
	page++;
	char sPageUrl[1024];
	strcpy_s(sPageUrl, m_sSearchUrl);
	strcat_s(sPageUrl, "&page=");
	char sCurrentPage[15] = "";
	_itoa_s(page, sCurrentPage, 10);
	strcat_s(sPageUrl, sCurrentPage);

	char* pBuffer = Internet::buffer;
	Internet::bufferAccess.lock();

	if (!Internet::URLDownloadToBuffer(sPageUrl))
	{
		Internet::bufferAccess.unlock();
		return pWallpaper;
	}

	for (unsigned int i = 0; i < index; i++)
		if ((pBuffer = Internet::parse(pBuffer, "\"path\":", nullptr)) == nullptr)
		{
			Internet::bufferAccess.unlock();
			return pWallpaper;
		}
	pWallpaper = new Wallpaper(CollectionType::search);
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

void SearchCollection::openCollectionSettingsWindow()
{
	SetSearchCollectionWindow::windowThread(this, m_pCollectionManager);
}

bool SearchCollection::loadWallpaper(const Wallpaper* pWallpaper)
{
	wchar_t wsImgPath[MAX_PATH];
	Filesystem::getRoamingDir(wsImgPath);
	wcscat_s(wsImgPath, MAX_PATH, L"Loaded wallpaper.dat");
	return Internet::URLDownloadToFile(pWallpaper->getPathA(), wsImgPath);
}

void SearchCollection::openWallpaperExternal(const Wallpaper* pWallpaper)
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