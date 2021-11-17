#pragma once

enum class CollectionType : unsigned char
{
	none,
	local,
	user,
	search
};

class Wallpaper
{
	CollectionType type;
	wchar_t* dataW;
	char* dataA;

public:
	Wallpaper(CollectionType type);
	~Wallpaper();
	CollectionType getType();
	char* getPathA();
	wchar_t* getPathW();
};