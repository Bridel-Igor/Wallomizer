#pragma once

#include "LocalCollection.h"
#include "IWindow.h"
#include "Static.h"
#include "Button.h"
#include "Edit.h"
#include "CheckBox.h"

class CollectionManager;

class SetLocalCollectionWindow : public IWindow
{
public:
	SetLocalCollectionWindow(HWND hCaller, CollectionManager& collectionManager, LocalCollection *pCollection);
	~SetLocalCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	CollectionManager& m_collectionManager;
	LocalCollection* m_pCurrentLocalCollection;
	Static stPath, stRecursive;
	Edit edPath;
	CheckBox cbRecursive;
	Button btnOk, btnCancel, btnPath;
};