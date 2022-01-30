#pragma once

#include <vector>

#include "BaseWindow.h"
#include "Static.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CollectionItem.h"
#include "Edit.h"
#include "Player.h"

class MainWindow : public BaseWindow<MainWindow>
{
private:
	class CollectionItemsFrame : public BaseWindow<CollectionItemsFrame>
	{
	private:
		void destroyCollectionItems();
		void updateScroll();

		std::vector<CollectionItem*> collectionItems;
		Static *stEmpty;

		HDC hdc;
		PAINTSTRUCT ps;
		SCROLLINFO si;
		int yMinScroll;
		int yCurrentScroll;
		int yMaxScroll;
		bool scrollBarIsVisible;

	public:
		void updateCollectionItems();
		LPCSTR ClassName() const { return "CollectionItemsFrameClass"; }
		LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		static constexpr int height = 400;
	};

private:
	Static *stCollections;
	Button *btnAdd, *btnSettings, *btnDonate;
	
public:
	LPCSTR ClassName() const { return "Main Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();
	static bool isReady();

	Player* player;
	static constexpr int width = 640;
	static constexpr int height = 480;
	static MainWindow* mainWindow;
	static CollectionItemsFrame* collectionItemsFrame;
	static bool s_isReady;
};