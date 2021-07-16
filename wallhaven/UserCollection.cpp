#include "UserCollection.h"
#include "SetUserCollectionWindow.h"

#include "Urlmon.h"
#pragma comment(lib, "Urlmon.lib")

char UserCollection::queryPath[] = "Query.json";

UserCollection::UserCollection()
{
	number = 0;
	buffer = new char[32768];
	pBuffer = buffer;
	pFile = nullptr;
	settings = new UserCollectionSettings;
	settings->username[0] = '\0';
	settings->collectionID[0] = '\0';
	settings->apiKey[0] = '\0';
	settings->isApiKeyUsed = true;
}

UserCollection::~UserCollection()
{
	delete[] buffer;
	delete settings;
}

bool UserCollection::saveSettings(FILE* pFile)
{
	if (pFile != NULL)
	{
		fputs(collectionType(), pFile);
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
	return false;
}

bool UserCollection::loadSettings(FILE* pFile)
{
	if (pFile != NULL)
	{
		fgets(settings->username, 64, pFile);
		settings->username[strlen(settings->username) - 1] = '\0';
		fgets(settings->collectionID, 16, pFile);
		settings->collectionID[strlen(settings->collectionID) - 1] = '\0';
		char buffer[10];
		fgets(buffer, 10, pFile);
		buffer[strlen(buffer) - 1] = '\0';
		settings->isApiKeyUsed = strcmp(buffer, "true") == 0 ? true : false;
		fgets(settings->apiKey, 64, pFile);
		settings->apiKey[strlen(settings->apiKey) - 1] = '\0';
	}
	else
		return false;
	// Forming collection URL
	collectionUrl = "https://wallhaven.cc/api/v1/collections/";
	collectionUrl.append(settings->username);
	collectionUrl.append("/");
	collectionUrl.append(settings->collectionID);
	if (settings->isApiKeyUsed)
	{
		collectionUrl.append("?apikey=");
		collectionUrl.append(settings->apiKey);
	}

	// Getting the META
	URLDownloadToFileA(nullptr, collectionUrl.c_str(), queryPath, 0, nullptr);
	fopen_s(&pFile, queryPath, "r");
	if (pFile == nullptr)
		return false;
	pBuffer = buffer;
	fgets(pBuffer, 32768, pFile);
	if (fclose(pFile))
		return false;
	char search[16] = "";
	pBuffer = strstr(pBuffer, "per_page") + 10;
	int i = 0;
	while (pBuffer != nullptr && pBuffer[i] != ',')
	{
		search[i] = pBuffer[i];
		i++;
	}
	search[i] = '\0';
	per_page = atoi(search);
	if (pBuffer != nullptr)
		pBuffer = strstr(pBuffer, "total") + 7;
	i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '}')
	{
		search[i] = pBuffer[i];
		i++;
	}
	search[i] = '\0';
	number = atoi(search);
	return true;
}

bool UserCollection::setRandomWallpaper()
{
	int randomWallpaperNumber = rand() % number;
	int randomPageNum = int(randomWallpaperNumber / per_page);
	randomWallpaperNumber -= randomPageNum * per_page;
	randomPageNum++;
	std::string pageUrl = collectionUrl;
	pageUrl.append("&page=");
	char curPageNum[15] = "";
	_itoa_s(randomPageNum, curPageNum, 10);
	pageUrl.append(curPageNum);
	URLDownloadToFileA(nullptr, pageUrl.c_str(), queryPath, 0, nullptr);
	fopen_s(&pFile, queryPath, "r");
	if (pFile == nullptr)
		return false;
	pBuffer = buffer;
	fgets(pBuffer, 32768, pFile);
	if (fclose(pFile))
		return false;
	char imgUrl[255] = "";
	for (int i = 1; i <= randomWallpaperNumber; i++)
	{
		if (pBuffer == nullptr)
			return false;
		pBuffer = strstr(pBuffer, "path") + 7;
	}
	int  slide = 0, i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '\"')
	{
		if (pBuffer[i] == 92)
		{
			slide++;
			i++;
			continue;
		}
		imgUrl[i - slide] = pBuffer[i];
		i++;
	}
	imgUrl[i - slide] = '\0';
	char imgPath[255] = "Current wallpaper.jpg";
	URLDownloadToFileA(nullptr, imgUrl, imgPath, 0, nullptr);
	GetCurrentDirectoryA(255, imgPath);
	strcat_s(imgPath, "\\Current wallpaper.jpg");
	if (SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, imgPath, SPIF_UPDATEINIFILE))
		return true;
	else
		return false;
}

LPCSTR UserCollection::collectionName() const
{
	return settings->collectionID;
}

void UserCollection::openCollectionSettingsWindow()
{
	SetUserCollectionWindow::windowThread(this);
}