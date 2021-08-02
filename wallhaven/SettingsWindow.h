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
	class CollectionItemsFrame : public BaseWindow<CollectionItemsFrame>
	{
	private:
		void destroyCollectionItems();
		void updateScroll();

		std::vector<CollectionItem*> collectionItems;
		HFONT font;
		HBRUSH bkBrush;

		HDC hdc;
		PAINTSTRUCT ps;
		SCROLLINFO si;
		int yMinScroll;
		int yCurrentScroll;
		int yMaxScroll;

	public:
		void updateCollectionItems();
		LPCSTR ClassName() const { return "CollectionItemsFrameClass"; }
		LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static constexpr int height = 380;
	};

private:
	Static *stCollections, *stHours, *stMinutes, *stSeconds, *stDelay;
	Button *btnOk, *btnAdd;
	UpDownEdit *udeHours, *udeMinutes, *udeSeconds;
	HFONT font;
	HBRUSH bkBrush;
	
public:
	LPCSTR ClassName() const { return "Settings Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();

	static constexpr int width = 640;
	static constexpr int height = 480;
	static SettingsWindow* settingsWindow;
	static CollectionItemsFrame* collectionItemsFrame;
};