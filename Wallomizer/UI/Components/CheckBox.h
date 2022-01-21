#pragma once

#include "IHoverable.h"

class CheckBox : public IHoverable
{
public:
	CheckBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, HINSTANCE hInstance, bool isChecked = false, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~CheckBox();
	void click();
	void setChecked(bool state);
	bool isChecked();
	bool draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd);

private:
	bool checked;
};