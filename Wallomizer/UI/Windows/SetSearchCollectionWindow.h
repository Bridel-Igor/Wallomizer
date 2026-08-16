#pragma once

#include <string>

#include "IWindow.h"
#include "Static.h"
#include "Edit.h"
#include "Button.h"
#include "CategoriesAndPurityComponents.h"

class SearchCollection;

class SetSearchCollectionWindow : public IWindow
{
public:
	SetSearchCollectionWindow(IWindow* pOwner, SearchCollection& collection);

	bool isOk() const noexcept { return m_isOk; }

private:
	LRESULT HandleMessage(HWND, UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	SearchCollection& m_searchCollection;

	Static stCategory, stPurity, stTag, stFilt, stTagInstruct[8];
	Edit edTag;
	Button btnOk, btnCancel, btnRes, btnAR, btnColor;
	CategoryComponent catCom;
	PurityComponent purCom;

	std::wstring tmpRes, tmpAR, tmpColor;
	bool m_isOk = false;
};
