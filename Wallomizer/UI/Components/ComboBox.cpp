#include "ComboBox.h"

ComboBox::ComboBox(IComponent* pParent, const std::wstring& text, int x, int y, int width, int height, DWORD additionalStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowW(L"COMBOBOX", text.c_str(), CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL | additionalStyles, x, y, width, height, parent()->hWnd(), nullptr, nullptr, nullptr);
}

ComboBox::~ComboBox()
{
	DestroyWindow(m_hWnd);
}

void ComboBox::addOption(const std::wstring& text)
{
	SendMessageW(m_hWnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(text.c_str()));
}

void ComboBox::clear()
{
	SendMessageW(m_hWnd, CB_RESETCONTENT, 0, 0);
}

void ComboBox::selectOption(int index) noexcept
{
	SendMessageW(m_hWnd, CB_SETCURSEL, index, 0);
}

int ComboBox::getSelectedOption() const noexcept
{
	return static_cast<int>(SendMessageW(m_hWnd, CB_GETCURSEL, 0, 0));
}

int ComboBox::findOption(const std::wstring& text) const noexcept
{
	return static_cast<int>(SendMessageW(m_hWnd, CB_FINDSTRINGEXACT, static_cast<WPARAM>(-1), reinterpret_cast<LPARAM>(text.c_str())));
}
