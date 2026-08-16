#include "AddCollectionWindow.h"

#include <memory>

#include "WinUtils.h"
#include "Settings.h"
#include "CollectionManager.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "SetLocalCollectionWindow.h"
#include "SetUserCollectionWindow.h"
#include "SetSearchCollectionWindow.h"

AddCollectionWindow::AddCollectionWindow(IWindow* pOwner, const WinUtils& winUtils, const Settings& settings, CollectionManager& collectionManager) :
	IWindow(pOwner, "Add collection", "Add Collection Window Class", WS_CAPTION | WS_SYSMENU, 0, 100,	100,	240,	140),
	m_winUtils(winUtils),
	m_settings(settings),
	m_collectionManager(collectionManager),
	btnAddLocalCollection(this, "Add local collection",				10,		10,		220,	20),
	btnAddUserCollection(this, "Add wallhaven user collection",		10,		40,		220,	20),
	btnAddSearchCollection(this, "Add wallhaven search collection",	10,		70,		220,	20),
	btnCancel(this, "Cancel",										10,		110,	220,	20)
{}

LRESULT AddCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM)
{
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (btnAddUserCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			auto collection = std::make_unique<UserCollection>(m_settings);
			SetUserCollectionWindow setUserCollectionWindow(this, m_settings, *collection);
			setUserCollectionWindow.windowLoop();
			if (setUserCollectionWindow.isOk() && collection->isValid())
				m_collectionManager.addCollection(std::move(collection));
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnAddLocalCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			auto collection = std::make_unique<LocalCollection>(m_collectionManager);
			SetLocalCollectionWindow setLocalCollectionWindow(this, m_winUtils, *collection);
			setLocalCollectionWindow.windowLoop();
			if (setLocalCollectionWindow.isOk() && collection->isValid())
				m_collectionManager.addCollection(std::move(collection));
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnAddSearchCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			auto collection = std::make_unique<SearchCollection>(m_settings, m_collectionManager);
			SetSearchCollectionWindow setSearchCollectionWindow(this, *collection);
			setSearchCollectionWindow.windowLoop();
			if (setSearchCollectionWindow.isOk() && collection->isValid())
				m_collectionManager.addCollection(std::move(collection));
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
