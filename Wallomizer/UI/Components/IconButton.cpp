#include "IconButton.h"

IconButton::IconButton(HWND hParent, int x, int y, int width, int height, HICON& hIcon, HICON& hIconHovered, DWORD additionalStyles, DWORD additionalExStyles)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, hParent, m_hMenu, NULL, NULL);
	m_hIcon = hIcon;
	m_hIconHovered = hIconHovered;
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
	DrawIconEx(pDIS->hDC, x, y, m_hovering ? m_hIconHovered : m_hIcon, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}