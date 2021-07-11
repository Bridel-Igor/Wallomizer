#pragma once

class UserCollectionSettings
{
public:
	static unsigned long delay;
	char username[64];
	char collectionID[16];
	bool isApiKeyUsed;
	char apiKey[64];

	bool saveSettings();
	void loadSettings();
};