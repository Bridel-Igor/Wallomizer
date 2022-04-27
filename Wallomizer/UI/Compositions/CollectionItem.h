#pragma once

#include "Static.h"
#include "IconButton.h"
#include "CheckBox.h"
#include "BaseCollection.h"
#include "CategoriesAndPurityComponents.h"

class CollectionItem
{
private:
	int x, y, width;

public:
	CollectionItem(HWND hParent, int x, int y, int width, BaseCollection* collection, HFONT hFont);
	~CollectionItem();
	void updateInfo(BaseCollection* collection);
	void reposition(int yPos, bool scrollBarISVisible);
	bool draw(LPDRAWITEMSTRUCT& pDIS);
	void mouseHovering(WPARAM wParam);

	CheckBox* chboEnabled;
	Static* stName, * stNumber;
	IconButton *btnSettings, *btnDelete;
	BaseCollection* collection;
	PurityComponent* purity;
	static int height;
};