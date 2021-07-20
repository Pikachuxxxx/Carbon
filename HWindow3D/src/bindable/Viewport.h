#pragma once

#include "Bindable.h"

class Viewport : public Bindable
{
public:
	Viewport(Graphics& gfx, int width, int height)
	{
		viewportDescription = {};
		viewportDescription.Width = width;
		viewportDescription.Height = height;
		viewportDescription.MaxDepth = 1;
		viewportDescription.MinDepth = 0;
		viewportDescription.TopLeftX = 0;
		viewportDescription.TopLeftY = 0;
	}

	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->RSSetViewports(1, &viewportDescription));
	}

private:
	D3D11_VIEWPORT viewportDescription = {};
};