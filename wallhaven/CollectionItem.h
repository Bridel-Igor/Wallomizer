#pragma once

#include "Static.h"
#include "Button.h"
#include "CheckBox.h"
#include "BaseCollection.h"

class CollectionItem
{
private:
	int x, y, width, height;

public:
	CollectionItem(HWND hParent, int _x, int _y, int _width, int _height, BaseCollection* collection, HFONT hFont);
	~CollectionItem();
	void updateInfo(BaseCollection* collection);
	void scroll(int yPos);

	CheckBox* chboEnabled;
	Static* stName, * stNumber;
	Button *btnSettings, *btnDelete;
};