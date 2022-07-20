#include "Edit.h"

COLORREF Edit::fontColor =		RGB(0, 0, 0);
COLORREF Edit::bkColor =		RGB(200, 200, 200);
COLORREF Edit::bkInvalidColor = RGB(200, 50, 50);

Edit::Edit(IComponent* pParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExA(WS_EX_CLIENTEDGE, TEXT("Edit"), text, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | additionalStyles, x, y, width, height, m_pParent->hWnd(), NULL, NULL, NULL);
}

Edit::Edit(IComponent* pParent, LPCWSTR text, int x, int y, int width, int height, DWORD additionalStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, TEXT(L"Edit"), text, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | additionalStyles, x, y, width, height, m_pParent->hWnd(), NULL, NULL, NULL);
}

Edit::~Edit()
{
	DestroyWindow(m_hWnd);
}

void Edit::getTextA(char* buffer, int size)
{
	GetWindowTextA(m_hWnd, buffer, size);
}

void Edit::getTextW(wchar_t* buffer, int size)
{
	GetWindowTextW(m_hWnd, buffer, size);
}

void Edit::setTextA(LPCSTR text)
{
	SetWindowTextA(m_hWnd, text);
}

void Edit::setTextW(LPCWSTR text)
{
	SetWindowTextW(m_hWnd, text);
}

bool Edit::isEmpty()
{
	char buf[2] = { 0 };
	GetWindowTextA(m_hWnd, buf, 2);
	return !buf[0];
}