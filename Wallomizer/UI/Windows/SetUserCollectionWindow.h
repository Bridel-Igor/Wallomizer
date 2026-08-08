#pragma once

#include "UserCollection.h"
#include "IWindow.h"
#include "Edit.h"
#include "Static.h"
#include "Button.h"
#include "ComboBox.h"

class Settings;

class SetUserCollectionWindow : public IWindow
{
public:
	SetUserCollectionWindow(HWND hCaller, const Settings& settings, UserCollection& userCollection);
	~SetUserCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	static constexpr int width = 350;
	static constexpr int height = 130;

private:
	const Settings& m_settings;
	UserCollection& m_userCollection;

	HWND m_hCaller;
	Static stUsername, stCollectionID, stPurity;
	Edit edUsername;
	ComboBox cbCollections;
	Button btnOk, btnCancel;
	PurityComponent purCom;

	std::list<UserCollection::UserCollectionInfo> uciList;
	bool validCollection = false;
};
