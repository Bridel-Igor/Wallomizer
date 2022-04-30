#pragma once

#include <vector>

#include "IWindow.h"
#include "Static.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "CollectionItem.h"
#include "Edit.h"
#include "Player.h"
#include "CollectionManager.h"
#include "BaseWindow.h"

class MainWindow : public IWindow
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
		LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LPCSTR ClassName() const { return "CollectionItemsFrameClass"; }

		static constexpr int height = 400;
		CollectionManager* collectionManager = nullptr;
	};

public:
	MainWindow(CollectionManager* pCollectionManager);
	~MainWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Player* player;
	static constexpr int width = 640;
	static constexpr int height = 480;
	static MainWindow* mainWindow;
	static CollectionItemsFrame* collectionItemsFrame;
	
private:
	Static *stCollections = nullptr;
	Button *btnAdd = nullptr, *btnSettings = nullptr, *btnDonate = nullptr;
	CollectionManager* m_pCollectionManager = nullptr;
};