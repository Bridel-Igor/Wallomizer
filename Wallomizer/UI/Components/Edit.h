#pragma once

#include "IComponent.h"

/// Class of generic "edit" component. Derrives IComponent interface.
/// Component usually used for recieving text input from user.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
class Edit : public IComponent
{
public:
	/// Edit component construction.
	/// 
	/// @param pParent - pointer to parent component
	/// @param text - text that will fill edit initially.
	/// @param x, y - coordinates, relative to parent window.
	/// @param width, height - size of button in pixels.
	/// @param additionalStyles - WinAPI style flags that will be added to WS_CHILD, WS_VISIBLE and ES_AUTOHSCROLL flags. Can be ignored.
	/// @param additionalExStyles - WinAPI extended style flags. Can be ignored.
	Edit(IComponent* pParent, LPCSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0);
	Edit(IComponent* pParent, LPCWSTR text, int x, int y, int width, int height, DWORD additionalStyles = 0);
	~Edit();

	/// Get an ASCII text from edit component. You should provide char array to write it to.
	/// 
	/// @param buffer - pointer to char array
	/// @param size - size of the char array
	void getTextA(char* buffer, int size);

	/// Get an wide text from edit component. You should provide wchar_t array to write it to.
	/// 
	/// @param buffer - pointer to wchar_t array
	/// @param size - size of the wchar_t array
	void getTextW(wchar_t* buffer, int size);

	/// Set ASCII text to edit component.
	/// 
	/// @param text - pointer to char buffer.
	void setTextA(LPCSTR text);
	
	/// Set wide text to edit component.
	/// 
	/// @param text - pointer to wchar_t buffer.
	void setTextW(LPCWSTR text);

	/// Check for emptiness of an edit component.
	/// 
	/// @return True if edit is empty, false otherwise.
	bool isEmpty();

	static COLORREF fontColor,	bkColor, bkInvalidColor;
};