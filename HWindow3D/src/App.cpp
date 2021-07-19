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
    const float t = timer.Mark();
    std::ostringstream oss;
    oss << "Time elapsed : " << std::to_string(t);
    wnd.SetTitle(oss.str());

    const float c = sin(timer.Peek()) / 2.0f + 0.5f;
    wnd.Gfx().SetClearColor(1.0f, c, c);

	wnd.Gfx().DrawHelloD3D11Triangle();

    // Draw the test triangle
    wnd.Gfx().DrawHelloD3D11Triangle(wnd.mouse.GetPosX() / 400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);

    // SwapBuffers
    wnd.Gfx().OnFlip();
}
