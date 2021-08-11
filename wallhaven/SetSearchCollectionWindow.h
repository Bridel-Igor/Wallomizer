#pragma once

#include "SearchCollection.h"
#include "BaseWindow.h"
#include "Button.h"
#include "Static.h"
#include "Edit.h"

class SetSearchCollectionWindow : public BaseWindow<SetSearchCollectionWindow>
{
	Static* stCategory, * stPurity, *stTag, *stTagInstruct[8];
	Edit* edTag;
	Button* btnOk, * btnCancel;
	CategoryComponent* catCom;
	PurityComponent* purCom;
	HFONT font;
	HBRUSH bkBrush;
	static SetSearchCollectionWindow* setSearchCollectionWindow;
	static SearchCollection* currentSearchCollection;

public:
	LPCSTR ClassName() const { return "Set Search Collection Window Class"; }
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static void windowThread(SearchCollection* collection);
};