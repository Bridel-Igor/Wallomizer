#pragma once

#include "UserCollection.h"
#include "IWindow.h"
#include "Static.h"
#include "Edit.h"
#include "ComboBox.h"
#include "Button.h"
#include "CategoriesAndPurityComponents.h"

class Settings;

class SetUserCollectionWindow : public IWindow
{
public:
	SetUserCollectionWindow(IWindow* pOwner, const Settings& settings, UserCollection& userCollection);

	bool isOk() const noexcept { return m_isOk; }

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	static constexpr int width = 350;
	static constexpr int height = 130;

	const Settings& m_settings;
	UserCollection& m_userCollection;

	Static stUsername, stCollectionID, stPurity;
	Edit edUsername;
	ComboBox cbCollections;
	Button btnOk, btnCancel;
	PurityComponent purCom;

	std::vector<UserCollection::UserCollectionInfo> uciList;
	bool validCollection = false;
	bool m_isOk = false;
};
