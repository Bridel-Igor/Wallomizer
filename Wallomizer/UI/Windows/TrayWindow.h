#pragma once

#include "BaseWindow.h"
#include "Button.h"
#include "Player.h"
#include "CollectionManager.h"

class TrayWindow : public BaseWindow<TrayWindow>
{
public:
	LPCSTR ClassName() const { return "Tray Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(CollectionManager* collectionManager);
	static bool isReady();
	
	Button* btnSettings, * btnExit;
	Player* player;
	
	static constexpr int width = 160;
	static constexpr int height = 90;
	static TrayWindow *trayWindow;

private:
	HICON hStatusIcon = nullptr;
	LPCSTR pszIDStatusIcon = nullptr;
	static bool s_isReady;
	CollectionManager* collectionManager = nullptr;
};