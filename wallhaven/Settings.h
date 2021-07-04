#pragma once

class Settings
{
public:
	unsigned long delay;
	char username[64];
	char collectionID[16];
	bool apikeyused;
	char apikey[64];

	Settings();
	bool saveSettings();
	bool loadSettings();
};