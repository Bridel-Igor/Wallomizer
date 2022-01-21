#include "Button.h"

Button::Button(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | additionalStyles, x, y, width, height, hParent, m_hMenu, NULL, NULL);
}

Button::~Button()
{
	DestroyWindow(m_hWnd);
}