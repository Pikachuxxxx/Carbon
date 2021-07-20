#pragma once

#include "Bindable.h"

class PixelShader : public Bindable
{
public:
	PixelShader(Graphics& gfx, LPCWSTR path)
	{
		INFOMAN(gfx);

		CBN_GFX_THROW_INFO(D3DReadFileToBlob(path, &pBlob));
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader));
	}

	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->PSSetShader(pPixelShader.Get(), nullptr, 0u));
	}
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};