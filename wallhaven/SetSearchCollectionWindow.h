#pragma once

#include "SearchCollection.h"
#include "BaseWindow.h"
#include "Button.h"
#include "Static.h"
#include "Edit.h"

class SetSearchCollectionWindow : public BaseWindow<SetSearchCollectionWindow>
{
	Static* stCategory, * stPurity, *stTag, *stTagInstruct[8], *stFilt;
	Edit* edTag;
	Button* btnOk, * btnCancel, *btnRes, *btnAR, *btnColor;
	CategoryComponent* catCom;
	PurityComponent* purCom;
	char tmpRes[255], tmpAR[128], tmpColor[16];
	static SetSearchCollectionWindow* setSearchCollectionWindow;
	static SearchCollection* currentSearchCollection;

public:
	LPCSTR ClassName() const { return "Set Search Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(SearchCollection* collection);
};