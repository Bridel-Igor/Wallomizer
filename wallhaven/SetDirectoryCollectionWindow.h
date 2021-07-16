#pragma once

#include "DirectoryCollection.h"
#include "BaseWindow.h"
#include "Static.h"
#include "Button.h"
#include "Edit.h"

class SetDirectoryCollectionWindow : public BaseWindow<SetDirectoryCollectionWindow>
{
	Static *stPath;
	Button *btnOk, *btnCancel, *btnPath;
	Edit *edPath;
	HFONT font;
	HBRUSH bkBrush;
	static SetDirectoryCollectionWindow* setDirectoryCollectionWindow;
	static DirectoryCollection *currentDirectoryCollection;

public:
	LPCSTR ClassName() const { return "Set Directory Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(DirectoryCollection *collection);
};