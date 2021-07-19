#pragma once
#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
	PixelShader(Graphics& gfx, wchar_t* path)
	{
		INFOMAN(gfx);

		CBN_GFX_THROW_INFO(D3DReadFileToBlob(path, &pBlob));
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

public:
	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};