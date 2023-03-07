#include "IconButton.h"

#include <CommCtrl.h>

IconButton::IconButton(IComponent* pParent, int x, int y, int width, int height, HICON& hIcon, HICON& hIconHovered, const char* toolTip, DWORD additionalStyles, DWORD additionalExStyles) :
	IHoverable(pParent)
{
	m_hWnd = CreateWindowExA(additionalExStyles, TEXT("Button"), TEXT(""), WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | additionalStyles, x, y, width, height, m_pParent->hWnd(), m_hMenu, NULL, NULL);
	m_hIcon = hIcon;
	m_hIconHovered = hIconHovered;

    m_hToolTip = nullptr;
    if (toolTip[0] != 0)
    {
        // Create a tooltip.
        m_hToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
            WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            m_hWnd, NULL, NULL, NULL);

        if (!m_hToolTip)
            return;

        SetWindowPos(m_hToolTip, HWND_TOPMOST, 0, 0, 0, 0,
            SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

        // Set up "tool" information. In this case, the "tool" is the entire parent window.
        TOOLINFO ti = { 0 };
        ti.cbSize = sizeof(TOOLINFO);
        ti.uFlags = TTF_SUBCLASS;
        ti.hwnd = m_hWnd;
        ti.hinst = NULL;
        char tooltip[32];
        strcpy_s(tooltip, toolTip);
        ti.lpszText = tooltip;

        GetClientRect(m_hWnd, &ti.rect);

        // Associate the tooltip with the "tool" window.
        SendMessage(m_hToolTip, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
    }
}

IconButton::~IconButton()
{
    if (m_hToolTip)
        DestroyWindow(m_hToolTip);
	DestroyWindow(m_hWnd);
}

bool IconButton::draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd, int x, int y)
{
	if (pDIS->hwndItem != m_hWnd)
		return false;
	FillRect(pDIS->hDC, &pDIS->rcItem, bkgrnd);
	DrawIconEx(pDIS->hDC, x, y, m_hovering ? m_hIconHovered : m_hIcon, 0, 0, 0, NULL, DI_NORMAL);
	return true;
}