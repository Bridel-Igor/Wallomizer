#pragma once

#include "LocalCollection.h"
#include "IWindow.h"
#include "Static.h"
#include "Button.h"
#include "Edit.h"
#include "CheckBox.h"

class SetLocalCollectionWindow : public IWindow
{
public:
	SetLocalCollectionWindow(HWND hCaller, LocalCollection& localCollection);
	~SetLocalCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	LocalCollection& m_localCollection;

	HWND m_hCaller;
	Static stPath, stRecursive;
	Edit edPath;
	CheckBox cbRecursive;
	Button btnOk, btnCancel, btnPath;
};
