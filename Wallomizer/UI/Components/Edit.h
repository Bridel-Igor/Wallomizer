#pragma once

#include "IComponent.h"

class Edit : public IComponent
{
public:
	Edit(HWND hParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0);
	~Edit();
	void getTextA(char* buffer, int size);
	void setTextW(LPWSTR text);
	void setTextA(LPCSTR text);
	bool isEmpty();
};