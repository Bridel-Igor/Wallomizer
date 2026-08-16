#include "SetSearchCollectionWindow.h"

#include "SearchCollection.h"
#include "ResPickerWindow.h"
#include "AspRatPickerWindow.h"
#include "ColorPickerWindow.h"

SetSearchCollectionWindow::SetSearchCollectionWindow(IWindow* pOwner, SearchCollection& searchCollection) :
	IWindow(pOwner, "Search collection", "Set Search Collection Window Class",WS_CAPTION | WS_SYSMENU, 0, 100, 100, 470, 260),
	m_searchCollection(searchCollection),
	stCategory		(this, "Category:",													10,		10,		60,		20),
	catCom			(this,																80,		10,		159,	20),

	stPurity		(this, "Purity:",													254,	10,		40,		20),
	purCom			(this,																301,	10,		159,	20),

	stFilt			(this, "Filters:",													10,		40,		60,		20),
	btnRes			(this, "Pick resolution",											80,		40,		120,	20),
	btnAR			(this, "Pick ratio",												210,	40,		120,	20),
	btnColor		(this, "Pick color",												340,	40,		120,	20),

	stTag			(this, "Tags:",														10,		70,		60,		20),
	edTag			(this, m_searchCollection.settings.tag,								80,		70,		380,	20),
	stTagInstruct { {this, "tagname - search fuzzily for a tag/keyword",				80,		100,	380,	15},
					{this, "- tagname - exclude a tag / keyword",						80,		115,	380,	15},
					{this, "+ tag1 + tag2 - must have tag1 and tag2",					80,		130,	380,	15},
					{this, "+ tag1 - tag2 - must have tag1 and NOT tag2",				80,		145,	380,	15},
					{this, "@username - user uploads",									80,		160,	380,	15},
					{this, "id : 123 - Exact tag search(can not be combined)",			80,		175,	380,	15},
					{this, "type : {png / jpg} - Search for file type(jpg = jpeg)",		80,		190,	380,	15},
					{this, "like : wallpaper ID - Find wallpapers with similar tags",	80,		205,	380,	15} },

	btnCancel		(this, "Cancel",													80,		230,	185,	20),
	btnOk			(this, "Ok",														275,	230,	185,	20),
	tmpRes(m_searchCollection.settings.resolution),
	tmpAR(m_searchCollection.settings.ratio),
	tmpColor(m_searchCollection.settings.color)
{
	catCom.setCategory(m_searchCollection.settings.categoriesAndPurity);
	purCom.setPurity(m_searchCollection.settings.categoriesAndPurity);
}

LRESULT SetSearchCollectionWindow::HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT drawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
		if (purCom.draw(drawItem))
			return TRUE;
		if (catCom.draw(drawItem))
			return TRUE;
		break;
	}

	case WM_COMMAND:
	{
		if (purCom.click(wParam))
			return 0;
		if (catCom.click(wParam))
			return 0;
		if (btnRes.isClicked(wParam))
		{
			ResPickerWindow resPickerWindow(this, tmpRes);
			resPickerWindow.windowLoop();
			return 0;
		}
		if (btnAR.isClicked(wParam))
		{
			AspRatPickerWindow aspRatPickerWindow(this, tmpAR);
			aspRatPickerWindow.windowLoop();
			return 0;
		}
		if (btnColor.isClicked(wParam))
		{
			ColorPickerWindow colorPickerWindow(this, tmpColor);
			colorPickerWindow.windowLoop();
			return 0;
		}
		if (btnOk.isClicked(wParam))
		{
			m_searchCollection.settings.categoriesAndPurity = catCom.getCategory() | purCom.getPurity();
			m_searchCollection.settings.tag = edTag.textW();
			m_searchCollection.settings.resolution = tmpRes;
			m_searchCollection.settings.ratio = tmpAR;
			m_searchCollection.settings.color = tmpColor;
			m_searchCollection.update();
			m_isOk = true;
			DestroyWindow(hWnd());
			return 0;
		}
		if (btnCancel.isClicked(wParam))
		{
			DestroyWindow(hWnd());
			return 0;
		}
		break;
	}
	}

	return RESULT_DEFAULT;
}
