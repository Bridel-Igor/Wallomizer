#include "UserCollection.h"
#include "Internet.h"
#include "SetUserCollectionWindow.h"
#include "Settings.h"

UserCollection::UserCollection()
{
	number = 0;
	settings = new UserCollectionSettings;
	settings->username[0] = '\0';
	settings->collectionID[0] = '\0';
	settings->collectionName[0] = '\0';
	settings->categoriesAndPurity = S_PURITY_SFW;
}

UserCollection::~UserCollection()
{
	delete settings;
}

bool UserCollection::saveSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	fputs(collectionType(), pFile);
	fputs("\n", pFile);
	fputs(isEnabled ? "true" : "false", pFile);
	fputs("\n", pFile);
	fputs(settings->username, pFile);
	fputs("\n", pFile);
	fputs(settings->collectionID, pFile);
	fputs("\n", pFile);
	fputs(settings->collectionName, pFile);
	fputs("\n", pFile);
	fputc(settings->categoriesAndPurity, pFile);
	fputs("\n", pFile);
	return true;
}

bool UserCollection::loadSettings(FILE* pFile)
{
	if (pFile == NULL)
		return false;
	char tmpBuffer[10]={0};

	fgets(tmpBuffer, 9, pFile);
	isEnabled = strcmp(tmpBuffer, "true\n") == 0 ? true : false;

	fgets(settings->username, 64, pFile);
	settings->username[strlen(settings->username) - 1] = '\0';

	fgets(settings->collectionID, 16, pFile);
	settings->collectionID[strlen(settings->collectionID) - 1] = '\0';

	fgets(settings->collectionName, 64, pFile);
	settings->collectionName[strlen(settings->collectionName) - 1] = '\0';

	fgets(tmpBuffer, 9, pFile);
	settings->categoriesAndPurity = tmpBuffer[0];

	// Forming collection URL
	strcpy_s(collectionUrl, "https://wallhaven.cc/api/v1/collections/");
	strcat_s(collectionUrl, settings->username);
	strcat_s(collectionUrl, "/");
	strcat_s(collectionUrl, settings->collectionID);

	strcat_s(collectionUrl, "?purity=");
	strcat_s(collectionUrl, settings->categoriesAndPurity & S_PURITY_SFW ? "1" : "0");
	strcat_s(collectionUrl, settings->categoriesAndPurity & S_PURITY_SKETCHY ? "1" : "0");
	strcat_s(collectionUrl, settings->categoriesAndPurity & S_PURITY_NSFW ? "1" : "0");

	if (Settings::isApiKeyUsed())
	{
		strcat_s(collectionUrl, "&apikey=");
		strcat_s(collectionUrl, Settings::getApiKey());
	}

	// Getting the META
	char* pBuffer = Internet::buffer;
	Internet::bufferAccess.lock();
	if (!Internet::URLDownloadToBuffer(collectionUrl))
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

bool UserCollection::loadWallpaper(unsigned int index)
{
	int PageNum = int(index / per_page);
	index -= PageNum * per_page;
	PageNum++;
	char pageUrl[255];
	strcpy_s(pageUrl, collectionUrl);
	strcat_s(pageUrl, "&page=");
	char curPageNum[15] = "";
	_itoa_s(PageNum, curPageNum, 10);
	strcat_s(pageUrl, curPageNum);

	Internet::bufferAccess.lock();

	if (!Internet::URLDownloadToBuffer(pageUrl))
	{
		Internet::bufferAccess.unlock();
		return false;
	}

	char *pBuffer = Internet::buffer;
	for (unsigned int i = 1; i < index; i++)
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

	char imgPath[255] = "Resources/Loaded wallpaper.dat";
	Internet::bufferAccess.unlock();
	return Internet::URLDownloadToFile(imgUrl, imgPath);
}

std::vector<UserCollection::UserCollectionInfo> UserCollection::loadCollectionList(char* username, char* apiKey)
{
	char collectionInfoURL[255];
	strcpy_s(collectionInfoURL, "https://wallhaven.cc/api/v1/collections/");
	strcat_s(collectionInfoURL, username);

	if (strlen(apiKey))
	{
		strcat_s(collectionInfoURL, "?apikey=");
		strcat_s(collectionInfoURL, Settings::getApiKey());
	}

	Internet::bufferAccess.lock();
	std::vector<UserCollectionInfo> list;
	char* pBuffer = Internet::buffer;

	if (!Internet::URLDownloadToBuffer(collectionInfoURL))
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
		if ((pBuffer = Internet::parse(pBuffer, "\"label\":", uci->label)) == nullptr)
		{
			delete uci;
			break;
		}
		list.push_back(*uci);
	}

	Internet::bufferAccess.unlock();
	return list;
}

LPCSTR UserCollection::collectionName() const
{
	char name[255] = { 0 };

	strcat_s(name, " ");
	strcat_s(name, settings->username);
	strcat_s(name, ": ");
	strcat_s(name, settings->collectionName);
	strcat_s(name, " | ");
	strcat_s(name, settings->categoriesAndPurity & S_PURITY_SFW ? "SFW " : "");
	strcat_s(name, settings->categoriesAndPurity & S_PURITY_SKETCHY ? "Sketchy " : "");
	strcat_s(name, settings->categoriesAndPurity & S_PURITY_NSFW ? "NSFW " : "");

	return name;
}

void UserCollection::openCollectionSettingsWindow()
{
	SetUserCollectionWindow::windowThread(this);
}