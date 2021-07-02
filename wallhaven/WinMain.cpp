#include <string>
#include <windows.h>
#include "time.h"
#include "Urlmon.h"
#pragma comment(lib, "Urlmon.lib")

bool setWallpaper(char* buffer, std::string wallpaperid, bool apiKeyUsed, std::string apiKey)
{
	std::string wallpaperurl = "https://wallhaven.cc/api/v1/w/";
	wallpaperurl.append(wallpaperid);
	if (apiKeyUsed)
	{
		wallpaperurl.append("?apikey=");
		wallpaperurl.append(apiKey);
	}

	char path[] = "D://Query.txt";
	URLDownloadToFileA(nullptr, wallpaperurl.c_str(), path, 0, nullptr);

	FILE* pFile;
	fopen_s(&pFile, path, "r");
	if (pFile != NULL)
	{
		fgets(buffer, 32768, pFile);
		fclose(pFile);
	}

	char imgurl[255] = "";
	buffer = strstr(buffer, "path") + 7;

	int i = 0, slide = 0;
	while (buffer[i] != '\"')
	{
		if (buffer[i] == 92)
		{
			slide++;
			i++;
			continue;
		}
		imgurl[i - slide] = buffer[i];
		i++;
	}
	imgurl[i - slide] = '\0';

	char pathimg[] = "D://Current wallpaper.jpg";
	URLDownloadToFileA(nullptr, imgurl, pathimg, 0, nullptr);

	if (SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, pathimg, SPIF_UPDATEINIFILE))
		return true;
	else
		return false;
}

struct Meta
{
	int per_page, total;
};

void getMeta(char* buffer, std::string url, Meta* meta)
{
	char path[] = "D://query.txt";
	URLDownloadToFileA(nullptr, url.c_str(), path, 0, nullptr);

	FILE* pFile;
	fopen_s(&pFile, path, "r");
	if (pFile != NULL)
	{
		fgets(buffer, 32768, pFile);
		fclose(pFile);
	}
	char search[16] = "";

	//per_page
	buffer = strstr(buffer, "per_page") + 10;
	int i = 0;
	while (buffer[i] != ',')
	{
		search[i] = buffer[i];
		i++;
	}
	search[i] = '\0';
	meta->per_page = atoi(search);

	//total
	buffer = strstr(buffer, "total") + 7;
	i = 0;
	while (buffer[i] != '}')
	{
		search[i] = buffer[i];
		i++;
	}
	search[i] = '\0';
	meta->total = atoi(search);
}

std::string getRandomWallpaperIDFromCollection(char* buffer, std::string username, std::string collectionID, bool apikeyused, std::string apikey)
{
	int page = 1;
	Meta meta;
	std::string collectionurl = "https://wallhaven.cc/api/v1/collections/";
	collectionurl.append(username);
	collectionurl.append("/");
	collectionurl.append(collectionID);
	if (apikeyused)
	{
		collectionurl.append("?apikey=");
		collectionurl.append(apikey);
	}
	getMeta(buffer, collectionurl, &meta);
	int randomWallpaper = rand() % meta.total;
	int randomPage = int(randomWallpaper / meta.per_page);
	randomWallpaper -= randomPage * meta.per_page;
	randomPage++;
	std::string pageurl = collectionurl;
	pageurl.append("&page=");
	char curpage[15] = "";
	_itoa_s(randomPage, curpage, 10);
	pageurl.append(curpage);

	char path[] = "D://query.txt";
	URLDownloadToFileA(nullptr, pageurl.c_str(), path, 0, nullptr);

	FILE* pFile;
	fopen_s(&pFile, path, "r");
	if (pFile != NULL)
	{
		fgets(buffer, 32768, pFile);
		fclose(pFile);
	}
	char search[255] = "";

	for (int i = 0; i < randomWallpaper; i++)
		buffer = strstr(buffer, "\"id\"") + 6;

	int i = 0;
	while (buffer[i] != '\"')
	{
		search[i] = buffer[i];
		i++;
	}
	search[i] = '\0';

	return search;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	std::string username = "Bridel";
	std::string collectionID = "620737";
	bool apikeyused = true;
	std::string apikey = "TUbURreToynW00fI8niwFOuA0nynXvzG";

	char* buffer = new char[32768];

	srand((unsigned int)time(NULL));
	while (true)
	{
		setWallpaper(buffer, getRandomWallpaperIDFromCollection(buffer, username, collectionID, apikeyused, apikey), apikeyused, "TUbURreToynW00fI8niwFOuA0nynXvzG");
		Sleep(5000);
	}

	return 0;
}
/*
* 620706 - general
* 620737 - woman
* 620731 - art
* 700018 - reserve
* 735880 - phone
*/