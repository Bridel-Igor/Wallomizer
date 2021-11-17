#pragma once

#include <Windows.h>
#include <CommCtrl.h>

class UpDownEdit
{
public:
    UpDownEdit(HWND hParent, int x, int y, int width, int height, int minPos=0, int maxPos=100, int Pos=1)
    {
        edithWnd = CreateWindowEx(WS_EX_CLIENTEDGE, WC_EDIT, NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_BORDER | ES_NUMBER,
            x, y, width, height, hParent, NULL, NULL, NULL);

        hWnd = CreateWindowEx(WS_EX_LEFT | WS_EX_LTRREADING, UPDOWN_CLASS, NULL, WS_CHILDWINDOW | WS_VISIBLE | UDS_SETBUDDYINT | UDS_ALIGNRIGHT | UDS_ARROWKEYS | UDS_HOTTRACK,
            0, 0, 0, 0, hParent, NULL, NULL, NULL);

        SendMessageA(hWnd, UDM_SETBUDDY, (WPARAM)edithWnd, NULL);
        SendMessageA(hWnd, UDM_SETRANGE32, (WPARAM)minPos, (LPARAM)maxPos);
        setPos(Pos);
        invalid = false;
    }
    ~UpDownEdit()
    {
        DestroyWindow(hWnd);
        DestroyWindow(edithWnd);
    }
    void setPos(int Pos)
    {
        SendMessageA(hWnd, UDM_SETPOS32, NULL, (LPARAM)Pos);
    }
    int getPos()
    {
        return (int)SendMessageA(hWnd, UDM_GETPOS32, NULL, NULL);
    }
    char* getPosA()
    {
        _itoa_s(getPos(), buffer, 10);
        return buffer;
    }
    void getTextA(char* buffer, int size)
    {
        GetWindowTextA(edithWnd, buffer, size);
    }
    void update()
    {
        SetWindowTextA(edithWnd, getPosA());
    }
    HWND gethWnd()
    {
        return hWnd;
    }

    HWND hWnd;
    HWND edithWnd;
    bool invalid;

private:
    char buffer[10];
};