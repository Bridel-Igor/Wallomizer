#pragma once

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <cstdint>
#include <string>
#include <vector>
#include <Windows.h>

#include "IComponent.h"

class IHoverable;

/// Generic window component.
/// Manages shared window resources and dispatches WinAPI messages to derived classes.
class IWindow : public IComponent
{
public:
	/// Manages shared resources used by application windows.
	class Resources
	{
	public:
		Resources() noexcept;
		~Resources() noexcept;

		static HFONT mainFont;
		static HFONT titleFont;
		static HBRUSH mainBkBrush;

	private:
		static std::uint16_t refCount;
	}resources;

public:
	/// Constructs a window with the specified class, style and geometry.
	///
	/// @param sWindowName - text displayed in the window title.
	/// @param className - unique window class name.
	/// @param dwStyle - window style flags.
	/// @param dwExStyle - extended window style flags.
	/// @param x, y - initial window position.
	/// @param nWidth, nHeight - initial window size.
	/// @param pParent - parent component, or nullptr for a top-level window.
	/// 
	/// @throws std::runtime_error If window class registration or window creation fails.
	IWindow(LPCSTR sWindowName, std::string className, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT, 
		IComponent* pParent = nullptr);

	~IWindow() noexcept;

	/// Runs the window message loop until WM_QUIT is received.
	virtual void windowLoop();

	/// Restores and brings the window to the foreground.
	void focus();

	/// Posts a request for the window to close.
	void requestClose();

	/// Centers the window relative to the specified parent window.
	void centerWindow(HWND hParent) noexcept;

	/// Indicates that the window message loop has started processing messages.
	bool isReady() const noexcept { return m_isReady; }

	/// Registers a component that should receive mouse-hover notifications.
	void registerHoverable(IHoverable* pHoverable);

	/// Removes a previously registered hoverable component.
	void unregisterHoverable(IHoverable* pHoverable);

protected:
	/// Handles messages forwarded from the window procedure.
	/// Return RESULT_DEFAULT to let IWindow continue processing the message.
	virtual LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	/// Sets the specified font for a child window.
	static BOOL CALLBACK SetChildFont(HWND hChild, LPARAM lParam);

	/// Indicates that a message was not handled by the component.
	static constexpr LRESULT RESULT_DEFAULT = -1;

private:
	/// Native Win32 window procedure.
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::vector<IHoverable*> m_hoverables;
	std::string m_name;
	bool m_isReady = false;
};
