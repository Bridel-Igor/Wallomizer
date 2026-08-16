#pragma once

#include "IWindow.h"
#include "Button.h"

class WinUtils;
class Settings;
class CollectionManager;

class AddCollectionWindow : public IWindow
{
public:
	AddCollectionWindow(IWindow* pOwner, const WinUtils& winUtils, const Settings& settings, CollectionManager& collectionManager);

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	const WinUtils& m_winUtils;
	const Settings& m_settings;
	CollectionManager& m_collectionManager;

	Button btnAddLocalCollection, btnAddUserCollection, btnAddSearchCollection, btnCancel;
};
