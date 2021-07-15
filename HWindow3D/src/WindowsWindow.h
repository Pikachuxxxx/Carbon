#pragma once
#include "Win.h"
#include <stdint.h>

#include "utils/Exception.h"

#define NOICE_NUMBER 69
#define WND_EXCEPT(hr) WindowsWindow::WindowsWindowException(__LINE__, __FILE__, hr)

// TODO: Don't close all instances of the window if we close one of them
class WindowsWindow
{
public:
	// TODO: Store the return code and return it at the end of the program
	class WindowsWindowException : public Exception
	{
	public:
		WindowsWindowException(int line, const char* file, HRESULT hr) noexcept;
		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	private:
		HRESULT m_HR;
	};
private:
	// singleton manages registration/cleanup of window class
	class WindowClass
	{
	public:
		static const char* GetClassName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		static constexpr const char* wndClassName = "Direct3D Renderer Window";
		static WindowClass wndClass;
		HINSTANCE hInst;
	};
public:
    WindowsWindow(const char* title, uint32_t width, uint32_t height, int32_t x, int32_t y);
    ~WindowsWindow();
private:
    int width;
    int height;
    int32_t xPos, yPos;
    HWND hWnd;
	NONCOPYABLE(WindowsWindow)
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

