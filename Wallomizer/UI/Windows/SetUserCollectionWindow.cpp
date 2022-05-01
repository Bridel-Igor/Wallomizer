#include "SetUserCollectionWindow.h"
#include "MainWindow.h"
#include "Settings.h"

SetUserCollectionWindow::SetUserCollectionWindow(UserCollection* pCollection, CollectionManager* pCollectionManager) :
	IWindow("User collection", "Set User Collection Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_pCurrentUserCollection(pCollection),
	m_pCollectionManager	(pCollectionManager),
	stUsername				(hWnd(), "Username:",		10,		10,		80,		20, SS_RIGHT),
	edUsername				(hWnd(), "",				100,	10,		240,	20),
	stCollectionID			(hWnd(), "Collection ID:",	10,		40,		80,		20, SS_RIGHT),
	cbCollections			(hWnd(), "",				100,	40,		240,	20),

	stPurity				(hWnd(), "Purity:",			10,		70,		80,		20, SS_RIGHT),
	purCom					(hWnd(),					100,	70,		240,	20),

	btnCancel				(hWnd(), "Cancel",			10,		100,	80,		20),
	btnOk					(hWnd(), "Ok",				100,	100,	240,	20)
{
	// Review: how to improve exception handling
	if (m_pCurrentUserCollection == nullptr)
		return;

	EnableWindow(MainWindow::s_pMainWindow->hWnd(), FALSE);
		
	edUsername.setTextA(m_pCurrentUserCollection->settings.sUsername);
	purCom.setPurity(m_pCurrentUserCollection->settings.categoriesAndPurity);

	if (edUsername.isEmpty())
	{
		edUsername.setTextA(Settings::username);
		strcpy_s(m_pCurrentUserCollection->settings.sUsername, Settings::username);
	}

	if (m_pCurrentUserCollection->settings.sCollectionID!=0 && strlen(m_pCurrentUserCollection->settings.sCollectionName)!=0)
	{
		list.clear();
		UserCollection::UserCollectionInfo info;
		info.id = atoi(m_pCurrentUserCollection->settings.sCollectionID);
		strcpy_s(info.sLabel, m_pCurrentUserCollection->settings.sCollectionName);
		list.push_back(info);
		SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
		SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)list[0].sLabel);
		SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		validCollection = true;
	}

	EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);

	centerWindow(MainWindow::s_pMainWindow->hWnd());
	ShowWindow(hWnd(), SW_SHOWNORMAL);
}

SetUserCollectionWindow::~SetUserCollectionWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(MainWindow::s_pMainWindow->hWnd(), TRUE);
	SetForegroundWindow(MainWindow::s_pMainWindow->hWnd());
}

LRESULT SetUserCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (purCom.draw(pDIS))
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
		if (purCom.click(wParam))
			return 0;
		if ((HWND)lParam==edUsername.hWnd() && HIWORD(wParam) == EN_CHANGE)
		{
			SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Click to update");
			SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			validCollection = false;
			return 0;
		}
		
		if ((HWND)lParam == cbCollections.hWnd() && HIWORD(wParam) == CBN_DROPDOWN)
		{
			char prevName[64] = {0};
			if (!list.empty() && validCollection)
				strcpy_s(prevName, list[cbCollections.getSelectedItem()].sLabel);

			validCollection = false;
			SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Updating...");
			SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			char tmpUsername[64];
			edUsername.getTextA(tmpUsername, 64);
			list.clear();
			list = UserCollection::loadCollectionList(tmpUsername, Settings::apiKey);
			SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			if (list.empty())
			{
				SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Empty");
				SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				validCollection = false;
				return 0;
			}
			for (size_t i=0;i<list.size(); i++)
				SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)list[i].sLabel);

			int index = 0;
			if (strlen(prevName))
			{
				index = (int)SendMessageA(cbCollections.hWnd(), CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)prevName);
				if (index == CB_ERR)
					index = 0;
			}
			SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)index, (LPARAM)0);
			validCollection = true;
			return 0;
		}

		if (btnOk.isClicked(wParam))
		{
			if (edUsername.isEmpty() || !validCollection)
			{
				MessageBoxA(nullptr, "Invalid data", "Wallomizer", MB_OK | MB_ICONEXCLAMATION);
				return 0;
			}
			strcpy_s(m_pCurrentUserCollection->settings.sCollectionName, list[cbCollections.getSelectedItem()].sLabel);
			char cid[16] = { 0 };
			_itoa_s(list[cbCollections.getSelectedItem()].id, cid, 10);
			strcpy_s(m_pCurrentUserCollection->settings.sCollectionID, cid);

			edUsername.getTextA(m_pCurrentUserCollection->settings.sUsername, 64);
			m_pCurrentUserCollection->settings.categoriesAndPurity = purCom.getPurity();
			if (m_pCurrentUserCollection->isValid() == false)
				m_pCurrentUserCollection->setValid(true);
			else
				m_pCollectionManager->reloadSettings();
			DestroyWindow(hWnd);
			return 0;
		}
		
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd);
			return 0;
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		purCom.mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	case WM_CTLCOLORSTATIC:
	{
		HWND hWndStatic = (HWND)lParam;
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, WindowStyles::mainFontColor);
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)WindowStyles::mainBkBrush;
	}
	return 0;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, WindowStyles::editFontColor);
		SetBkColor(hdc, WindowStyles::editBkColor);
		SetDCBrushColor(hdc, WindowStyles::editBkColor);
		return (LRESULT)GetStockObject(DC_BRUSH);
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