#include "App.h"

#include <sstream>

App::App(const std::string& appName, int width, int height, int x, int y)
	: wnd(appName.c_str(), width, height, x, y)
{}

int App::Run()
{
	while (true)
	{
		if (const auto ecode = WindowsWindow::ProcessMessages())
			return *ecode;
		OnFrame();
	}
}

void App::OnFrame()
{
	const float t = timer.Peek();
	std::ostringstream oss;
	oss << "Time elapsed : " << std::to_string(t);
	wnd.SetTitle(oss.str());

	const float c = sin(timer.Peek()) / 2.0f + 0.5f;
	wnd.Gfx().SetClearColor(c, 1.0f, c);

	// SwapBuffers
	wnd.Gfx().OnFlip();
}
