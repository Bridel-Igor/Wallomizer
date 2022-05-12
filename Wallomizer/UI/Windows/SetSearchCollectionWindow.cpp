#include "SetSearchCollectionWindow.h"
#include "ResPickerWindow.h"
#include "AspRatPickerWindow.h"
#include "ColorPickerWindow.h"

SetSearchCollectionWindow::SetSearchCollectionWindow(HWND hCaller, CollectionManager* pCollectionManager, SearchCollection* pCollection) :
	IWindow("Search collection", "Set Search Collection Window Class",WS_CAPTION | WS_SYSMENU, NULL, 100, 100, 470, 260),
	m_hCaller(hCaller),
	m_pCollectionManager(pCollectionManager),
	m_pCurrentSearchCollection(pCollection),
	stCategory		(this, "Category:",												10,		10,		60,		20),
	catCom			(this,																80,		10,		159,	20),

	stPurity		(this, "Purity:",													254,	10,		40,		20),
	purCom			(this,																301,	10,		159,	20),

	stFilt			(this, "Filters:",												10,		40,		60,		20),
	btnRes			(this, "Pick resolution",											80,		40,		120,	20),
	btnAR			(this, "Pick ratio",												210,	40,		120,	20),
	btnColor		(this, "Pick color",												340,	40,		120,	20),

	stTag			(this, "Tags:",													10,		70,		60,		20),
	edTag			(this, L"",														80,		70,		380,	20),
	stTagInstruct { {this, "tagname - search fuzzily for a tag/keyword",				80,		100,	380,	15},
					{this, "- tagname - exclude a tag / keyword",						80,		115,	380,	15},
					{this, "+ tag1 + tag2 - must have tag1 and tag2",					80,		130,	380,	15},
					{this, "+ tag1 - tag2 - must have tag1 and NOT tag2",				80,		145,	380,	15},
					{this, "@username - user uploads",								80,		160,	380,	15},
					{this, "id : 123 - Exact tag search(can not be combined)",		80,		175,	380,	15},
					{this, "type : {png / jpg} - Search for file type(jpg = jpeg)",	80,		190,	380,	15},
					{this, "like : wallpaper ID - Find wallpapers with similar tags",	80,		205,	380,	15} },

	btnCancel		(this, "Cancel",													80,		230,	185,	20),
	btnOk			(this, "Ok",														275,	230,	185,	20)
{
	if (m_pCurrentSearchCollection)
	{
		catCom.setCategory(m_pCurrentSearchCollection->settings.categoriesAndPurity);
		purCom.setPurity(m_pCurrentSearchCollection->settings.categoriesAndPurity);
		edTag.setTextW(m_pCurrentSearchCollection->settings.wsTag);
		wcscpy_s(tmpRes, m_pCurrentSearchCollection->settings.wsResolution);
		wcscpy_s(tmpAR, m_pCurrentSearchCollection->settings.wsRatio);
		wcscpy_s(tmpColor, m_pCurrentSearchCollection->settings.wsColor);
	}

	EnumChildWindows(hWnd(), SetChildFont, (LPARAM)resources.mainFont);
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
			edTag.getTextW(m_pCurrentSearchCollection->settings.wsTag, 255);
			wcscpy_s(m_pCurrentSearchCollection->settings.wsResolution, tmpRes);
			wcscpy_s(m_pCurrentSearchCollection->settings.wsRatio, tmpAR);
			wcscpy_s(m_pCurrentSearchCollection->settings.wsColor, tmpColor);
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

	default:
		return RESULT_DEFAULT;
	}
}