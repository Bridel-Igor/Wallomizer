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
public:
	Wallpaper(CollectionType type);
	~Wallpaper();
	CollectionType getType();
	char* getPathA();
	wchar_t* getPathW();

private:
	union CharX
	{
		wchar_t* w;
		char* a;
	}charX;
	CollectionType type;
};