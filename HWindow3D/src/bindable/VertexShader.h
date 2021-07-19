#pragma once

#include "Bindable.h"

class VertexShader : public Bindable
{
	VertexShader(Graphics& gfx, wchar_t* path)
	{
		INFOMAN(gfx);

		CBN_GFX_THROW_INFO(D3DReadFileToBlob(path, &pBlob));
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	}

public:
	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	}
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};