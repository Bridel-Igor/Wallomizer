#include "AddCollectionWindow.h"

#include <memory>

#include "Settings.h"
#include "CollectionManager.h"
#include "UserCollection.h"
#include "LocalCollection.h"
#include "SearchCollection.h"
#include "SetLocalCollectionWindow.h"
#include "SetUserCollectionWindow.h"
#include "SetSearchCollectionWindow.h"

AddCollectionWindow::AddCollectionWindow(HWND hCaller, const Settings& settings, CollectionManager& collectionManager) :
	IWindow("Add collection", "Add Collection Window Class", WS_CAPTION | WS_SYSMENU, NULL, 100,	100,	240,	140),
	m_settings(settings),
	m_collectionManager(collectionManager),
	m_hCaller(hCaller),
	btnAddLocalCollection(this, "Add local collection",					10,		10,		220,	20),
	btnAddUserCollection(this, "Add wallhaven user collection",		10,		40,		220,	20),
	btnAddSearchCollection(this, "Add wallhaven search collection",	10,		70,		220,	20),
	btnCancel(this, "Cancel",											10,		110,	220,	20)
{
	centerWindow(m_hCaller);
	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
	EnableWindow(m_hCaller, FALSE);
}

AddCollectionWindow::~AddCollectionWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

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
			SetUserCollectionWindow setUserCollectionWindow(hWnd(), m_settings, *collection);
			setUserCollectionWindow.windowLoop();
			if (collection->isValid())
				m_collectionManager.addCollection(std::move(collection));
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnAddLocalCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			auto collection = std::make_unique<LocalCollection>(m_collectionManager);
			SetLocalCollectionWindow setLocalCollectionWindow(hWnd(), *collection);
			setLocalCollectionWindow.windowLoop();
			if (collection->isValid())
				m_collectionManager.addCollection(std::move(collection));
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnAddSearchCollection.isClicked(wParam))
		{
			ShowWindow(hWnd(), SW_HIDE);
			auto collection = std::make_unique <SearchCollection>(m_settings, m_collectionManager);
			SetSearchCollectionWindow setSearchCollectionWindow(hWnd(), *collection);
			setSearchCollectionWindow.windowLoop();
			if (collection->isValid())
				m_collectionManager.addCollection(std::move(collection));
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
