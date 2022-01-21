#pragma once

#include "IComponent.h"

class UpDownEdit : public IComponent
{
public:
    UpDownEdit(HWND hParent, int x, int y, int width, int height, int minPos = 0, int maxPos = 100, int Pos = 1);
    ~UpDownEdit();
    void setPos(int Pos);
    int getPos();
    char* getPosA();
    void getTextA(char* buffer, int size);
    void update();

    HWND edithWnd;
    bool invalid;

private:
    char buffer[10];
};