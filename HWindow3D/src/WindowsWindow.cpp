#include "WindowsWindow.h"

#include "utils/WindowsMessageMap.h"
#include "../resource.h"
#include <sstream>

// Window Class Construction for the static singleton variable
WindowsWindow::WindowClass WindowsWindow::WindowClass::wndClass;

WindowsWindow::WindowsWindow(const char* title, uint32_t width, uint32_t height, int32_t x, int32_t y) : width(width), height(height), xPos(x), yPos(y)
{
    // calculate window size based on desired client region size
    RECT wr{};
    wr.left = 100;
    wr.right = width + wr.left;
    wr.top = 100;
    wr.bottom = height + wr.top;
    if (FAILED(AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE)))
    {
        throw LAST_EXCEPT();
    }
    // Create Window Instance & get hWnd
    hWnd = CreateWindow(
        WindowClass::GetName(), title,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        xPos, yPos, wr.right - wr.left, wr.bottom - wr.top,
        nullptr, nullptr, WindowClass::GetInstance(), this
    );
    if (hWnd == nullptr)
    {
        throw LAST_EXCEPT();
    }
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

/* Message callback Handler */
LRESULT WindowsWindow::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
    // Maps the message map ID to map it to a string
    static WindowsMessageMap mm;
    //OutputDebugString(mm(msg, wParam, lParam).c_str());

    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(NOICE_NUMBER);
        break;
    // Clear keystate when the focus is lost to prevent zombie keys
    case WM_KILLFOCUS:
        keyBoard.ClearState();
        break;
    case WM_KEYDOWN:
        if (!(lParam & 0x40000000) || keyBoard.AutoRepeadIsEnabled())
        {
            keyBoard.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
		break;
    case WM_KEYUP:
        keyBoard.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    case WM_CHAR:
        keyBoard.OnChar(static_cast<unsigned char>(wParam));
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
    windowClass.style = CS_OWNDC;				    // Creates multiple windows with it's own device context (CS = class style, OWN DC =  own device context)
    windowClass.lpfnWndProc = HandleMsgSetup;	    // The window procedure to handle the messages
    windowClass.cbClsExtra = 0;					    // No extra used defines data for the custom data for current window class registered
    windowClass.cbWndExtra = 0;					    // Same as above but it's the data for every instance of this class 
    windowClass.hInstance = GetInstance();		    // The instance handler for the current window 
    windowClass.hIcon = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0));				    // Icon 
    windowClass.hCursor = nullptr;				    // Using the default cursor
    windowClass.hbrBackground = nullptr;		    // We take care of this through DirectX, We don't specify how to clear the background we leave it empty to be taken care by DirectX
    windowClass.lpszMenuName = nullptr;			    // Not using any menus
    windowClass.lpszClassName = GetName();		    // The name to identify this class handle
    windowClass.hIconSm = static_cast<HICON>(LoadImage(hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0));;
    RegisterClassEx(&windowClass);
}

WindowsWindow::WindowClass::~WindowClass()
{
    UnregisterClass(wndClassName, GetInstance());
}

const char* WindowsWindow::WindowClass::GetName() noexcept
{
    return wndClassName;
}

HINSTANCE WindowsWindow::WindowClass::GetInstance() noexcept
{
    return wndClass.hInst;
}

/************************************************************************
 *				    WindowsWindow::WindowsWindowException               *
*************************************************************************/
WindowsWindow::WindowsWindowException::WindowsWindowException(int line, const char* file, HRESULT hr) noexcept
    : Exception(line, file), m_HR(hr)
{}

const char* WindowsWindow::WindowsWindowException::what() const noexcept
{
    std::ostringstream oss;
    oss << GetType() << std::endl
        << "[Error Code] " << GetErrorCode() << std::endl
        << "[Description] " << GetErrorString() << std::endl
        << GetOriginString();
    whatBufffer = oss.str();
    return whatBufffer.c_str();
}

const char* WindowsWindow::WindowsWindowException::GetType() const noexcept
{
    return "Windows Window Exception";
}

std::string WindowsWindow::WindowsWindowException::TranslateErrorCode(HRESULT hr)
{
    char* pMsgBuf = nullptr;
    DWORD nMsgLen = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr);
    if (nMsgLen == 0)
        return "Unidentified Error code!";
    std::string errorString = pMsgBuf;
    LocalFree(pMsgBuf);
    return errorString;
}

HRESULT WindowsWindow::WindowsWindowException::GetErrorCode() const noexcept
{
    return  m_HR;
}

std::string WindowsWindow::WindowsWindowException::GetErrorString() const noexcept
{
    return TranslateErrorCode(m_HR);
}