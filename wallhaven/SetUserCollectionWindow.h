#pragma once

#include "UserCollection.h"
#include "BaseWindow.h"
#include "Edit.h"
#include "Static.h"
#include "Button.h"
#include "CheckBox.h"

class SetUserCollectionWindow : public BaseWindow<SetUserCollectionWindow>
{
	Static	*stUsername, *stCollectionID, *stApiKey, *stIsApiKeyUsed;
	Edit	*edUsername, *edCollectionID, *edApiKey;
	Button *btnOk, *btnCancel;
	CheckBox *cbIsApiKeyUsed;
	HFONT font;
	HBRUSH bkBrush;
	static SetUserCollectionWindow *setUserCollectionWindow;
	static UserCollection* currentUserCollection;

public:
	LPCSTR ClassName() const { return "Set User Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(UserCollection *collection);
};