#include "Edit.h"

Edit::Edit(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExA(WS_EX_CLIENTEDGE, "Edit", text.c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | additionalStyles, x, y, width, height, parent()->hWnd(), nullptr, nullptr, nullptr);
}

Edit::Edit(IComponent* pParent, const std::wstring& text, int x, int y, int width, int height, DWORD additionalStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExW(WS_EX_CLIENTEDGE, L"Edit", text.c_str(), WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | additionalStyles, x, y, width, height, parent()->hWnd(), nullptr, nullptr, nullptr);
}

Edit::~Edit()
{
	DestroyWindow(m_hWnd);
}
