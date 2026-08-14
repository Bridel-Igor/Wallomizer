#pragma once

#include <cstdint>

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
	private:
		static std::uint16_t refCount;
	}resources;

public:
	CollectionItem(IComponent* pParent, int x, int y, int width);
	~CollectionItem() = default;
	void updateInfo(const BaseCollection& collection);
	void reposition(int yPos, bool scrollBarISVisible);
	bool draw(LPDRAWITEMSTRUCT pDIS);

	CheckBox chboEnabled;
	Static stName, stNumber;
	IconButton btnSettings, btnDelete;
	PurityComponent purity;

	static constexpr int height = 26;
	static constexpr int scrollbarOffset = 18;

private:
	int x, y, width;
};
