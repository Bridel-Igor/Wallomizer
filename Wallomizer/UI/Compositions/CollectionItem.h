#pragma once

#include "Static.h"
#include "IconButton.h"
#include "CheckBox.h"
#include "CategoriesAndPurityComponents.h"

class BaseCollection;

class CollectionItem
{
public:
	/// Class manages resources (de)allocation for all CollectionItem resources.
	class Resources
	{
	public:
		Resources();
		~Resources();

		static HICON hIOptions, hIOptionsHover, hIDelete, hIDeleteHover;
		static HBRUSH collItemBkBrush;
		static COLORREF collItemFontColor;
		static COLORREF collItemBkColor;
	private:
		static unsigned char refCount;
	}resources;
public:
	CollectionItem(IComponent* pParent, int x, int y, int width, HFONT hFont);
	~CollectionItem() {}
	void updateInfo(BaseCollection& collection);
	void reposition(int yPos, bool scrollBarISVisible);
	bool draw(LPDRAWITEMSTRUCT& pDIS);

	CheckBox chboEnabled;
	Static stName, stNumber;
	IconButton btnSettings, btnDelete;
	PurityComponent purity;
	static constexpr int height = 26;

private:
	int x, y, width;
};
