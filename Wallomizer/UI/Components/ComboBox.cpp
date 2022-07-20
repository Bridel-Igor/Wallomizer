#include "ComboBox.h"

ComboBox::ComboBox(IComponent* pParent, LPCSTR text, int x, int y, int width, int height, int nOfItems, int lenOfItems, CHAR* items, int selectedItem, DWORD additionalStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowA("COMBOBOX", text, CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VSCROLL | additionalStyles, x, y, width, height, m_pParent->hWnd(), NULL, NULL, NULL);

	for (int i = 0; i < nOfItems; i++)
	{
		SendMessage(m_hWnd, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)(CHAR*)items);
		items += lenOfItems;
	}

	SendMessage(m_hWnd, CB_SETCURSEL, (WPARAM)selectedItem, (LPARAM)0);
}

ComboBox::~ComboBox()
{
	DestroyWindow(m_hWnd);
}

int ComboBox::getSelectedItem()
{
	return (int)SendMessage(m_hWnd, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
}

void ComboBox::getTextA(char* buffer, int size)
{
	GetWindowTextA(m_hWnd, buffer, size);
}

void ComboBox::setTextA(LPCSTR text)
{
	SetWindowTextA(m_hWnd, text);
}

bool ComboBox::isEmpty()
{
	char buf[2] = { 0 };
	GetWindowTextA(m_hWnd, buf, 2);
	return !buf[0];
}