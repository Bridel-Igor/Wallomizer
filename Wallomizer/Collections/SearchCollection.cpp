#include "SearchCollection.h"

#include "Internet.h"
#include "SetSearchCollectionWindow.h"
#include "App.h"

SearchCollection::SearchCollection(App& app) :
	m_app(app)
{
}

bool SearchCollection::saveSettings(FILE* pFile) const
{
	if (pFile == NULL)
		return false;
	const Collection::Type collType = getCollectionType();
	fwrite(&collType, sizeof(collType), 1, pFile);
	fwrite(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);
	fwrite(&settings, sizeof(SearchCollection::SearchCollectionSettings), 1, pFile);
	return true;
}

bool SearchCollection::loadSettings(FILE* pFile, unsigned short fileVersion)
{
	if (pFile == NULL)
		return false;

	fread(&m_isEnabled, sizeof(m_isEnabled), 1, pFile);

	if (fileVersion == 2U)
	{
		struct SearchCollectionSettings_2U
		{
			CategoriesAndPurity categoriesAndPurity = CAP::categoryGeneral | CAP::categoryAnime | CAP::categoryPeople | CAP::puritySFW;
			wchar_t wsTag[255] = L"";
			wchar_t wsResolution[255] = L"";
			wchar_t wsRatio[128] = L"";
			wchar_t wsColor[16] = L"";
		}settings_2U;
		fread(&settings_2U, sizeof(SearchCollectionSettings_2U), 1, pFile);
		settings.categoriesAndPurity = settings_2U.categoriesAndPurity;
		wcscpy_s(settings.wsColor, settings_2U.wsColor);
		wcscpy_s(settings.wsRatio, settings_2U.wsRatio);
		wcscpy_s(settings.wsResolution, settings_2U.wsResolution);
		wcscpy_s(settings.wsTag, settings_2U.wsTag);
	}
	if (fileVersion >= 3U && fileVersion <= CollectionManager::COLLECTION_MANAGER_FILE_VERSION)
		fread(&settings, sizeof(SearchCollection::SearchCollectionSettings), 1, pFile);

	wcscpy_s(m_wsSearchUrl, L"https://wallhaven.cc/api/v1/search?");

	if (settings.wsTag[0])
	{
		wcscat_s(m_wsSearchUrl, L"q=");
		wcscat_s(m_wsSearchUrl, settings.wsTag);
		wcscat_s(m_wsSearchUrl, L"&");
	}

	wcscat_s(m_wsSearchUrl, L"categories=");
	wcscat_s(m_wsSearchUrl, settings.categoriesAndPurity & CAP::categoryGeneral ? L"1" : L"0");
	wcscat_s(m_wsSearchUrl, settings.categoriesAndPurity & CAP::categoryAnime ? L"1" : L"0");
	wcscat_s(m_wsSearchUrl, settings.categoriesAndPurity & CAP::categoryPeople ? L"1" : L"0");

	wcscat_s(m_wsSearchUrl, L"&purity=");
	wcscat_s(m_wsSearchUrl, settings.categoriesAndPurity & CAP::puritySFW ? L"1" : L"0");
	wcscat_s(m_wsSearchUrl, settings.categoriesAndPurity & CAP::puritySketchy ? L"1" : L"0");
	wcscat_s(m_wsSearchUrl, settings.categoriesAndPurity & CAP::purityNSFW ? L"1" : L"0");

	wcscat_s(m_wsSearchUrl, settings.wsResolution);
	wcscat_s(m_wsSearchUrl, settings.wsRatio);
	wcscat_s(m_wsSearchUrl, settings.wsColor);

	if (m_app.getSettings().isApiKeyUsed())
	{
		wcscat_s(m_wsSearchUrl, L"&apikey=");
		wcscat_s(m_wsSearchUrl, m_app.getSettings().apiKey);
	}

	if (!m_isEnabled)
		return true;

	Internet internet;
	internet.DownloadToBuffer(m_wsSearchUrl, m_app.getSettings().uPerPage * 750);
	if (!internet.parse("meta"))
		return false;
	if (!internet.parse("total", m_uiNumber, true))
		return false;
	return true;
}

void SearchCollection::getCollectionName(wchar_t* wsName, size_t size) const
{
	wcscpy_s(wsName, 255, L" Search: ");

	if (settings.wsTag[0])
	{
		wchar_t wsTag[255]{ 0 };
		for (int i = 0; settings.wsTag[i]; i++)
			wsTag[i] = settings.wsTag[i];
		wcscat_s(wsName, 255, wsTag);
		wcscat_s(wsName, 255, L" | ");
	}

	wcscat_s(wsName, size, settings.categoriesAndPurity & CAP::categoryGeneral ? L"General " : L"");
	wcscat_s(wsName, size, settings.categoriesAndPurity & CAP::categoryAnime ? L"Anime " : L"");
	wcscat_s(wsName, size, settings.categoriesAndPurity & CAP::categoryPeople ? L"People " : L"");
}

CategoriesAndPurity SearchCollection::getCAP() const
{
	return settings.categoriesAndPurity;
}

Wallpaper SearchCollection::getWallpaper(unsigned int index) const
{
	int page = int(index / m_app.getSettings().uPerPage);
	index -= page * m_app.getSettings().uPerPage;
	page++;
	wchar_t wsPageUrl[1024];
	wcscpy_s(wsPageUrl, m_wsSearchUrl);
	wcscat_s(wsPageUrl, L"&page=");
	wchar_t wsCurrentPage[15] = L"";
	_itow_s(page, wsCurrentPage, 10);
	wcscat_s(wsPageUrl, wsCurrentPage);

	Internet internet;
	internet.DownloadToBuffer(wsPageUrl, m_app.getSettings().uPerPage * 750);
	for (unsigned int i = 0; i < index; i++)
		if (!internet.parse("path", true))
			return Wallpaper::getEmptyWallpaper();

	wchar_t wsPath[Collection::getMaxPathSize(Collection::Type::search) + 1] = {};
	if (!internet.parse("path", wsPath, true))
		return Wallpaper::getEmptyWallpaper();

	return Wallpaper(Collection::Type::search, wsPath);
}

void SearchCollection::openCollectionSettingsWindow(HWND hCaller)
{
	SetSearchCollectionWindow setSearchCollectionWindow(hCaller, m_app.getCollectionManager(), this);
	setSearchCollectionWindow.windowLoop();
}

bool SearchCollection::loadWallpaper(std::wstring_view path, const WinUtils& winUtils)
{
	wchar_t wsImgPath[MAX_PATH];
	wcscpy_s(wsImgPath, MAX_PATH, winUtils.getRoamingDir());
	wcscat_s(wsImgPath, MAX_PATH, L"Loaded wallpaper.dat");
	Internet internet;
	return internet.DownloadToFile(path.data(), wsImgPath);
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
