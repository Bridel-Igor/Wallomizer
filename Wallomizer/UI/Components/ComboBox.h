#pragma once

#include "IComponent.h"

class ComboBox : public IComponent
{
public:
	ComboBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, int nOfItems = 0, int lenOfItems = 0, CHAR* items = nullptr, int selectedItem = 0, DWORD additionalStyles = 0);
	~ComboBox();
	int getSelectedItem();
	void getTextA(char* buffer, int size);
	void setTextA(LPCSTR text);
	bool isEmpty();
};

/*
Example:
CHAR list[2][10] = {TEXT("item1"), TEXT("item2")};
cbComBox = new ComboBox(Window(), "", x, y, w, h, 2, 10, &TimeUnitsList[0][0]);
*/