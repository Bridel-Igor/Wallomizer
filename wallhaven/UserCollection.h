#pragma once
#include <string>
#include "Settings.h"

class UserCollection
{
public:
	UserCollection(Settings *_settings);
	~UserCollection();
	bool setRandomWallpaper();

private:
	struct Meta
	{
		int per_page, total;
	}*meta;

	char *buffer, *pBuffer;
	Settings* settings;
	FILE* pFile;
	static char queryPath[];
};