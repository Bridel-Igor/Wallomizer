#pragma once

#include <list>

#include "IWindow.h"
#include "Button.h"
#include "Player.h"
#include "CollectionManager.h"

class TrayWindow : public IWindow
{
public:
	TrayWindow(CollectionManager* pCollectionManager);
	~TrayWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	static constexpr int width = 160;
	static constexpr int height = 90;
	static TrayWindow *trayWindow;

private: 
	CollectionManager* m_pCollectionManager;
	
public:
	Button btnSettings, btnExit;
	Player player;

private:
	HICON hStatusIcon = nullptr;
	LPCSTR pszIDStatusIcon = nullptr;
};