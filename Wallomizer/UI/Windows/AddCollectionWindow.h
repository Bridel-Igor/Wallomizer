#pragma once

#include "IWindow.h"
#include "Button.h"

class Settings;
class CollectionManager;

class AddCollectionWindow : public IWindow
{
public:
	AddCollectionWindow(HWND hCaller, const Settings& settings, CollectionManager& collectionManager);
	~AddCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	const Settings& m_settings;
	CollectionManager& m_collectionManager;

	HWND m_hCaller;
	Button btnAddLocalCollection, btnAddUserCollection, btnAddSearchCollection, btnCancel;
};
