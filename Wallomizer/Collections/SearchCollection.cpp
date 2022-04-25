#include "SearchCollection.h"
#include "Internet.h"
#include "SetSearchCollectionWindow.h"
#include "Settings.h"
#include "Filesystem.h"

SearchCollection::SearchCollection(CollectionManager* _collectionManager)
{
	m_pCollectionManager = _collectionManager;
	isEnabled = true;
	number = 0;
	settings = new SearchCollectionSettings;
	settings->categoriesAndPurity = S_CATEGORY_GENERAL | S_CATEGORY_ANIME | S_CATEGORY_PEOPLE | S_PURITY_SFW;
	settings->tag[0] = '\0';
	settings->resolution[0] = '\0';
	settings->ratio[0] = '\0';
	settings->color[0] = '\0';
}

SearchCollection::~SearchCollection()
{
	delete settings;
}

bool SearchCollection::saveSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	fputs(collectionType(), pFile);
	fputs("\n", pFile);
	fputs(isEnabled ? "true" : "false", pFile);
	fputs("\n", pFile);
	fputs(settings->tag, pFile);
	fputs("\n", pFile);
	fputs(settings->resolution, pFile);
	fputs("\n", pFile);
	fputs(settings->ratio, pFile);
	fputs("\n", pFile);
	fputs(settings->color, pFile);
	fputs("\n", pFile);
	fputc(settings->categoriesAndPurity, pFile);
	fputs("\n", pFile);
	return true;
}

bool SearchCollection::loadSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	char tmpBuffer[10] = { 0 };

	fgets(tmpBuffer, 9, pFile);
	isEnabled = strcmp(tmpBuffer, "true\n") == 0 ? true : false;
	fgets(settings->tag, 255, pFile);
	settings->tag[strlen(settings->tag) - 1] = '\0';
	fgets(settings->resolution, 255, pFile);
	settings->resolution[strlen(settings->resolution) - 1] = '\0';
	fgets(settings->ratio, 128, pFile);
	settings->ratio[strlen(settings->ratio) - 1] = '\0';
	fgets(settings->color, 16, pFile);
	settings->color[strlen(settings->color) - 1] = '\0';
	fgets(tmpBuffer, 9, pFile);
	settings->categoriesAndPurity = tmpBuffer[0];

	strcpy_s(searchUrl, "https://wallhaven.cc/api/v1/search?");

	if (strlen(settings->tag))
	{
		strcat_s(searchUrl, "q=");
		strcat_s(searchUrl, settings->tag);
		strcat_s(searchUrl, "&");
	}

	strcat_s(searchUrl, "categories=");
	strcat_s(searchUrl, settings->categoriesAndPurity & S_CATEGORY_GENERAL ? "1" : "0");
	strcat_s(searchUrl, settings->categoriesAndPurity & S_CATEGORY_ANIME ? "1" : "0");
	strcat_s(searchUrl, settings->categoriesAndPurity & S_CATEGORY_PEOPLE ? "1" : "0");

	strcat_s(searchUrl, "&purity=");
	strcat_s(searchUrl, settings->categoriesAndPurity & S_PURITY_SFW ? "1" : "0");
	strcat_s(searchUrl, settings->categoriesAndPurity & S_PURITY_SKETCHY ? "1" : "0");
	strcat_s(searchUrl, settings->categoriesAndPurity & S_PURITY_NSFW ? "1" : "0");

	strcat_s(searchUrl, settings->resolution);
	strcat_s(searchUrl, settings->ratio);
	strcat_s(searchUrl, settings->color);

	if (Settings::isApiKeyUsed())
	{
		strcat_s(searchUrl, "&apikey=");
		strcat_s(searchUrl, Settings::getApiKey());
	}

	if (!isEnabled)
		return true;

	char* pBuffer = Internet::buffer;
	Internet::bufferAccess.lock();
	if (!Internet::URLDownloadToBuffer(searchUrl))
	{
		Internet::bufferAccess.unlock();
		return false;
	}
	if ((pBuffer = Internet::parse(pBuffer, "\"meta\"", nullptr)) == nullptr)
	{
		Internet::bufferAccess.unlock();
		return false;
	}
	if (Internet::parse(pBuffer, "\"total\":", &number) == nullptr)
	{
		Internet::bufferAccess.unlock();
		return false;
	}
	Internet::bufferAccess.unlock();
	return true;
}


