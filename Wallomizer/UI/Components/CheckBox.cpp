#include <Windows.h>

#include "CheckBox.h"
#include "WindowStyles.h"
#include "resource.h"

HICON CheckBox::Resources::hICheckBox, CheckBox::Resources::hICheckBoxChecked, 
	CheckBox::Resources::hICheckBoxHover, CheckBox::Resources::hICheckBoxCheckedHover;
unsigned char CheckBox::Resources::refCount = 0;

CheckBox::Resources::Resources()
{
	if (refCount++) // Loading icons only if this is the first player creating
		return;
	hICheckBox =				(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX),					IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxChecked =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_CHECKED),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxHover =			(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_HOVER),			IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);
	hICheckBoxCheckedHover =	(HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_CHECKBOX_CHECKED_HOVER),	IMAGE_ICON, 0, 0, LR_LOADTRANSPARENT);

}

CheckBox::Resources::~Resources()
{
	if (--refCount) // Destroying icons only if this is the last player destroying
		return;
	DestroyIcon(hICheckBoxCheckedHover);
	DestroyIcon(hICheckBoxHover);
	DestroyIcon(hICheckBoxChecked);
	DestroyIcon(hICheckBox);
}

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

bool CheckBox::isChecked() const
{
	return m_checked;
}

bool CheckBox::draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);

	DrawIconEx(pDIS->hDC, (pDIS->rcItem.right - 20) / 2, (pDIS->rcItem.bottom - 20) / 2,
		m_checked ? (m_hovering ? resources.hICheckBoxCheckedHover : resources.hICheckBoxChecked) : (m_hovering ? resources.hICheckBoxHover : resources.hICheckBox)
		, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}