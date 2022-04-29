#pragma once

#include "BaseWindow.h"
#include "Button.h"
#include "Player.h"
#include "CollectionManager.h"

class TrayWindow : public BaseWindow<TrayWindow>
{
public:
	TrayWindow(CollectionManager* collectionManager);
	~TrayWindow();
	void windowLoop();
	LPCSTR ClassName() const { return "Tray Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool isReady() const { return m_isReady; }
	
	static constexpr int width = 160;
	static constexpr int height = 90;
	static TrayWindow *trayWindow;

	Button* btnSettings = nullptr, * btnExit = nullptr;
	Player* player = nullptr;

private:
	HICON hStatusIcon = nullptr;
	LPCSTR pszIDStatusIcon = nullptr;
	bool m_isReady = false;
	CollectionManager* m_pCollectionManager = nullptr;
};