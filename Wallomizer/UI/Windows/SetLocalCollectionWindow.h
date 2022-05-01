#pragma once

#include "LocalCollection.h"
#include "IWindow.h"
#include "Static.h"
#include "Button.h"
#include "Edit.h"
#include "CollectionManager.h"

class SetLocalCollectionWindow : public IWindow
{
public:
	SetLocalCollectionWindow(LocalCollection *pCollection, CollectionManager* pCollectionManager);
	~SetLocalCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CollectionManager* m_pCollectionManager;
	LocalCollection* m_pCurrentLocalCollection;
	Static stPath;
	Edit edPath;
	Button btnOk, btnCancel, btnPath;
};