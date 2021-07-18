#pragma once
#include "Win.h"
#include <stdint.h>
#include <optional>
#include <memory>

#include "utils/CarbonException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#define NOICE_NUMBER 69
#define EXCEPT(hr) WindowsWindow::WindowsWindowException(__LINE__, __FILE__, hr)
#define LAST_EXCEPT() WindowsWindow::WindowsWindowException(__LINE__, __FILE__, GetLastError())

// TODO: Don't close all instances of the window if we close one of them
class WindowsWindow
{
	// Internal Type declaration first
public:
	// TODO: Store the return code and return it at the end of the program
	class WindowsWindowException : public CarbonException
	{
		using CarbonException::CarbonException;
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
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		WindowClass() noexcept;
		~WindowClass();

		// Singleton instance
		static WindowClass wndClass;
		static constexpr const char* wndClassName = "Direct3D Renderer Window";
		HINSTANCE hInst;
	};
public:
    WindowsWindow(const char* title, uint32_t width, uint32_t height, int32_t x, int32_t y);
    ~WindowsWindow();
	NONCOPYABLE(WindowsWindow)

	void SetTitle(const std::string& title);
	static std::optional<int> ProcessMessages();
	inline Graphics& Gfx() {
		if (!gfx)
		{
			throw CBN_CHWND_NOGFX_EXCEPT();
		}
		return *gfx;
	}
public:
	Keyboard keyBoard;
	Mouse mouse;
private:
    int width;
    int height;
    int32_t xPos, yPos;
    HWND hWnd;
	std::unique_ptr<Graphics> gfx;
private:
    static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

