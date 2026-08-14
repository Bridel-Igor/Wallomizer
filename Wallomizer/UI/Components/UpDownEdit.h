#pragma once

#include "IComponent.h"

/// Generic numeric input component combining an edit control with an up-down control. 
/// Derrives IComponent interface.
/// Component is used for receiving a numeric value within a specified range.
class UpDownEdit : public IComponent
{
public:
    /// Constructs an UpDown edit component.
    /// 
    /// @param pParent - pointer to the parent component
    /// @param x, y - coordinates, relative to the parent window.
    /// @param width, height - size of the component in pixels.
    /// @param minPos - minimum allowed value.
    /// @param maxPos - maximum allowed value.
    /// @param pos - initial value.
    UpDownEdit(IComponent* pParent, int x, int y, int width, int height, int minPos = 0, int maxPos = 100, int pos = 1);
    
    ~UpDownEdit();

    /// Sets current value of the component.
    /// 
    /// @param Pos - desired value.
    void setPos(int pos) noexcept;

    /// @return Current value of the up-down control.
    int getPos() const noexcept;

    /// @return True if the edit value is within the allowed range, false otherwise.
    bool isEditValid() const;

    /// Synchronizes the edit control with the current value of the up-down control.
    void syncEdit() noexcept;

    /// Handle to edit control.
    HWND m_editHWnd = nullptr;

private:
    /// @return Numeric value currently entered in the edit control.
    int getEditValue() const;

    int m_minPos, m_maxPos;
};
