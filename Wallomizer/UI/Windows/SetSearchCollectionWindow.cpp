#include "SetSearchCollectionWindow.h"
#include "ResPickerWindow.h"
#include "AspRatPickerWindow.h"
#include "ColorPickerWindow.h"

SetSearchCollectionWindow::SetSearchCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager, SearchCollection* pCollection) :
	IWindow("Search collection", "Set Search Collection Window Class",WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 470, 260),
	m_hCaller(hCaller),
	m_pCollectionManager(pCollectionManager),
	m_pCurrentSearchCollection(pCollection),
	stCategory		(hWnd(), "Category:",												10,		10,		60,		20),
	catCom			(hWnd(),															80,		10,		159,	20),

	stPurity		(hWnd(), "Purity:",													254,	10,		40,		20),
	purCom			(hWnd(),															301,	10,		159,	20),

	stFilt			(hWnd(), "Filters:",												10,		40,		60,		20),
	btnRes			(hWnd(), "Pick resolution",											80,		40,		120,	20),
	btnAR			(hWnd(), "Pick ratio",												210,	40,		120,	20),
	btnColor		(hWnd(), "Pick color",												340,	40,		120,	20),

	stTag			(hWnd(), "Tags:",													10,		70,		60,		20),
	edTag			(hWnd(), "",														80,		70,		380,	20),
	stTagInstruct { {hWnd(), "tagname - search fuzzily for a tag/keyword",				80,		100,	380,	15},
					{hWnd(), "- tagname - exclude a tag / keyword",						80,		115,	380,	15},
					{hWnd(), "+ tag1 + tag2 - must have tag1 and tag2",					80,		130,	380,	15},
					{hWnd(), "+ tag1 - tag2 - must have tag1 and NOT tag2",				80,		145,	380,	15},
					{hWnd(), "@username - user uploads",								80,		160,	380,	15},
					{hWnd(), "id : 123 - Exact tag search(can not be combined)",		80,		175,	380,	15},
					{hWnd(), "type : {png / jpg} - Search for file type(jpg = jpeg)",	80,		190,	380,	15},
					{hWnd(), "like : wallpaper ID - Find wallpapers with similar tags",	80,		205,	380,	15} },

	btnCancel		(hWnd(), "Cancel",													80,		230,	185,	20),
	btnOk			(hWnd(), "Ok",														275,	230,	185,	20)
{
	if (m_pCurrentSearchCollection)
	{
		catCom.setCategory(m_pCurrentSearchCollection->settings.categoriesAndPurity);
		purCom.setPurity(m_pCurrentSearchCollection->settings.categoriesAndPurity);
		edTag.setTextA(m_pCurrentSearchCollection->settings.sTag);
		strcpy_s(tmpRes, m_pCurrentSearchCollection->settings.sResolution);
		strcpy_s(tmpAR, m_pCurrentSearchCollection->settings.sRatio);
		strcpy_s(tmpColor, m_pCurrentSearchCollection->settings.sColor);
	}

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)WindowStyles::mainFont);
	centerWindow(m_hCaller);
	EnableWindow(m_hCaller, FALSE);
	SetForegroundWindow(m_hCaller);
	ShowWindow(hWnd(), SW_SHOWNORMAL);
	SetForegroundWindow(hWnd());
}

SetSearchCollectionWindow::~SetSearchCollectionWindow()
{
	ShowWindow(hWnd(), SW_HIDE);
	EnableWindow(m_hCaller, TRUE);
	SetForegroundWindow(m_hCaller);
}

LRESULT SetSearchCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT pDIS = (LPDRAWITEMSTRUCT)lParam;
		if (purCom.draw(pDIS))
			return TRUE;
		if (catCom.draw(pDIS))
			return TRUE;
	}
	return 0;

	case WM_COMMAND:
	{
		if (purCom.click(wParam))
			return 0;
		if (catCom.click(wParam))
			return 0;
		if (btnRes.isClicked(wParam))
		{
			ResPickerWindow resPickerWindow(hWnd(), tmpRes);
			resPickerWindow.windowLoop();
			SetForegroundWindow(m_hCaller);
			SetForegroundWindow(hWnd());
			return 0;
		}
		if (btnAR.isClicked(wParam))
		{
			AspRatPickerWindow aspRatPickerWindow(hWnd(), tmpAR);
			aspRatPickerWindow.windowLoop();
			SetForegroundWindow(m_hCaller);
			SetForegroundWindow(hWnd());
			return 0;
		}
		if (btnColor.isClicked(wParam))
		{
			ColorPickerWindow colorPickerWindow(hWnd(), tmpColor);
			colorPickerWindow.windowLoop();
			SetForegroundWindow(m_hCaller);
			SetForegroundWindow(hWnd());
			return 0;
		}
		if (btnOk.isClicked(wParam))
		{
			m_pCurrentSearchCollection->settings.categoriesAndPurity = 0;
			m_pCurrentSearchCollection->settings.categoriesAndPurity = catCom.getCategory() | purCom.getPurity();
			edTag.getTextA(m_pCurrentSearchCollection->settings.sTag, 255);
			strcpy_s(m_pCurrentSearchCollection->settings.sResolution, tmpRes);
			strcpy_s(m_pCurrentSearchCollection->settings.sRatio, tmpAR);
			strcpy_s(m_pCurrentSearchCollection->settings.sColor, tmpColor);
			if (m_pCurrentSearchCollection->isValid() == false)
				m_pCurrentSearchCollection->setValid(true);
			else
				m_pCollectionManager->reloadSettings();
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
	}
	return 0;

	case WM_SETCURSOR:
	{
		catCom.mouseHovering(wParam);
		purCom.mouseHovering(wParam);
		// Fallthrough. DefWindowProc must be reached anyway.
	}

	default:
		return RESULT_DEFAULT;
	}
}