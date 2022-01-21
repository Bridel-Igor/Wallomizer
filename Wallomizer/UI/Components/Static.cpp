#include "Static.h"

Static::Static(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles)
{
	m_hWnd = CreateWindowExA(WS_EX_TRANSPARENT | additionalExStyles, TEXT("Static"), text, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | additionalStyles, x, y, width, height, hParent, NULL, NULL, NULL);
}

Static::~Static()
{
	DestroyWindow(m_hWnd);
}