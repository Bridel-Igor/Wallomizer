#pragma once

#include "UserCollection.h"
#include "CollectionManager.h"
#include "BaseWindow.h"
#include "Edit.h"
#include "Static.h"
#include "Button.h"
#include "ComboBox.h"

class SetUserCollectionWindow : public BaseWindow<SetUserCollectionWindow>
{
	Static	*stUsername, *stCollectionID, *stPurity;
	Edit	*edUsername;
	ComboBox* cbCollections;
	Button* btnOk, * btnCancel;
	PurityComponent* purCom;
	static SetUserCollectionWindow *setUserCollectionWindow;
	static UserCollection* currentUserCollection;
	std::vector<UserCollection::UserCollectionInfo> list;
	bool validCollection = false;
	CollectionManager* collectionManager = nullptr;

public:
	LPCSTR ClassName() const { return "Set User Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(UserCollection *collection, CollectionManager* collectionManager);

	static constexpr int width = 350;
	static constexpr int height = 130;
};