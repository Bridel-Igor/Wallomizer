#include "UpDownEdit.h"

#include <CommCtrl.h>

UpDownEdit::UpDownEdit(HWND hParent, int x, int y, int width, int height, int minPos, int maxPos, int Pos)
{
    edithWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_NUMBER,
        x, y, width, height, hParent, NULL, NULL, NULL);

    m_hWnd = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING, UPDOWN_CLASS, NULL, WS_CHILDWINDOW | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
        0, 0, 0, 0, hParent, NULL, NULL, NULL);

    SendMessageA(m_hWnd, UDM_SETBUDDY, (WPARAM)edithWnd, NULL);
    SendMessageA(m_hWnd, UDM_SETRANGE32, (WPARAM)minPos, (LPARAM)maxPos);
    setPos(Pos);
    invalid = false;
}

UpDownEdit::~UpDownEdit()
{
    DestroyWindow(m_hWnd);
    DestroyWindow(edithWnd);
}

void UpDownEdit::setPos(int Pos)
{
    SendMessageA(m_hWnd, UDM_SETPOS32, NULL, (LPARAM)Pos);
}

int UpDownEdit::getPos()
{
    return (int)SendMessageA(m_hWnd, UDM_GETPOS32, NULL, NULL);
}

char* UpDownEdit::getPosA()
{
    _itoa_s(getPos(), buffer, 10);
    return buffer;
}

void UpDownEdit::getTextA(char* buffer, int size)
{
    GetWindowTextA(edithWnd, buffer, size);
}

void UpDownEdit::update()
{
    SetWindowTextA(edithWnd, getPosA());
}