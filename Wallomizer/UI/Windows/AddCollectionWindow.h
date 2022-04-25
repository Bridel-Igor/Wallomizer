#pragma once

#include "BaseWindow.h"
#include "Button.h"
#include "CollectionManager.h"

class AddCollectionWindow : public BaseWindow<AddCollectionWindow>
{
	Button *btnAddUserCollection, *btnAddLocalCollection, *btnAddSearchCollection, *btnCancel;
	static AddCollectionWindow* addCollectionWindow;
	CollectionManager* collectionManager = nullptr;

public:
	LPCSTR ClassName() const { return "Add Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(CollectionManager* collectionManager);
};