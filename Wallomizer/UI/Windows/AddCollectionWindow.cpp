#include "AddCollectionWindow.h"
#include "MainWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"

AddCollectionWindow* AddCollectionWindow::addCollectionWindow = nullptr;

AddCollectionWindow::AddCollectionWindow(CollectionManager* pCollectionManager) :
	IWindow("Add collection", "Add Collection Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100,	100,	240,	140),
	m_pCollectionManager(pCollectionManager),
	btnAddLocalCollection(this->hWnd(), "Add local collection",								10,		10,		220,	20),
	btnAddUserCollection(this->hWnd(), "Add wallhaven user collection",						10,		40,		220,	20),
	btnAddSearchCollection(this->hWnd(), "Add wallhaven search collection",					10,		70,		220,	20),
	btnCancel(this->hWnd(), "Cancel",														10,		110,	220,	20)
{
	if (addCollectionWindow)
	{
		SetForegroundWindow(addCollectionWindow->hWnd());
		return;
	}
	addCollectionWindow = this;
	addCollectionWindow->centerWindow(MainWindow::mainWindow->hWnd());
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	ShowWindow(addCollectionWindow->hWnd(), SW_SHOWNORMAL);
	EnableWindow(MainWindow::mainWindow->hWnd(), FALSE);		
}

AddCollectionWindow::~AddCollectionWindow()
{
	ShowWindow(addCollectionWindow->hWnd(), SW_HIDE);
	EnableWindow(MainWindow::mainWindow->hWnd(), TRUE);
	SetForegroundWindow(MainWindow::mainWindow->hWnd());
	addCollectionWindow = nullptr;
}

LRESULT AddCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
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
		if (btnAddUserCollection.isClicked(wParam))
		{
			ShowWindow(this->hWnd(), SW_HIDE);
			m_pCollectionManager->addCollection<UserCollection>();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnAddLocalCollection.isClicked(wParam))
		{
			ShowWindow(this->hWnd(), SW_HIDE);
			m_pCollectionManager->addCollection<LocalCollection>();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnAddSearchCollection.isClicked(wParam))
		{
			ShowWindow(this->hWnd(), SW_HIDE);
			m_pCollectionManager->addCollection<SearchCollection>();
			DestroyWindow(this->hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(this->hWnd());
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}