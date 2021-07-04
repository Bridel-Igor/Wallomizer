#include "UserCollection.h"

#include "Urlmon.h"
#pragma comment(lib, "Urlmon.lib")

char UserCollection::queryPath[] = "Query.json";

UserCollection::UserCollection(Settings* _settings)
{
	settings = _settings;
	buffer = new char[32768];
	pBuffer = buffer;
	meta = new Meta;
	pFile = nullptr;
}

UserCollection::~UserCollection()
{
	delete meta;
	delete[] buffer;
}

bool UserCollection::setRandomWallpaper()
{
	// Forming of collection URL
	std::string collectionUrl = "https://wallhaven.cc/api/v1/collections/";
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
	meta->per_page = atoi(search);
	if (pBuffer != nullptr)
		pBuffer = strstr(pBuffer, "total") + 7;
	i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '}')
	{
		search[i] = pBuffer[i];
		i++;
	}
	search[i] = '\0';
	meta->total = atoi(search);

	// getting the URL of random wallpaper from collection
	int randomWallpaperNumber = rand() % meta->total;
	int randomPageNum = int(randomWallpaperNumber / meta->per_page);
	randomWallpaperNumber -= randomPageNum * meta->per_page;
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
	char wallpaperID[32] = "";
	for (int i = 0; i < randomWallpaperNumber; i++)
		pBuffer = strstr(pBuffer, "\"id\"") + 6;
	i = 0;
	while (pBuffer != nullptr && pBuffer[i] != '\"')
	{
		wallpaperID[i] = pBuffer[i];
		i++;
	}
	wallpaperID[i] = '\0';

	// downloading and setting wallpaper
	std::string wallpaperurl = "https://wallhaven.cc/api/v1/w/";
	wallpaperurl.append(wallpaperID);
	if (settings->isApiKeyUsed)
	{
		wallpaperurl.append("?apikey=");
		wallpaperurl.append(settings->apiKey);
	}
	URLDownloadToFileA(nullptr, wallpaperurl.c_str(), queryPath, 0, nullptr);
	fopen_s(&pFile, queryPath, "r");
	if (pFile == nullptr)
		return false;
	pBuffer = buffer;
	fgets(pBuffer, 32768, pFile);
	if (fclose(pFile))
		return false;
	char imgUrl[255] = "";
	pBuffer = strstr(pBuffer, "path") + 7;
	int  slide = 0; 
	i = 0;
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