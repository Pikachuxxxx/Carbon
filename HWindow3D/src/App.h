#pragma once

#include "Win.h"
#include "WindowsWindow.h"
#include "Timer.h"

class App
{
public:
	App(const std::string& appName, int whidth, int height, int x, int y);
	int Run();
private:
	void OnFrame();
private:
	WindowsWindow wnd;
	Timer timer;
		
};

