#include "Edit.h"

Edit::Edit(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles)
{
	m_hWnd = CreateWindowExA(WS_EX_CLIENTEDGE, TEXT("Edit"), text, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | additionalStyles, x, y, width, height, hParent, NULL, NULL, NULL);
}

Edit::~Edit()
{
	DestroyWindow(m_hWnd);
}

void Edit::getTextA(char* buffer, int size)
{
	GetWindowTextA(m_hWnd, buffer, size);
}

void Edit::setTextA(LPCSTR text)
{
	SetWindowTextA(m_hWnd, text);
}

void Edit::setTextW(LPWSTR text)
{
	SetWindowTextW(m_hWnd, text);
}

bool Edit::isEmpty()
{
	char buf[10] = { 0 };
	GetWindowTextA(m_hWnd, buf, 10);
	return strlen(buf) ? false : true;
}