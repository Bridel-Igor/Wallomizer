#pragma once
#include <string>
#include "UserCollectionSettings.h"

class UserCollection
{
public:
	UserCollection();
	~UserCollection();
	bool setRandomWallpaper();

private:
	struct Meta
	{
		int per_page, total;
	}*meta;

	char *buffer, *pBuffer;
	UserCollectionSettings* settings;
	FILE* pFile;
	static char queryPath[];
	std::string collectionUrl;
};