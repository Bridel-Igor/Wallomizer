#pragma once

#include "IComponent.h"

/// Class of generic "combo box" component. Derrives IComponent interface.
/// Component usually used for chosing for recieving users choice between multiple items.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
/// In order to fill combo box with items you should provide it with the two-dimensional char array.
/// To process change events you should in reaction to WM_COMMAND message compare lValue with m_hWnd, and HIWORD of wParam with EN_CHANGE
/// To process dropping down events you should in reaction to WM_COMMAND message compare lValue with m_hWnd, and HIWORD of wParam with CBN_DROPDOWN
class ComboBox : public IComponent
{
public:
	/// Combo box component construction.
	/// 
	/// @param hParent - HWND of parent window.
	/// @param text - text that will be displayed.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param nOfItems, lenOfItems - size of two dimensional char array that holds items for check box. Can be ignored.
	/// @param items - pointer to first element of two dimensional char array that holds items for check box. Can be ignored.
	/// @param selectedItem - number of item to be selected after combo box creation. Can be ignored.
	/// @param additionalStyles - WinAPI style flags that will be added to CBS_DROPDOWNLIST, CBS_HASSTRINGS, WS_CHILD, WS_OVERLAPPED, WS_VISIBLE and WS_VSCROLL flags. Can be ignored.
	ComboBox(HWND hParent, LPCSTR text, int x, int y, int width, int height, int nOfItems = 0, int lenOfItems = 0, CHAR* items = nullptr, int selectedItem = 0, DWORD additionalStyles = 0);
	~ComboBox();

	/// Get number of selected item.
	/// 
	/// @return Number of selected item.
	int getSelectedItem();

	/// Get an ASCII text from combo box component's caption. You should provide char array to write it to.
	/// 
	/// @param buffer - pointer to char array
	/// @param size - size of the char array
	void getTextA(char* buffer, int size);

	/// Set ASCII text to combo box component's caption.
	/// 
	/// @param text - pointer to char buffer.
	void setTextA(LPCSTR text);

	/// Check for emptiness of an combo box component's caption.
	/// 
	/// @return True if combo box's caption is empty, false otherwise.
	bool isEmpty();
};

///Example:
///CHAR list[2][10] = {TEXT("item1"), TEXT("item2")};
/// cbComBox = new ComboBox(Window(), "", x, y, w, h, 2, 10, &list[0][0]);