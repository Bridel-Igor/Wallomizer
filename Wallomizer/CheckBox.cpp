#include <Windows.h>

#include "CheckBox.h"
#include "HMenuGenerator.h"
#include "WindowStyles.h"

CheckBox::CheckBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, HINSTANCE hInstance, bool isChecked, DWORD additionalStyles, DWORD additionalExStyles)
{
	hMenu = HMenuGenerator::getNewHMenu();
	hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, hMenu, hInstance, NULL);
	checked = isChecked;
	hovering = false;
}

CheckBox::~CheckBox()
{
	DestroyWindow(hWnd);
	HMenuGenerator::releaseHMenu(hMenu);
}

void CheckBox::click()
{
	setChecked(!checked);
}

void CheckBox::setChecked(bool state)
{
	checked = state;
	InvalidateRect(hWnd, NULL, FALSE);
}

bool CheckBox::isChecked()
{
	return checked;
}

bool CheckBox::draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd)
{
	if (pDIS->hwndItem != hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);

	DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
		checked ? (hovering ? WindowStyles::hICheckBoxCheckedHover : WindowStyles::hICheckBoxChecked) : (hovering ? WindowStyles::hICheckBoxHover : WindowStyles::hICheckBox)
		, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}

void CheckBox::mouseHovering(bool isHovering)
{
	if (hovering != isHovering)
	{
		hovering = isHovering;
		InvalidateRect(hWnd, NULL, FALSE);
	}
}