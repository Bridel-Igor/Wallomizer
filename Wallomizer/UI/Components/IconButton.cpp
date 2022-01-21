#include "IconButton.h"

IconButton::IconButton(HWND hParent, int x, int y, int width, int height, HICON& _hIcon, HICON& _hIconHover, DWORD additionalStyles, DWORD additionalExStyles)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	hIcon = _hIcon;
	hIconHover = _hIconHover;
}

IconButton::~IconButton()
{
	DestroyWindow(m_hWnd);
}

bool IconButton::draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd, int x, int y)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);
	DrawIconEx(pDIS->hDC, x, y, m_hovering ? hIconHover : hIcon, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}