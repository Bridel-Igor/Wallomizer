#include "UpDownEdit.h"

#include <CommCtrl.h>

UpDownEdit::UpDownEdit(IComponent* pParent, int x, int y, int width, int height, int minPos, int maxPos, int pos) :
    IComponent(pParent)
{
    m_edithWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        x, y, width, height, m_pParent->hWnd(), NULL, NULL, NULL);

    m_hWnd = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING, UPDOWN_CLASS, NULL, WS_CHILDWINDOW | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
        0, 0, 0, 0, m_pParent->hWnd(), NULL, NULL, NULL);

    SendMessageA(m_hWnd, UDM_SETBUDDY, (WPARAM)m_edithWnd, NULL);
    SendMessageA(m_hWnd, UDM_SETRANGE32, (WPARAM)minPos, (LPARAM)maxPos);
    setPos(pos);
    m_invalid = false;
}

UpDownEdit::~UpDownEdit()
{
    DestroyWindow(m_hWnd);
    DestroyWindow(m_edithWnd);
}

void UpDownEdit::setPos(int pos)
{
    SendMessageA(m_hWnd, UDM_SETPOS32, NULL, (LPARAM)pos);
}

int UpDownEdit::getPos()
{
    return (int)SendMessageA(m_hWnd, UDM_GETPOS32, NULL, NULL);
}

char* UpDownEdit::getPosA()
{
    _itoa_s(getPos(), m_buffer, 10);
    return m_buffer;
}

void UpDownEdit::getTextA(char* buffer, int size)
{
    GetWindowTextA(m_edithWnd, buffer, size);
}

void UpDownEdit::update()
{
    SetWindowTextA(m_edithWnd, getPosA());
}