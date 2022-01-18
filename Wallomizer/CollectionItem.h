#pragma once

#include "Static.h"
#include "Button.h"
#include "CheckBox.h"
#include "BaseCollection.h"
#include "CategoriesAndPurity.h"

class CollectionItem
{
private:
	int x, y, width, height;

public:
	CollectionItem(HWND hParent, int x, int y, int width, int height, BaseCollection* collection, HFONT hFont);
	~CollectionItem();
	void updateInfo(BaseCollection* collection);
	void reposition(int yPos);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	bool notify(HWND hWnd);

	CheckBox* chboEnabled;
	Static* stName, * stNumber;
	Button *btnSettings, *btnDelete;
	BaseCollection* collection;
	PurityComponent* purity;
};