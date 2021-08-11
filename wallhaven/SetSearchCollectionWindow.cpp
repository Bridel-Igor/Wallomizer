#include "SetSearchCollectionWindow.h"
#include "CollectionManager.h"
#include "MainWindow.h"

SetSearchCollectionWindow* SetSearchCollectionWindow::setSearchCollectionWindow = nullptr;
SearchCollection* SetSearchCollectionWindow::currentSearchCollection = nullptr;

LRESULT SetSearchCollectionWindow::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		EnableWindow(MainWindow::mainWindow->Window(), FALSE);

		stCategory = new Static(Window(), "Category:",	10,		10,		60,		20);
		catCom = new CategoryComponent(Window(),		80,		10,		150,	20);

		stPurity = new Static(Window(), "Purity:",		240,	10,		60,		20);
		purCom = new PurityComponent(Window(),			310,	10,		150,	20);

		stTag = new Static(Window(), "Tags:",			10,		40,		60,		20);
		edTag = new Edit(Window(), "",					80,		40,		380,	20);
		stTagInstruct[0] = new Static(Window(), "tagname - search fuzzily for a tag/keyword",				80,		70,		380,	15);
		stTagInstruct[1] = new Static(Window(), "- tagname - exclude a tag / keyword",						80,		85,		380,	15);
		stTagInstruct[2] = new Static(Window(), "+ tag1 + tag2 - must have tag1 and tag2",					80,		100,	380,	15);
		stTagInstruct[3] = new Static(Window(), "+ tag1 - tag2 - must have tag1 and NOT tag2",				80,		115,	380,	15);
		stTagInstruct[4] = new Static(Window(), "@username - user uploads",									80,		130,	380,	15);
		stTagInstruct[5] = new Static(Window(), "id : 123 - Exact tag search(can not be combined)",			80,		145,	380,	15);
		stTagInstruct[6] = new Static(Window(), "type : {png / jpg} - Search for file type(jpg = jpeg)",	80,		160,	380,	15);
		stTagInstruct[7] = new Static(Window(), "like : wallpaper ID - Find wallpapers with similar tags",	80,		175,	380,	15);

		btnCancel = new Button(Window(), "Cancel",		80,		200,	185,	20);
		btnOk = new Button(Window(), "Ok",				275,	200,	185,	20);

		if (currentSearchCollection)
		{
			catCom->setCategory(currentSearchCollection->settings->categoriesAndPurity);
			purCom->setPurity(currentSearchCollection->settings->categoriesAndPurity);
			edTag->setTextA(currentSearchCollection->settings->tag);
		}

		font = CreateFont(15, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, FF_DONTCARE, "Arial");
		EnumChildWindows(Window(), SetChildFont, (LPARAM)font);
		bkBrush = CreateSolidBrush(RGB(26, 26, 26));
	}
	return 0;

	case WM_DESTROY:
	{
		currentSearchCollection = nullptr;
		for (int i = 0; i < 8; i++)
			delete stTagInstruct[i];
		delete stCategory, stPurity, stTag;
		delete edTag;
		delete	btnOk, btnCancel;
		delete catCom;
		delete purCom;
		DeleteObject(font);
		DeleteObject(bkBrush);
		EnableWindow(MainWindow::mainWindow->Window(), TRUE);
		SetForegroundWindow(MainWindow::mainWindow->Window());
		PostQuitMessage(0);
	}
	return 0;

	case WM_CLOSE:
	{
		DestroyWindow(hWnd);
		return 0;
	}
	return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(m_hWnd, &ps);
		FillRect(hdc, &ps.rcPaint, bkBrush);
		EndPaint(m_hWnd, &ps);
	}
	return 0;

	case WM_COMMAND:
	{
		if COMMANDEVENT(btnOk)
		{
			currentSearchCollection->settings->categoriesAndPurity = 0;
			currentSearchCollection->settings->categoriesAndPurity = catCom->getCategory() | purCom->getPurity();
			edTag->getTextA(currentSearchCollection->settings->tag, 255);
			if (currentSearchCollection->isValid == false)
				currentSearchCollection->isValid = true;
			else
				CollectionManager::reloadSettings();
			DestroyWindow(hWnd);
			return 0;
		}

		if COMMANDEVENT(btnCancel)
		{
			DestroyWindow(hWnd);
			return 0;
		}
	}
	return 0;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor(hdcStatic, RGB(129, 193, 193));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)bkBrush;
	}
	return 0;

	case WM_CTLCOLOREDIT:
	{
		HDC hdc = (HDC)wParam;
		SetTextColor(hdc, RGB(0, 0, 0));
		SetBkColor(hdc, RGB(200, 200, 200));
		SetDCBrushColor(hdc, RGB(200, 200, 200));
		return (LRESULT)GetStockObject(DC_BRUSH);
	}
	return 0;

	case WM_CTLCOLORBTN:
	{
		return (LRESULT)GetSysColorBrush(COLOR_WINDOW + 1);
	}
	return 0;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
	}
	return TRUE;
}

void SetSearchCollectionWindow::windowThread(SearchCollection* collection)
{
	if (setSearchCollectionWindow)
	{
		SetForegroundWindow(setSearchCollectionWindow->Window());
		return;
	}
	currentSearchCollection = collection;
	setSearchCollectionWindow = new SetSearchCollectionWindow;
	setSearchCollectionWindow->Create("Search collection", WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 470, 230, NULL, NULL);
	setSearchCollectionWindow->centerWindow(MainWindow::mainWindow->Window());
	ShowWindow(setSearchCollectionWindow->Window(), SW_SHOWNORMAL);
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ShowWindow(setSearchCollectionWindow->Window(), SW_HIDE);
	setSearchCollectionWindow->Destroy();
	delete setSearchCollectionWindow;
	setSearchCollectionWindow = nullptr;
}