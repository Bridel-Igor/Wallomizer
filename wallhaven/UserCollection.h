#pragma once
#include <string>
#include "BaseCollection.h"

class UserCollection : public BaseCollection
{
private:
	struct UserCollectionSettings
	{
		char username[64];
		char collectionID[16];
		bool isApiKeyUsed;
		char apiKey[64];
	};
public:
	UserCollection();
	~UserCollection();
	bool saveSettings(FILE* pFile);
	bool loadSettings(FILE* pFile);
	bool setRandomWallpaper();
	LPCSTR collectionType() const { return "User collection"; }
	LPCSTR collectionName() const;
	unsigned int getNumber() { return number; }
	void openCollectionSettingsWindow();

	UserCollectionSettings* settings;

private:
	unsigned int number = 0;
	int per_page;
	char *buffer, *pBuffer;
	FILE* pFile;
	static char queryPath[];
	std::string collectionUrl;
};