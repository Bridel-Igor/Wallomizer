#pragma once

#include "SearchCollection.h"
#include "CollectionManager.h"
#include "IWindow.h"
#include "Button.h"
#include "Static.h"
#include "Edit.h"

class SetSearchCollectionWindow : public IWindow
{
public:
	SetSearchCollectionWindow(SearchCollection* pCollection, CollectionManager* pCollectionManager);
	~SetSearchCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CollectionManager* m_pCollectionManager;
	SearchCollection* m_pCurrentSearchCollection;
	Static stCategory, stPurity, stTag, stFilt, stTagInstruct[8];
	Edit edTag;
	Button btnOk, btnCancel, btnRes, btnAR, btnColor;
	CategoryComponent catCom;
	PurityComponent purCom;

	char tmpRes[255] = "", tmpAR[128] = "", tmpColor[16] = "";
};