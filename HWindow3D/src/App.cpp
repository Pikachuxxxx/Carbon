#include "App.h"

#include <sstream>
#include <memory>
#include <random>

#include "drawable/DrawableBase.h"

App::App(const std::string& appName, int width, int height, int x, int y)
    : wnd(appName.c_str(), width, height, x, y)
{
    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> adist(0.0f, 3.1415f *2.0f);
	std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 2.0f);
	std::uniform_real_distribution<float> rdist(6.0f, 20.0f);

    for (auto i = 0; i < 40; i++)
    {
        boxes.push_back(std::make_unique<Box>(wnd.Gfx(), rng, adist, ddist, odist, rdist));
    }
    wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH(1.0f, width / height, 0.01f, 100.0f));
}

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
    const float dt = timer.Mark();
    std::ostringstream oss;
    oss << "Delta time : " << std::to_string(dt);
    wnd.SetTitle(oss.str());

    const float c = sin(timer.Peek()) / 2.0f + 0.5f;
    wnd.Gfx().SetClearColor(1.0f, c, c);

//#define HELLO_TRIANGLE
#ifdef HELLO_TRIANGLE
	wnd.Gfx().DrawHelloD3D11Triangle();
	// Draw the test triangle
	wnd.Gfx().DrawHelloD3D11Triangle(wnd.mouse.GetPosX() / 400.0f - 1.0f, -wnd.mouse.GetPosY() / 300.0f + 1.0f);
#endif // !NDEBUG

    for (auto& b : boxes)
    {
        b->Update(dt);
        b->Draw(wnd.Gfx());
    }

    // SwapBuffers
    wnd.Gfx().OnFlip();
}
