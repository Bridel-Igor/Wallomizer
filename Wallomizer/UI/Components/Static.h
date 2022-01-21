#pragma once

#include "IComponent.h"

class Static : public IComponent
{
public:
	Static(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~Static();
};