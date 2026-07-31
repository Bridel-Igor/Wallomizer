#pragma once

#include "IWindow.h"
#include "Button.h"
#include "CollectionManager.h"

class AddCollectionWindow : public IWindow
{
public:
	AddCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager);
	~AddCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	CollectionManager* m_pCollectionManager; //TODO: change to reference
	Button btnAddLocalCollection, btnAddUserCollection, btnAddSearchCollection, btnCancel;
};