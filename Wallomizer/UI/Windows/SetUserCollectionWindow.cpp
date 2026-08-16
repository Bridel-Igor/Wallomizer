#include "SetUserCollectionWindow.h"

#include "Settings.h"

SetUserCollectionWindow::SetUserCollectionWindow(IWindow* pOwner, const Settings& settings, UserCollection& userCollection) :
	IWindow(pOwner, "User collection", "Set User Collection Window Class", WS_CAPTION | WS_SYSMENU, 0, 100, 100, width, height),
	m_settings(settings),
	m_userCollection(userCollection),
	stUsername				(this, "Username:",							10,		10,		80,		20, SS_RIGHT),
	edUsername				(this, m_userCollection.settings.username,	100,	10,		240,	20),
	stCollectionID			(this, "Collection ID:",					10,		40,		80,		20, SS_RIGHT),
	cbCollections			(this, L"",									100,	40,		240,	20),

	stPurity				(this, "Purity:",							10,		70,		80,		20, SS_RIGHT),
	purCom					(this,										100,	70,		240,	20),

	btnCancel				(this, "Cancel",							10,		100,	80,		20),
	btnOk					(this, "Ok",								100,	100,	240,	20)
{
	purCom.setPurity(m_userCollection.settings.categoriesAndPurity);

	if (edUsername.isEmpty())
	{
		edUsername.setText(m_settings.getData().username);
		cbCollections.clear();
		cbCollections.addOption(L"Click to update");
		cbCollections.selectOption(0);
		validCollection = false;
	}

	if (!m_userCollection.settings.collectionID.empty() && !m_userCollection.settings.collectionName.empty())
	{
		uciList.clear();
		UserCollection::UserCollectionInfo info;
		info.id = std::stoi(m_userCollection.settings.collectionID.c_str());
		info.label = m_userCollection.settings.collectionName;
		uciList.push_back(info);
		cbCollections.clear();
		cbCollections.addOption(uciList.begin()->label);
		cbCollections.selectOption(0);
		validCollection = true;
	}
}

LRESULT SetUserCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (purCom.draw(drawItem))
			return TRUE;
		break;
	}

	case WM_COMMAND:
	{
		const HWND source = reinterpret_cast<HWND>(lParam);

		if (purCom.click(wParam))
			return 0;
		if (source ==edUsername.hWnd() && HIWORD(wParam) == EN_CHANGE)
		{
			cbCollections.clear();
			cbCollections.addOption(L"Click to update");
			cbCollections.selectOption(0);
			validCollection = false;
			return 0;
		}
		if (source == cbCollections.hWnd() && HIWORD(wParam) == CBN_DROPDOWN)
		{
			std::wstring prevName;
			if (!uciList.empty() && validCollection)
			{
				const int index = cbCollections.getSelectedOption();
				prevName = uciList[index].label;
			}

			validCollection = false;
			cbCollections.clear();
			cbCollections.addOption(L"Updating...");
			cbCollections.selectOption(0);
			uciList.clear();
			UserCollection::loadCollectionList(uciList, edUsername.textW(), m_settings.getData().apiKey);
			cbCollections.clear();
			if (uciList.empty())
			{
				cbCollections.addOption(L"Empty");
				cbCollections.selectOption(0);
				validCollection = false;
				return 0;
			}
			for (const auto& item : uciList)
				cbCollections.addOption(item.label);

			int index = 0;
			if (!prevName.empty())
			{
				index = cbCollections.findOption(prevName);
				if (index == CB_ERR)
					index = 0;
			}
			cbCollections.selectOption(index);
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

			const int index = cbCollections.getSelectedOption();
			const auto& uci = uciList[index];
			m_userCollection.settings.collectionName = uci.label;
			m_userCollection.settings.collectionID = std::to_wstring(uci.id);

			m_userCollection.settings.username = edUsername.textW();
			m_userCollection.settings.categoriesAndPurity = purCom.getPurity();
			m_userCollection.update();
			m_isOk = true;
			DestroyWindow(hWnd());
			return 0;
		}
		
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
		break;
	}
	}
	
	return RESULT_DEFAULT;
}
