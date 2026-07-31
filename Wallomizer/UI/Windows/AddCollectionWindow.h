#pragma once

#include "IWindow.h"
#include "Button.h"

class CollectionManager;

class AddCollectionWindow : public IWindow
{
public:
	AddCollectionWindow(HWND hCaller, CollectionManager& collectionManager);
	~AddCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	CollectionManager& m_collectionManager;
	Button btnAddLocalCollection, btnAddUserCollection, btnAddSearchCollection, btnCancel;
};