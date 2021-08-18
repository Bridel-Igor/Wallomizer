#pragma once

#include "LocalCollection.h"
#include "BaseWindow.h"
#include "Static.h"
#include "Button.h"
#include "Edit.h"

class SetLocalCollectionWindow : public BaseWindow<SetLocalCollectionWindow>
{
	Static *stPath;
	Button *btnOk, *btnCancel, *btnPath;
	Edit *edPath;
	static SetLocalCollectionWindow* setLocalCollectionWindow;
	static LocalCollection *currentLocalCollection;

public:
	LPCSTR ClassName() const { return "Set Local Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(LocalCollection *collection);
};