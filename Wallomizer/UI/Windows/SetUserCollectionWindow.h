#pragma once

#include "UserCollection.h"
#include "IWindow.h"
#include "Edit.h"
#include "Static.h"
#include "Button.h"
#include "ComboBox.h"

class App;

class SetUserCollectionWindow : public IWindow
{
public:
	SetUserCollectionWindow(HWND hCaller, App& app, UserCollection& userCollection);
	~SetUserCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int width = 350;
	static constexpr int height = 130;

private:
	HWND m_hCaller;
	App& m_app;
	UserCollection& m_userCollection;
	Static stUsername, stCollectionID, stPurity;
	Edit edUsername;
	ComboBox cbCollections;
	Button btnOk, btnCancel;
	PurityComponent purCom;

	std::list<UserCollection::UserCollectionInfo> uciList;
	bool validCollection = false;
};