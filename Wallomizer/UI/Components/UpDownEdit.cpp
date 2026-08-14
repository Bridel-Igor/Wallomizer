#include "UpDownEdit.h"

#include <CommCtrl.h>

UpDownEdit::UpDownEdit(IComponent* pParent, int x, int y, int width, int height, int minPos, int maxPos, int pos) :
    IComponent(pParent),
    m_minPos(minPos), 
    m_maxPos(maxPos)
{
    m_editHWnd = CreateWindowExA(WS_EX_CLIENTEDGE, WC_EDIT, nullptr, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        x, y, width, height, parent()->hWnd(), nullptr, nullptr, nullptr);

    m_hWnd = CreateWindowExA(WS_EX_LEFT | WS_EX_LTRREADING, UPDOWN_CLASS, nullptr, WS_CHILDWINDOW | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
        0, 0, 0, 0, parent()->hWnd(), nullptr, nullptr, nullptr);

    SendMessageA(m_hWnd, UDM_SETBUDDY, reinterpret_cast<WPARAM>(m_editHWnd), 0);
    SendMessageA(m_hWnd, UDM_SETRANGE32, static_cast<WPARAM>(minPos), static_cast<LPARAM>(maxPos));
    setPos(pos);
}

UpDownEdit::~UpDownEdit()
{
    DestroyWindow(m_hWnd);
    DestroyWindow(m_editHWnd);
}

void UpDownEdit::setPos(int pos) noexcept
{
    SendMessageA(m_hWnd, UDM_SETPOS32, 0, static_cast<LPARAM>(pos));
}

int UpDownEdit::getPos() const noexcept
{
    return static_cast<int>(SendMessageA(m_hWnd, UDM_GETPOS32, 0, 0));
}

bool UpDownEdit::isEditValid() const
{
    const int value = getEditValue();
    return value >= m_minPos && value <= m_maxPos;
}

void UpDownEdit::syncEdit() noexcept
{
    SetWindowTextW(m_editHWnd, std::to_wstring(getPos()).c_str());
}

int UpDownEdit::getEditValue() const
{
    const int length = GetWindowTextLengthA(m_editHWnd);
    if (length == 0)
        return 0;

    std::string text(length, L'\0');
    GetWindowTextA(m_editHWnd, text.data(), length + 1);

    return std::stol(text.c_str(), nullptr, 10);
}
