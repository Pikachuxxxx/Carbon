#include "WindowsWindow.h"

// Window Class Construction for the static singleton variable
WindowsWindow::WindowClass WindowsWindow::WindowClass::wndClass;

WindowsWindow::WindowsWindow(const char* title, uint32_t width, uint32_t height, int32_t x, int32_t y) : width(width), height(height), xPos(x), yPos(y)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	// Create Window Instance & get hWnd
	hWnd = CreateWindow(
		WindowClass::GetClassName(), title,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		xPos, yPos, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInstance(), this
	);
	// newly created windows start off as hidden
	ShowWindow(hWnd, SW_SHOW);
}

WindowsWindow::~WindowsWindow()
{
	DestroyWindow(hWnd);
}

LRESULT CALLBACK WindowsWindow::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	if (msg == WM_NCCREATE)
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		WindowsWindow* const pWnd = static_cast<WindowsWindow*>(pCreate->lpCreateParams);
		// set WinAPI-managed user data to store ptr to window instance
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WindowsWindow::HandleMsgThunk));
		// forward message to window instance handler
		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK WindowsWindow::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	// retrieve ptr to window instance
	WindowsWindow* const pWnd = reinterpret_cast<WindowsWindow*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	// forward message to window instance handler
	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}

LRESULT WindowsWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(NOICE_NUMBER);
		break;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/*************************************************************************
 *						WindowsWindow::Window Class                      *
*************************************************************************/

WindowsWindow::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr))
{
	// Register the Window Class using a configuration struct
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(windowClass);
	windowClass.style = CS_OWNDC;				// Creates multiple windows with it's own device context (CS = class style, OWN DC =  own device context)
	windowClass.lpfnWndProc = HandleMsgSetup;	// The window procedure to handle the messages
	windowClass.cbClsExtra = 0;					// No extra used defines data for the custom data for current window class registered
	windowClass.cbWndExtra = 0;					// Same as above but it's the data for every instance of this class 
	windowClass.hInstance = GetInstance();		// The instance handler for the current window 
	windowClass.hIcon = nullptr;				// No Icon 
	windowClass.hCursor = nullptr;				// Using the default cursor
	windowClass.hbrBackground = nullptr;		// We take care of this through DirectX, We don't specify how to clear the backgorund we leave it empty to be taken care by DirectX
	windowClass.lpszMenuName = nullptr;			// Not using any menus
	windowClass.lpszClassName = GetClassName();		// The name to identify this class handle
	RegisterClassEx(&windowClass);
}

WindowsWindow::WindowClass::~WindowClass()
{
	UnregisterClass(wndClassName, GetInstance());
}

const char* WindowsWindow::WindowClass::GetClassName() noexcept
{
	return wndClassName;
}

HINSTANCE WindowsWindow::WindowClass::GetInstance() noexcept
{
	return wndClass.hInst;
}