#include "utils/WindowsMessageMap.h"
#include "WindowsWindow.h"

/**
 * The Windows subsystem WinMain method
 * @brief Unlike a console app, the WinMain build the minimum application without attaching the o/p and i/p streams and other overheads etc
 * @param hInstance The Handle to the current window instance
 * @param hPrevInstance The handle to the previous instance, Not Needed!
 * @param lpCmdLine Same as the argc in normal main, Gets the entire command line arguments as a single large string
 * @param nShowCmd Tells how to show the window, basically simple styling, can be done while registering the window class
 */
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	WindowsWindow wnd("LOLOLOLOLOLLLL!!!", 800, 600, 400, 200);

	MSG msg;
	BOOL gResult;
	while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (gResult == -1)
		return -1;
	else
		return msg.wParam;
}