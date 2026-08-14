#include "Button.h"

Button::Button(IComponent* pParent, const std::string& text, int x, int y, int width, int height, DWORD additionalStyles, DWORD additionalExStyles) :
	IClickable(pParent)
{
	m_hWnd = CreateWindowExA(additionalExStyles, "Button", text.c_str(), WS_CHILD | WS_VISIBLE | additionalStyles, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);
}

Button::~Button() noexcept
{
	DestroyWindow(m_hWnd);
}
