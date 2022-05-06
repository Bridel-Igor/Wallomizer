#include "SetUserCollectionWindow.h"
#include "Settings.h"

SetUserCollectionWindow::SetUserCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager, UserCollection* pCollection) :
	IWindow("User collection", "Set User Collection Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_hCaller(hCaller),
	m_pCollectionManager	(pCollectionManager),
	m_pCurrentUserCollection(pCollection),
	stUsername				(hWnd(), "Username:",		10,		10,		80,		20, SS_RIGHT),
	edUsername				(hWnd(), "",				100,	10,		240,	20),
	stCollectionID			(hWnd(), "Collection ID:",	10,		40,		80,		20, SS_RIGHT),
	cbCollections			(hWnd(), "",				100,	40,		240,	20),

	stPurity				(hWnd(), "Purity:",			10,		70,		80,		20, SS_RIGHT),
	purCom					(hWnd(),					100,	70,		240,	20),

	btnCancel				(hWnd(), "Cancel",			10,		100,	80,		20),
	btnOk					(hWnd(), "Ok",				100,	100,	240,	20)
{
	edUsername.setTextA(m_pCurrentUserCollection->settings.sUsername);
	purCom.setPurity(m_pCurrentUserCollection->settings.categoriesAndPurity);

	if (edUsername.isEmpty())
	{
		edUsername.setTextA(Settings::username);
		strcpy_s(m_pCurrentUserCollection->settings.sUsername, Settings::username);
	}

	if (m_pCurrentUserCollection->settings.sCollectionID!=0 && strlen(m_pCurrentUserCollection->settings.sCollectionName)!=0)
	{
		uciList.clear();
		UserCollection::UserCollectionInfo info;
		info.id = atoi(m_pCurrentUserCollection->settings.sCollectionID);
		strcpy_s(info.sLabel, m_pCurrentUserCollection->settings.sCollectionName);
		uciList.push_back(info);
		SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
		SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)uciList.begin()->sLabel);
		SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		validCollection = true;
	}

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
	centerWindow(m_hCaller);
	EnableWindow(m_hCaller, FALSE);
	SetForegroundWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
	SetForegroundWindow(hWnd());
}

SetUserCollectionWindow::~SetUserCollectionWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT SetUserCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			if (!uciList.empty() && validCollection)
			{
				auto uci = uciList.begin();
				std::advance(uci, cbCollections.getSelectedItem());
				strcpy_s(prevName, uci->sLabel);
			}

			validCollection = false;
			SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Updating...");
			SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			char tmpUsername[64];
			edUsername.getTextA(tmpUsername, 64);
			uciList.clear();
			UserCollection::loadCollectionList(uciList, tmpUsername, Settings::apiKey);
			SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			if (uciList.empty())
			{
				SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)"Empty");
				SendMessageA(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				validCollection = false;
				return 0;
			}
			for (const auto& item : uciList)
				SendMessageA(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)item.sLabel);

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

			auto uci = uciList.begin();
			std::advance(uci, cbCollections.getSelectedItem());
			strcpy_s(m_pCurrentUserCollection->settings.sCollectionName, uci->sLabel);
			char cid[16] = { 0 };
			_itoa_s(uci->id, cid, 10);
			strcpy_s(m_pCurrentUserCollection->settings.sCollectionID, cid);

			edUsername.getTextA(m_pCurrentUserCollection->settings.sUsername, 64);
			m_pCurrentUserCollection->settings.categoriesAndPurity = purCom.getPurity();
			if (m_pCurrentUserCollection->isValid() == false)
				m_pCurrentUserCollection->setValid(true);
			else
				m_pCollectionManager->reloadSettings();
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

	case WM_SETCURSOR:
	{
		purCom.mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return RESULT_DEFAULT;
	}
}