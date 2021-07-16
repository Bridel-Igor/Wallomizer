#pragma once

#include <vector>

#include "BaseWindow.h"
#include "Static.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CollectionItem.h"

class SettingsWindow : public BaseWindow<SettingsWindow>
{
private:
	void destroyCollectionItems();

	std::vector<CollectionItem*> collectionItems;
	Static *stCollections, *stHours, *stMinutes, *stSeconds, *stDelay;
	Button *btnOk, *btnAdd;
	UpDownEdit *udeHours, *udeMinutes, *udeSeconds;
	HFONT font;
	HBRUSH bkBrush;
	
public:
	void updateCollectionItems();
	LPCSTR ClassName() const { return "Settings Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();
	
	static SettingsWindow* settingsWindow;
};