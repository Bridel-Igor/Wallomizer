#pragma once

#include "IWindow.h"
#include "Button.h"
#include "CollectionManager.h"

class AddCollectionWindow : public IWindow
{
public:
	AddCollectionWindow(CollectionManager* pCollectionManager);
	~AddCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CollectionManager* m_pCollectionManager;
	Button btnAddLocalCollection, btnAddUserCollection, btnAddSearchCollection, btnCancel;
};