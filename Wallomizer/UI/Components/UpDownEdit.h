#pragma once

#include "IComponent.h"

/// Class of generic "up down" with "edit" components. Derrives IComponent interface.
/// Component usually used for recieving user's choice of number in certain range.
/// Needs to be dynamically allocated in response to WinAPI WM_CREATE message.
/// Needs to be destroyed in response to WM_DESTROY message.
class UpDownEdit : public IComponent
{
public:
    /// Up down edit component construction.
    /// 
    /// @param hParent 
    /// @param x, y - coordinates, relative to parent window.
    /// @param width, height - size of button in pixels.
    /// @param minPos, maxPos - range of possible values.
    /// @param pos - initial value.
    UpDownEdit(HWND hParent, int x, int y, int width, int height, int minPos = 0, int maxPos = 100, int pos = 1);
    ~UpDownEdit();

    /// Sets current value of component.
    /// 
    /// @param Pos - desired value.
    void setPos(int pos);

    /// Gets current value of component.
    /// 
    /// @return Current value.
    int getPos();

    /// Gets ASCII representation of current value of component.
    /// 
    /// @return Pointer to char array, that holds value in ASCII form.
    char* getPosA();

    /// Get an ASCII text from edit component. You should provide char array to write it to.
    /// 
    /// @param buffer - pointer to char array
    /// @param size - size of the char array
    void getTextA(char* buffer, int size);

    /// Updates edit component according to value of ud down component.
    void update();

    /// Handle to edit component.
    HWND m_edithWnd;
    /// Flag that says if set value is valid. Needs to be set externally.
    bool m_invalid;

private:
    /// @brief temporary char buffer for some methods.
    char m_buffer[10];
};