Wallpaper* SearchCollection::getWallpaperInfo(unsigned int index)
{
	Wallpaper* wallpaper = nullptr;
	int PageNum = int(index / per_page);
	index -= PageNum * per_page;
	PageNum++;
	char pageUrl[1024];
	strcpy_s(pageUrl, searchUrl);
	strcat_s(pageUrl, "&page=");
	char curPageNum[15] = "";
	_itoa_s(PageNum, curPageNum, 10);
	strcat_s(pageUrl, curPageNum);

	char* pBuffer = Internet::buffer;
	Internet::bufferAccess.lock();

	if (!Internet::URLDownloadToBuffer(pageUrl))
	{
		Internet::bufferAccess.unlock();
		return wallpaper;
	}

	for (unsigned int i = 0; i < index; i++)
		if ((pBuffer = Internet::parse(pBuffer, "\"path\":", nullptr)) == nullptr)
		{
			Internet::bufferAccess.unlock();
			return wallpaper;
		}
	wallpaper = new Wallpaper(CollectionType::search);
	if (Internet::parse(pBuffer, "\"path\":", wallpaper->getPathA()) == nullptr)
	{
		Internet::bufferAccess.unlock();
		delete wallpaper;
		return wallpaper;
	}
	
	Internet::bufferAccess.unlock();
	return wallpaper;
}

bool SearchCollection::loadWallpaper(Wallpaper* wallpaper)
{
	wchar_t imgPath[MAX_PATH];
	Filesystem::getRoamingDir(imgPath);
	wcscat_s(imgPath, MAX_PATH, L"Loaded wallpaper.dat");
	return Internet::URLDownloadToFile(wallpaper->getPathA(), imgPath);
}

LPCSTR SearchCollection::collectionName() const
{
	char* name;
	name = new char[255]{ 0 };

	strcat_s(name, 255, " Search: ");

	if (strlen(settings->tag))
	{
		strcat_s(name, 255, settings->tag);
		strcat_s(name, 255, " | ");
	}

	strcat_s(name, 255, settings->categoriesAndPurity & S_CATEGORY_GENERAL ? "General " : "");
	strcat_s(name, 255, settings->categoriesAndPurity & S_CATEGORY_ANIME ? "Anime " : "");
	strcat_s(name, 255, settings->categoriesAndPurity & S_CATEGORY_PEOPLE ? "People " : "");

	return name;
}

CategoriesAndPurity SearchCollection::getCAP()
{
	if (settings)
		return settings->categoriesAndPurity;
	return 0;
}

void SearchCollection::openCollectionSettingsWindow()
{
	SetSearchCollectionWindow::windowThread(this, m_pCollectionManager);
}

void SearchCollection::openWallpaperExternal(Wallpaper* wallpaper)
{
	char imgUrl[255] = "https://wallhaven.cc/w/";
	bool dashFound = false;
	int j = (int)strlen(imgUrl);
	for (int i = 0; i < strlen(wallpaper->getPathA()); i++)
	{
		if (dashFound)
		{
			if (wallpaper->getPathA()[i] == '.')
				break;
			imgUrl[j] = wallpaper->getPathA()[i];
			j++;
		}
		if (wallpaper->getPathA()[i] == '-')
			dashFound = true;
	}
	imgUrl[j] = '\0';

	ShellExecute(0, 0, imgUrl, 0, 0, SW_SHOW);
}