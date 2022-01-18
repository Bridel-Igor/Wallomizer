#pragma once

class CheckBox
{
public:
	CheckBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, HINSTANCE hInstance, bool isChecked = false, DWORD additionalStyles = 0, DWORD additionalExStyles = 0);
	~CheckBox();
	void click();
	void setChecked(bool state);
	bool isChecked();
	bool draw(LPDRAWITEMSTRUCT& pDIS, HBRUSH bkgrnd);
	void mouseHovering(bool isHovering);

	HWND hWnd = NULL;
	HMENU hMenu;

private:
	bool checked, hovering;
};