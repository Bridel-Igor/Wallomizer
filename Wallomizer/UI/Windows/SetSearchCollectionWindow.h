#pragma once

#include "SearchCollection.h"
#include "IWindow.h"
#include "Button.h"
#include "Static.h"
#include "Edit.h"

class SetSearchCollectionWindow : public IWindow
{
public:
	SetSearchCollectionWindow(HWND hCaller, SearchCollection& collection);
	~SetSearchCollectionWindow();
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	HWND m_hCaller;
	SearchCollection& m_searchCollection;
	Static stCategory, stPurity, stTag, stFilt, stTagInstruct[8];
	Edit edTag;
	Button btnOk, btnCancel, btnRes, btnAR, btnColor;
	CategoryComponent catCom;
	PurityComponent purCom;

	wchar_t tmpRes[255] = L"", tmpAR[128] = L"", tmpColor[16] = L"";
};