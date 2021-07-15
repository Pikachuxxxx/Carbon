#pragma once

#include <Windows.h>
#include <unordered_map>

class WindowsMessageMap
{
public:
    WindowsMessageMap() noexcept;
    std::string operator()(DWORD msg, LPARAM lp, WPARAM wp) const noexcept;
private:
    std::unordered_map<DWORD, std::string> map;
};

/**
 * WM - Windows message
 * NC - Non-Client message WM_NCXXXXX
 * Client region is inside the window
 * Non-Client region is the status bar and close buttons etc.
 * lParam - key press message, information about message
 * wParam - which key is pressed, the actual details of the message
 */