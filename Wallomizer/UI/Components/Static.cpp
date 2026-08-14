#include "Static.h"

Static::Static(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExA(additionalExStyles, "Static", text.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | additionalStyles, x, y, width, height, parent()->hWnd(), nullptr, nullptr, nullptr);
}

Static::Static(IComponent* pParent, const std::wstring& text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles) :
	IComponent(pParent)
{
	m_hWnd = CreateWindowExW(additionalExStyles, L"Static", text.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE | additionalStyles, x, y, width, height, parent()->hWnd(), nullptr, nullptr, nullptr);
}

Static::~Static()
{
	DestroyWindow(m_hWnd);
}
