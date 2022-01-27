#include <Windows.h>

#include "CheckBox.h"
#include "WindowStyles.h"

CheckBox::CheckBox(HWND hParent, int x, int y, int width, int height, bool isChecked, DWORD additionalStyles, DWORD additionalExStyles)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	m_checked = isChecked;
}

CheckBox::~CheckBox()
{
	DestroyWindow(m_hWnd);
}

void CheckBox::click()
{
	setChecked(!m_checked);
}

void CheckBox::setChecked(bool state)
{
	m_checked = state;
	InvalidateRect(m_hWnd, NULL, FALSE);
}

bool CheckBox::isChecked()
{
	return m_checked;
}

bool CheckBox::draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);

	DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
		m_checked ? (m_hovering ? WindowStyles::hICheckBoxCheckedHover : WindowStyles::hICheckBoxChecked) : (m_hovering ? WindowStyles::hICheckBoxHover : WindowStyles::hICheckBox)
		, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}