#pragma once

#include "Static.h"
#include "IconButton.h"
#include "CheckBox.h"
#include "CategoriesAndPurityComponents.h"
#include "SharedResources.h"

class BaseCollection;

class CollectionItem
{
private:
	struct Resources
	{
	public:
		Resources();
		~Resources();

		HICON iconOptions = nullptr, iconOptionsHover = nullptr, iconDelete = nullptr, iconDeleteHover = nullptr;
		HBRUSH brushBk = nullptr;
	};

public:
	CollectionItem(IComponent* pParent, int x, int y, int width);
	~CollectionItem() = default;

	void updateInfo(const BaseCollection& collection);
	void reposition(int yPos, bool scrollBarIsVisible);
	bool draw(LPDRAWITEMSTRUCT drawItem);
	LRESULT handleColor(HWND hWnd, HDC hdc) const;

	bool toggle() noexcept;
	bool isSettingsClicked(WPARAM wParam) const noexcept { return btnSettings.isClicked(wParam); }
	bool isDeleteClicked(WPARAM wParam) const noexcept { return btnDelete.isClicked(wParam); }
	bool isCheckboxClicked(WPARAM wParam) const noexcept { return chboEnabled.isClicked(wParam); }

	static constexpr int height = 26;

private:
	SharedResources<Resources> m_resources;
	CheckBox chboEnabled;
	Static stName, stNumber;
	IconButton btnSettings, btnDelete;
	PurityComponent purity;

	static constexpr int scrollbarOffset = 18;

	int x, y, width;
};
