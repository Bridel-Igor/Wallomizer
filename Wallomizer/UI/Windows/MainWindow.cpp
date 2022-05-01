#include <thread>

#include "MainWindow.h"
#include "Settings.h"
#include "AddCollectionWindow.h"
#include "SettingsWindow.h"
#include "TrayWindow.h"
#include "Delay.h"

MainWindow* MainWindow::s_pMainWindow = nullptr;

MainWindow::MainWindow(CollectionManager* pCollectionManager) :
	IWindow("Wallomizer", "Main Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_pCollectionManager(pCollectionManager),
	stCollections		(hWnd(), "Collections:",		20,		10,		100,	20),
	btnAdd				(hWnd(), "Add collection..",	530,	10,		100,	20),

	collectionItemsFrame(hWnd(), pCollectionManager,	10,		40, width - 20, 400),

	btnSettings			(hWnd(), "Settings",			10,		450,	95,		20),
	player				(hWnd(),						250,	450,
														400,	450,	100,	20, m_pCollectionManager),
	btnDonate			(hWnd(), "Donate",				535,	450,	95,		20)
{
	s_pMainWindow = this;
	while (!m_pCollectionManager->isReady())
		Sleep(50);
	centerWindow(GetDesktopWindow());
	EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	player.updateTimer(true);
	// HACK: Redo this loop in the same thread or force thread to end on destruction. Thread leeks memory as it doesn't end...
	std::thread thr([&]() {collectionItemsFrame.windowLoop(); });
	thr.detach();
	ShowWindow(collectionItemsFrame.hWnd(), SW_SHOWNORMAL);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

MainWindow::~MainWindow()
{
	s_pMainWindow = nullptr;
	ShowWindow(hWnd(), SW_HIDE);
}

LRESULT MainWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SHOWWINDOW:
	{
		if (wParam == TRUE)
		{
			player.updateTimer(true);
			player.redrawPlayers();
		}
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (player.draw(pDIS))
			return TRUE;
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, WindowStyles::mainBkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		if (btnAdd.isClicked(wParam))
		{
			AddCollectionWindow addCollectionWindow(m_pCollectionManager);
			addCollectionWindow.windowLoop();
			return 0;
		}
		if (player.click(wParam))
			return 0;
		if (btnSettings.isClicked(wParam))
		{
			SettingsWindow settingsWindow;
			settingsWindow.windowLoop();
			Settings::saveSettings();
			return 0;
		}
		if (btnDonate.isClicked(wParam))
		{
			ShellExecute(0, 0, "https://donatello.to/IgorBridel", 0, 0, SW_SHOW);
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, WindowStyles::mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)WindowStyles::mainBkBrush;
	}
	return 0;

	case WM_CTLCOLORBTN:
	{	
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}
	return 0;
	
	case WM_SETCURSOR:
	{
		player.mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	} 

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}