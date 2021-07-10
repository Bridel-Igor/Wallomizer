#pragma once

#include "BaseWindow.h"
#include "Edit.h"
#include "Static.h"
#include "Button.h"
#include "CheckBox.h"

class SetUserCollectionWindow : public BaseWindow<SetUserCollectionWindow>
{
	Static	*stDelay, *stUsername, *stCollectionID, *stApiKey, *stIsApiKeyUsed;
	Edit	*edDelay, *edUsername, *edCollectionID, *edApiKey;
	Button *btnOk, *btnCancel;
	CheckBox *cbIsApiKeyUsed;
	HFONT font;
	HBRUSH bkBrush;

public:
	LPCSTR ClassName() const { return "Sample Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};