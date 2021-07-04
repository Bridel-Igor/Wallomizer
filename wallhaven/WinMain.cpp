#include <string>
#include <windows.h>
#include "time.h"
#include "Urlmon.h"
#pragma comment(lib, "Urlmon.lib")

#include "Settings.h"

bool setWallpaper(char* buffer, std::string wallpaperid, Settings *settings)
{
	std::string wallpaperurl = "https://wallhaven.cc/api/v1/w/";
	wallpaperurl.append(wallpaperid);
	if (settings->apiKeyUsed)
	{
		wallpaperurl.append("?apikey=");
		wallpaperurl.append(settings->apiKey);
	}

	char path[] = "Query.txt";
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
	while (buffer != nullptr && buffer[i] != '\"')
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

	std::string pathimg="";
	char s[255]="Current wallpaper.jpg";
	URLDownloadToFileA(nullptr, imgurl, s, 0, nullptr);
	GetCurrentDirectoryA(255, s);
	pathimg.append(s);
	pathimg.append("\\Current wallpaper.jpg");
	strcpy_s(s, pathimg.c_str());
	if (SystemParametersInfoA(SPI_SETDESKWALLPAPER, 0, s, SPIF_UPDATEINIFILE))
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
	char path[] = "query.txt";
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
	while (buffer != nullptr && buffer[i] != ',')
	{
		search[i] = buffer[i];
		i++;
	}
	search[i] = '\0';
	meta->per_page = atoi(search);

	//total
	if (buffer!=nullptr)
		buffer = strstr(buffer, "total") + 7;
	i = 0;
	while (buffer != nullptr && buffer[i] != '}')
	{
		search[i] = buffer[i];
		i++;
	}
	search[i] = '\0';
	meta->total = atoi(search);
}

std::string getRandomWallpaperIDFromCollection(char* buffer, Settings *settings)
{
	int page = 1;
	Meta meta;
	std::string collectionurl = "https://wallhaven.cc/api/v1/collections/";
	collectionurl.append(settings->username);
	collectionurl.append("/");
	collectionurl.append(settings->collectionID);
	if (settings->apikeyused)
	{
		collectionurl.append("?apikey=");
		collectionurl.append(settings->apikey);
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

	char path[] = "query.txt";
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
	while (buffer!=nullptr && buffer[i] != '\"')
	{
		search[i] = buffer[i];
		i++;
	}
	search[i] = '\0';

	return search;
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Settings *settings = new Settings;
	char* buffer = new char[32768];
	srand((unsigned int)time(NULL));
	while (true)
	{
		setWallpaper(buffer, getRandomWallpaperIDFromCollection(buffer, settings), settings);
		Sleep(settings->delay);
	}
	delete [] buffer;
	return 0;
}