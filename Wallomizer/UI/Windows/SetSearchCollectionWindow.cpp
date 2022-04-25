#include "SetSearchCollectionWindow.h"
#include "MainWindow.h"
#include "ResPickerWindow.h"
#include "AspRatPickerWindow.h"
#include "ColorPickerWindow.h"

SetSearchCollectionWindow* SetSearchCollectionWindow::setSearchCollectionWindow = nullptr;
SearchCollection* SetSearchCollectionWindow::currentSearchCollection = nullptr;

LRESULT SetSearchCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->hWnd(), FALSE);

		stCategory = new Static(this->hWnd(), "Category:",	10,		10,		60,		20);
		catCom = new CategoryComponent(this->hWnd(),		80,		10,		159,	20);

		stPurity = new Static(this->hWnd(), "Purity:",		254,	10,		40,		20);
		purCom = new PurityComponent(this->hWnd(),			301,	10,		159,	20);

		stFilt = new Static(this->hWnd(), "Filters:",		10,		40,		60,		20);
		btnRes = new Button(this->hWnd(), "Pick resolution",80,		40,		120,	20);
		btnAR = new Button(this->hWnd(), "Pick ratio",		210,	40,		120,	20);
		btnColor = new Button(this->hWnd(), "Pick color",	340,	40,		120,	20);

		stTag = new Static(this->hWnd(), "Tags:",			10,		70,		60,		20);
		edTag = new Edit(this->hWnd(), "",					80,		70,		380,	20);
		stTagInstruct[0] = new Static(this->hWnd(), "tagname - search fuzzily for a tag/keyword",				80,		100,	380,	15);
		stTagInstruct[1] = new Static(this->hWnd(), "- tagname - exclude a tag / keyword",						80,		115,	380,	15);
		stTagInstruct[2] = new Static(this->hWnd(), "+ tag1 + tag2 - must have tag1 and tag2",					80,		130,	380,	15);
		stTagInstruct[3] = new Static(this->hWnd(), "+ tag1 - tag2 - must have tag1 and NOT tag2",				80,		145,	380,	15);
		stTagInstruct[4] = new Static(this->hWnd(), "@username - user uploads",									80,		160,	380,	15);
		stTagInstruct[5] = new Static(this->hWnd(), "id : 123 - Exact tag search(can not be combined)",			80,		175,	380,	15);
		stTagInstruct[6] = new Static(this->hWnd(), "type : {png / jpg} - Search for file type(jpg = jpeg)",	80,		190,	380,	15);
		stTagInstruct[7] = new Static(this->hWnd(), "like : wallpaper ID - Find wallpapers with similar tags",	80,		205,	380,	15);

		btnCancel = new Button(this->hWnd(), "Cancel",		80,		230,	185,	20);
		btnOk = new Button(this->hWnd(), "Ok",				275,	230,	185,	20);

		if (currentSearchCollection)
		{
			catCom->setCategory(currentSearchCollection->settings->categoriesAndPurity);
			purCom->setPurity(currentSearchCollection->settings->categoriesAndPurity);
			edTag->setTextA(currentSearchCollection->settings->tag);
			strcpy_s(tmpRes, currentSearchCollection->settings->resolution);
			strcpy_s(tmpAR, currentSearchCollection->settings->ratio);
			strcpy_s(tmpColor, currentSearchCollection->settings->color);
		}

		EnumChildWindows(this->hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	}
	return 0;

	case WM_DESTROY:
	{
		currentSearchCollection = nullptr;
		for (int i = 0; i < 8; i++)
			delete stTagInstruct[i];

		delete stCategory;
		delete stPurity;
		delete stTag;
		delete stFilt;

		delete edTag;

		delete btnOk;
		delete btnCancel;
		delete btnRes;
		delete btnAR;
		delete btnColor;

		delete catCom;
		delete purCom;
		EnableWindow(MainWindow::mainWindow->hWnd(), TRUE);
		SetForegroundWindow(MainWindow::mainWindow->hWnd());
		PostQuitMessage(0);
	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	return 0;

	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (purCom->draw(pDIS))
			return TRUE;
		if (catCom->draw(pDIS))
			return TRUE;
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, WindowStyles::mainBkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		if (purCom->click(wParam))
			return 0;
		if (catCom->click(wParam))
			return 0;
		if (btnRes->isClicked(wParam))
		{
			ResPickerWindow::windowThread(tmpRes, this->hWnd());
			return 0;
		}
		if (btnAR->isClicked(wParam))
		{
			AspRatPickerWindow::windowThread(tmpAR, this->hWnd());
			return 0;
		}
		if (btnColor->isClicked(wParam))
		{
			ColorPickerWindow::windowThread(tmpColor, this->hWnd());
			return 0;
		}
		if (btnOk->isClicked(wParam))
		{
			currentSearchCollection->settings->categoriesAndPurity = 0;
			currentSearchCollection->settings->categoriesAndPurity = catCom->getCategory() | purCom->getPurity();
			edTag->getTextA(currentSearchCollection->settings->tag, 255);
			strcpy_s(currentSearchCollection->settings->resolution, tmpRes);
			strcpy_s(currentSearchCollection->settings->ratio, tmpAR);
			strcpy_s(currentSearchCollection->settings->color, tmpColor);
			if (currentSearchCollection->isValid == false)
				currentSearchCollection->isValid = true;
			else
				collectionManager->reloadSettings();
			DestroyWindow(hWnd);
			return 0;
		}
		if (btnCancel->isClicked(wParam))
		{
			DestroyWindow(hWnd);
			return 0;
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		catCom->mouseHovering(wParam);
		purCom->mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void SetSearchCollectionWindow::windowThread(SearchCollection* collection, CollectionManager* _collectionManager)
{
	if (setSearchCollectionWindow)
	{
		SetForegroundWindow(setSearchCollectionWindow->hWnd());
		return;
	}
	currentSearchCollection = collection;
	setSearchCollectionWindow = new SetSearchCollectionWindow;
	setSearchCollectionWindow->collectionManager = _collectionManager;
	setSearchCollectionWindow->Create("Search collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 470, 260, NULL, NULL);
	setSearchCollectionWindow->centerWindow(MainWindow::mainWindow->hWnd());
	ShowWindow(setSearchCollectionWindow->hWnd(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setSearchCollectionWindow->hWnd(), SW_HIDE);
	setSearchCollectionWindow->Destroy();
	delete setSearchCollectionWindow;
	setSearchCollectionWindow = nullptr;
}