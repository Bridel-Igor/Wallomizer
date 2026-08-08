#pragma once

#include "IWindow.h"
#include "Static.h"
#include "Button.h"
#include "UpDownEdit.h"
#include "Edit.h"
#include "Player.h"
#include "CollectionItem.h"
#include "Panel.h"

class App;

class MainWindow : public IWindow
{
public:
	MainWindow(App& app);
	~MainWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void updateCollectionItems();

	static constexpr int width = 640;
	static constexpr int height = 480;
	static MainWindow* s_pMainWindow;
	
private:
	void destroyCollectionItems();
	void updateScroll();

	App& m_app;

	COLORREF bkColor;
	HBRUSH bkBrush;
	int fWidth, fHeight, fX, fY;

	Static stCollections;
	Panel collectionsPanel;
	Static stEmpty;
	Button btnAdd, btnSettings;
	Player player;
	
	std::list<CollectionItem> collectionItems;
	SCROLLINFO si;
	int yMinScroll = 0;
	int yCurrentScroll = 0;
	int yMaxScroll = 0;
	bool scrollBarIsVisible;
};
