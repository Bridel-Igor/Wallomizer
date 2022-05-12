#include "Button.h"

Button::Button(IComponent* pParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles) :
	IClickable(pParent)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), text, WS_CHILD | WS_VISIBLE | additionalStyles, x, y, width, height, m_pParent->hWnd(), m_hMenu, NULL, NULL);
}

Button::~Button()
{
	DestroyWindow(m_hWnd);
}