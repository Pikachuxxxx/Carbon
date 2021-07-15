#pragma once
#include "Win.h"
#include <stdint.h>

#define NOICE_NUMBER 69

class WindowsWindow
{
public:
	WindowsWindow(const char* title, uint32_t width, uint32_t height, int32_t x, int32_t y);
	~WindowsWindow();
private:
	int width;
	int height;
	int32_t xPos, yPos;
	HWND hWnd;
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

	static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
	LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
};

