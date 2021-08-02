#include "Internet.h"
#include "UserCollection.h"
#include "SetUserCollectionWindow.h"
#include "Settings.h"

char UserCollection::buffer[bufferSize];
char *UserCollection::pBuffer = nullptr;

UserCollection::UserCollection()
{
	number = 0;
	settings = new UserCollectionSettings;
	settings->username[0] = '\0';
	settings->collectionID[0] = '\0';
	settings->apiKey[0] = '\0';
	settings->isApiKeyUsed = true;
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
	fputs(settings->isApiKeyUsed?"true":"false", pFile);
	fputs("\n", pFile);
	fputs(settings->apiKey, pFile);
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

	fgets(tmpBuffer, 9, pFile);
	settings->isApiKeyUsed = strcmp(tmpBuffer, "true\n") == 0 ? true : false;

	fgets(settings->apiKey, 64, pFile);
	settings->apiKey[strlen(settings->apiKey) - 1] = '\0';

	// Forming collection URL
	strcpy_s(collectionUrl, "https://wallhaven.cc/api/v1/collections/");
	strcat_s(collectionUrl, settings->username);
	strcat_s(collectionUrl, "/");
	strcat_s(collectionUrl, settings->collectionID);
	if (settings->isApiKeyUsed)
	{
		strcat_s(collectionUrl, "?apikey=");
		strcat_s(collectionUrl, settings->apiKey);
	}

	// Getting the META
	char collectionInfoURL[255];
	strcpy_s(collectionInfoURL, "https://wallhaven.cc/api/v1/collections/");
	strcat_s(collectionInfoURL, settings->username);
	if (settings->isApiKeyUsed)
	{
		strcat_s(collectionInfoURL, "?apikey=");
		strcat_s(collectionInfoURL, settings->apiKey);
	}

	if (!Internet::URLDownloadToBuffer(collectionInfoURL, buffer, bufferSize))
		return false;
	if ((pBuffer = Internet::parse(buffer, settings->collectionID, nullptr)) == nullptr)
		return false;
	if (Internet::parse(pBuffer, "\"count\":", &number) == nullptr)
		return false;

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

	if (!Internet::URLDownloadToBuffer(pageUrl, buffer, bufferSize))
		return false;

	pBuffer = buffer;
	for (unsigned int i = 1; i < index; i++)
		if ((pBuffer = Internet::parse(pBuffer, "\"path\":", nullptr)) == nullptr)
			return false;
	char imgUrl[255] = "";
	if (Internet::parse(pBuffer, "\"path\":", imgUrl) == nullptr)
		return false;

	char imgPath[255] = "Resources/Loaded wallpaper.dat";
	return Internet::URLDownloadToFile(imgUrl, imgPath);
}

LPCSTR UserCollection::collectionName() const
{
	return settings->collectionID;
}

void UserCollection::openCollectionSettingsWindow()
{
	SetUserCollectionWindow::windowThread(this);
}