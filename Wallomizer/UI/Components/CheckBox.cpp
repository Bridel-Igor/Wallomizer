#include <Windows.h>

#include "CheckBox.h"
#include "WindowStyles.h"

CheckBox::CheckBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, HINSTANCE hInstance, bool isChecked, DWORD additionalStyles, DWORD additionalExStyles)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, m_hMenu, hInstance, NULL);
	checked = isChecked;
}

CheckBox::~CheckBox()
{
	DestroyWindow(m_hWnd);
}

void CheckBox::click()
{
	setChecked(!checked);
}

void CheckBox::setChecked(bool state)
{
	checked = state;
	InvalidateRect(m_hWnd, NULL, FALSE);
}

bool CheckBox::isChecked()
{
	return checked;
}

bool CheckBox::draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);

	DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
		checked ? (m_hovering ? WindowStyles::hICheckBoxCheckedHover : WindowStyles::hICheckBoxChecked) : (m_hovering ? WindowStyles::hICheckBoxHover : WindowStyles::hICheckBox)
		, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}