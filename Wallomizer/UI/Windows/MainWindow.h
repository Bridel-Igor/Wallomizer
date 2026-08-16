#pragma once

#include <list>

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
	~MainWindow() noexcept;

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	void updateCollectionItems();
	void updateScroll();

	static constexpr int width = 640;
	static constexpr int height = 480;

	static constexpr int panelWidth = width - 20;
	static constexpr int panelHeight = 400;
	static constexpr int panelX = 10;
	static constexpr int panelY = 40;

	App& m_app;
	HBRUSH bkBrush;

	Static stCollections;
	Panel collectionsPanel;
	Static stEmpty;
	Button btnAdd, btnSettings;
	Player player;
	
	std::list<CollectionItem> collectionItems;
	SCROLLINFO si{};
	int yMinScroll = 0;
	int yCurrentScroll = 0;
	int yMaxScroll = 0;
	bool scrollBarIsVisible;
};
