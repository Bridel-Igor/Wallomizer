#include "SetUserCollectionWindow.h"
#include "Settings.h"

SetUserCollectionWindow::SetUserCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager, UserCollection* pCollection) :
	IWindow("User collection", "Set User Collection Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, width, height),
	m_hCaller(hCaller),
	m_pCollectionManager	(pCollectionManager),
	m_pCurrentUserCollection(pCollection),
	stUsername				(this, "Username:",			10,		10,		80,		20, SS_RIGHT),
	edUsername				(this, "",					100,	10,		240,	20),
	stCollectionID			(this, "Collection ID:",	10,		40,		80,		20, SS_RIGHT),
	cbCollections			(this, "",					100,	40,		240,	20),

	stPurity				(this, "Purity:",			10,		70,		80,		20, SS_RIGHT),
	purCom					(this,						100,	70,		240,	20),

	btnCancel				(this, "Cancel",			10,		100,	80,		20),
	btnOk					(this, "Ok",				100,	100,	240,	20)
{
	edUsername.setTextW(m_pCurrentUserCollection->settings.wsUsername);
	purCom.setPurity(m_pCurrentUserCollection->settings.categoriesAndPurity);

	if (edUsername.isEmpty())
	{
		edUsername.setTextW(Settings::username);
		wcscpy_s(m_pCurrentUserCollection->settings.wsUsername, Settings::username);
		SendMessageW(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
		SendMessageW(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)L"Click to update");
		SendMessageW(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
		validCollection = false;
	}

	if (m_pCurrentUserCollection->settings.wsCollectionID[0] && m_pCurrentUserCollection->settings.wsCollectionName[0])
	{
		uciList.clear();
		UserCollection::UserCollectionInfo info;
		info.id = _wtoi(m_pCurrentUserCollection->settings.wsCollectionID);
		wcscpy_s(info.wsLabel, m_pCurrentUserCollection->settings.wsCollectionName);
		uciList.push_back(info);
		SendMessageA(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
		SendMessageW(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)uciList.begin()->wsLabel);
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
			SendMessageW(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageW(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)L"Click to update");
			SendMessageW(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			validCollection = false;
			return 0;
		}
		if ((HWND)lParam == cbCollections.hWnd() && HIWORD(wParam) == CBN_DROPDOWN)
		{
			wchar_t prevName[64] = {0};
			if (!uciList.empty() && validCollection)
			{
				auto uci = uciList.begin();
				std::advance(uci, cbCollections.getSelectedItem());
				wcscpy_s(prevName, uci->wsLabel);
			}

			validCollection = false;
			SendMessageW(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			SendMessageW(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)L"Updating...");
			SendMessageW(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			wchar_t tmpUsername[64];
			edUsername.getTextW(tmpUsername, 64);
			uciList.clear();
			UserCollection::loadCollectionList(uciList, tmpUsername, Settings::apiKey);
			SendMessageW(cbCollections.hWnd(), CB_RESETCONTENT, NULL, NULL);
			if (uciList.empty())
			{
				SendMessageW(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)L"Empty");
				SendMessageW(cbCollections.hWnd(), CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
				validCollection = false;
				return 0;
			}
			for (const auto& item : uciList)
				SendMessageW(cbCollections.hWnd(), CB_ADDSTRING, NULL, (LPARAM)item.wsLabel);

			int index = 0;
			if (prevName[0])
			{
				index = (int)SendMessageW(cbCollections.hWnd(), CB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)prevName);
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
			wcscpy_s(m_pCurrentUserCollection->settings.wsCollectionName, uci->wsLabel);
			wchar_t cid[16] = { 0 };
			_itow_s(uci->id, cid, 10);
			wcscpy_s(m_pCurrentUserCollection->settings.wsCollectionID, cid);

			edUsername.getTextW(m_pCurrentUserCollection->settings.wsUsername, 64);
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

	default:
		return RESULT_DEFAULT;
	}
}