#include "Static.h"

Static::Static(IComponent* pParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExA(WS_EX_TRANSPARENT | additionalExStyles, TEXT("Static"), text, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | additionalStyles, x, y, width, height, m_pParent->hWnd(), NULL, NULL, NULL);
}

Static::Static(IComponent* pParent, LPCWSTR text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExW(WS_EX_TRANSPARENT | additionalExStyles, TEXT(L"Static"), text, WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | additionalStyles, x, y, width, height, m_pParent->hWnd(), NULL, NULL, NULL);
}

Static::~Static()
{
	DestroyWindow(m_hWnd);
}