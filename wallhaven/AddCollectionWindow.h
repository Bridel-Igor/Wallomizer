#pragma once

#include "BaseWindow.h"
#include "Button.h"

class AddCollectionWindow : public BaseWindow<AddCollectionWindow>
{
	Button *btnAddUserCollection, *btnAddDirectoryCollection, *btnAddSearchCollection, *btnCancel;
	static AddCollectionWindow* addCollectionWindow;
	HFONT font;
	HBRUSH bkBrush;

public:
	LPCSTR ClassName() const { return "Add Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread();
};