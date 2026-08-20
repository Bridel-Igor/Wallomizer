#include "IconButton.h"

#include <CommCtrl.h>

#include "IWindow.h"

IconButton::IconButton(IComponent* pParent, int x, int y, int width, int height, 
    HICON hIcon, HICON hIconHovered, HICON hIconDisabled, HICON hIconToggled,
    std::string_view toolTip, DWORD additionalStyles, DWORD additionalExStyles) :
    IHoverable(pParent),
    m_hIcon(hIcon),
    m_hIconHovered(hIconHovered),
    m_hIconDisabled(hIconDisabled),
    m_hIconToggled(hIconToggled),
    m_tooltip(toolTip)
{
	m_hWnd = CreateWindowExA(additionalExStyles, "Button", "", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, parent()->hWnd(), hMenu(), nullptr, nullptr);

    if (!toolTip.empty())
    {
        // Create a tooltip.
        m_hTooltip = CreateWindowExA(WS_EX_TOPMOST, TOOLTIPS_CLASS, nullptr,
            WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            m_hWnd, nullptr, nullptr, nullptr);

        if (!m_hTooltip)
            return;

        // Set up tooltip information for the entire button.
        TOOLINFOA ti{};
        ti.cbSize = sizeof(ti);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = m_hWnd;
        ti.hinst = nullptr;
        ti.lpszText = const_cast<char*>(m_tooltip.data());

        GetClientRect(m_hWnd, &ti.rect);

        // Associate the tooltip with the "tool" window.
        SendMessageA(m_hTooltip, TTM_ADDTOOLA, 0, reinterpret_cast<LPARAM>(&ti));
    }
}

IconButton::~IconButton()
{
    if (m_hTooltip)
        DestroyWindow(m_hTooltip);
	DestroyWindow(m_hWnd);
}

bool IconButton::draw(LPDRAWITEMSTRUCT pDIS, bool enabled, bool toggled, HBRUSH bkgrnd, int x, int y) const
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd ? bkgrnd : IWindow::Resources::mainBkBrush);

    HICON hIcon = m_hIcon;
    if (toggled && m_hIconToggled)
        hIcon = m_hIconToggled;
    else if (!enabled && m_hIconDisabled)
        hIcon = m_hIconDisabled;
    else if (m_hovering && m_hIconHovered)
        hIcon = m_hIconHovered;

	DrawIconEx(pDIS->hDC, x, y, hIcon, 0, 0, 0, nullptr, DI_NORMAL);
	return true;
}
