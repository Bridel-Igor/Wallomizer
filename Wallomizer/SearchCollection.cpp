#include "SearchCollection.h"
#include "Internet.h"
#include "SetSearchCollectionWindow.h"
#include "Settings.h"
#include "Filesystem.h"

SearchCollection::SearchCollection()
{
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

bool SearchCollection::loadWallpaper(unsigned int index)
{
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
		return false;
	}

	for (unsigned int i = 0; i < index; i++)
		if ((pBuffer = Internet::parse(pBuffer, "\"path\":", nullptr)) == nullptr)
		{
			Internet::bufferAccess.unlock();
			return false;
		}
	char imgUrl[255] = "";
	if (Internet::parse(pBuffer, "\"path\":", imgUrl) == nullptr)
	{
		Internet::bufferAccess.unlock();
		return false;
	}

	wchar_t imgPath[MAX_PATH];
	Filesystem::getRoamingDir(imgPath);
	wcscat_s(imgPath, MAX_PATH, L"Loaded wallpaper.dat");
	Internet::bufferAccess.unlock();
	return Internet::URLDownloadToFile(imgUrl, imgPath);
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
	
	strcat_s(name, 255, "| ");

	strcat_s(name, 255, settings->categoriesAndPurity & S_PURITY_SFW ? "SFW " : "");
	strcat_s(name, 255, settings->categoriesAndPurity & S_PURITY_SKETCHY ? "Sketchy " : "");
	strcat_s(name, 255, settings->categoriesAndPurity & S_PURITY_NSFW ? "NSFW " : "");

	return name;
}

void SearchCollection::openCollectionSettingsWindow()
{
	SetSearchCollectionWindow::windowThread(this);
}

void SearchCollection::openWallpaperExternal(unsigned int index)
{
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
		return;
	}

	for (unsigned int i = 0; i < index; i++)
		if ((pBuffer = Internet::parse(pBuffer, "\"url\":", nullptr)) == nullptr)
		{
			Internet::bufferAccess.unlock();
			return;
		}
	char imgUrl[255] = "";
	if (Internet::parse(pBuffer, "\"url\":", imgUrl) == nullptr)
	{
		Internet::bufferAccess.unlock();
		return;
	}

	Internet::bufferAccess.unlock();
	ShellExecute(0, 0, imgUrl, 0, 0, SW_SHOW);
}