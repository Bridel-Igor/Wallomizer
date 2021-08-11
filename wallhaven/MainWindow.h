#pragma once

#include <vector>

#include "BaseWindow.h"
#include "Static.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CollectionItem.h"
#include "Edit.h"

class MainWindow : public BaseWindow<MainWindow>
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

		static constexpr int height = 400;
	};

private:
	Static *stCollections;
	Button *btnAdd, *btnSetUserData;
	HFONT font;
	HBRUSH bkBrush;
	
public:
	LPCSTR ClassName() const { return "Main Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();

	static constexpr int width = 640;
	static constexpr int height = 480;
	static MainWindow* mainWindow;
	static CollectionItemsFrame* collectionItemsFrame;
};