#include "AddCollectionWindow.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"

AddCollectionWindow::AddCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager) :
	IWindow("Add collection", "Add Collection Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100,	100,	240,	140),
	m_hCaller(hCaller),
	m_pCollectionManager(pCollectionManager),
	btnAddLocalCollection(hWnd(), "Add local collection",								10,		10,		220,	20),
	btnAddUserCollection(hWnd(), "Add wallhaven user collection",						10,		40,		220,	20),
	btnAddSearchCollection(hWnd(), "Add wallhaven search collection",					10,		70,		220,	20),
	btnCancel(hWnd(), "Cancel",														10,		110,	220,	20)
{
	centerWindow(m_hCaller);
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
	EnableWindow(m_hCaller, FALSE);
}

AddCollectionWindow::~AddCollectionWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT AddCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			ShowWindow(hWnd(), SW_HIDE);
			m_pCollectionManager->addCollection(CollectionType::user);
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnAddLocalCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			m_pCollectionManager->addCollection(CollectionType::local);
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnAddSearchCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			m_pCollectionManager->addCollection(CollectionType::search);
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORBTN:
	return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
}