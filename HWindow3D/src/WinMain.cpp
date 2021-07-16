#include "utils/WindowsMessageMap.h"
#include "WindowsWindow.h"

/**
 * The Windows subsystem Main method - WinMain
 * @brief Unlike a console app, the WinMain build the minimum application without attaching the o/p and i/p streams and other overheads etc
 * @param hInstance The Handle to the current window instance
 * @param hPrevInstance The handle to the previous instance, Not Needed!
 * @param lpCmdLine Same as the argc in normal main, Gets the entire command line arguments as a single large string
 * @param nShowCmd Tells how to show the window, basically simple styling. Can also be done while registering the window class
 */
int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    try {
        // Create the Win32 Window Instance
        WindowsWindow wnd("LOLOLOLOLOLLLL!!!", 800, 600, 400, 200);

        MSG msg;
        BOOL gResult;
        while ((gResult = GetMessage(&msg, nullptr, 0, 0)) > 0)
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (wnd.keyBoard.KeyIsPressed(VK_SPACE))
                MessageBox(nullptr, "Fuck you!", "Space was pressed!", MB_OK);
        }

        if (gResult == -1)
            return -1;
        else
            return int(msg.wParam);
    }
    catch (const Exception& e)
    {
        MessageBox(nullptr, e.what(), e.GetType(), MB_OK | MB_ICONEXCLAMATION);
    }
    catch (const std::exception& e)
    {
        MessageBox(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    catch (...)
    {
        MessageBox(nullptr, "None", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
    return  -1;
}