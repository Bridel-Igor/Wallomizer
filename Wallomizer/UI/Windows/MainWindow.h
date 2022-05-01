#pragma once

#include "IWindow.h"
#include "Static.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "Edit.h"
#include "Player.h"
#include "CollectionManager.h"
#include "CollectionItemsFrame.h"

class MainWindow : public IWindow
{
public:
	MainWindow(CollectionManager* pCollectionManager);
	~MainWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int width = 640;
	static constexpr int height = 480;
	static MainWindow* mainWindow;
	CollectionItemsFrame collectionItemsFrame;
	
private:
	CollectionManager* m_pCollectionManager;
	Static stCollections;
	Button btnAdd, btnSettings, btnDonate;	
	Player player;
};