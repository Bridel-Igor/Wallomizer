#pragma once

#include "SearchCollection.h"
#include "BaseWindow.h"
#include "Button.h"
#include "Static.h"
#include "Edit.h"

class SetSearchCollectionWindow : public BaseWindow<SetSearchCollectionWindow>
{
	Static* stCategory, * stPurity, *stTag, *stTagInstruct[8], *stRes, *stAR;
	Edit* edTag;
	Button* btnOk, * btnCancel, *btnRes, *btnAR;
	CategoryComponent* catCom;
	PurityComponent* purCom;
	HFONT font;
	HBRUSH bkBrush;
	char tmpRes[255], tmpAR[255];
	static SetSearchCollectionWindow* setSearchCollectionWindow;
	static SearchCollection* currentSearchCollection;

public:
	LPCSTR ClassName() const { return "Set Search Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(SearchCollection* collection);
};