#pragma once

#include "Win.h"
#include <d3d11.h>

class Graphics
{
public:
	Graphics(HWND hwnd);
	~Graphics();
	NONCOPYABLE(Graphics)
	void SetClearColor(float r, float g, float b);
	void OnFlip();
private:
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwapchain = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};

