#pragma once

#include "IClickable.h"

class Button : public IClickable
{
public:
	Button(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~Button();
};