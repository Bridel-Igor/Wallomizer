#include "CheckBox.h"

#include <Windows.h>

#include "GraphicsUtils.h"
#include "resource.h"

CheckBox::Icons::Icons()
{
	checkBox =				GraphicsUtils::loadIcon(IDI_CHECKBOX);
	checkBoxChecked =		GraphicsUtils::loadIcon(IDI_CHECKBOX_CHECKED);
	checkBoxHover =			GraphicsUtils::loadIcon(IDI_CHECKBOX_HOVER);
	checkBoxCheckedHover =	GraphicsUtils::loadIcon(IDI_CHECKBOX_CHECKED_HOVER);
}

CheckBox::Icons::~Icons()
{
	DestroyIcon(checkBoxCheckedHover);
	DestroyIcon(checkBoxHover);
	DestroyIcon(checkBoxChecked);
	DestroyIcon(checkBox);
}

CheckBox::CheckBox(IComponent* pParent, int x, int y, int width, int height, bool isChecked, DWORD additionalStyles, DWORD additionalExStyles) :
	IHoverable(pParent),
	m_icons(m_resources.get()),
	m_checked(isChecked)
{
	m_hWnd = CreateWindowExA(additionalExStyles, "Button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
}

CheckBox::~CheckBox()
{
	DestroyWindow(m_hWnd);
}

void CheckBox::setChecked(bool state) noexcept
{
	m_checked = state;
	InvalidateRect(m_hWnd, nullptr, FALSE);
}

bool CheckBox::draw(LPDRAWITEMSTRUCT drawItem, HBRUSH bkgrnd)
{
	if (drawItem->hwndItem != m_hWnd)
		return false;
	FillRect(drawItem->hDC, &drawItem->rcItem, bkgrnd);

	DrawIconEx(drawItem->hDC, (drawItem->rcItem.right - 20) / 2, (drawItem->rcItem.bottom - 20) / 2,
		m_checked ? 
		(m_hovering ? m_icons.checkBoxCheckedHover : m_icons.checkBoxChecked) :
		(m_hovering ? m_icons.checkBoxHover : m_icons.checkBox)
		, 0, 0, 0, nullptr, DI_NORMAL);
	return true;
}
