#pragma once

#include "Bindable.h"
#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

class VertexShader : public Bindable
{
public:
	VertexShader(Graphics& gfx, LPCWSTR path)
	{
		INFOMAN(gfx);

		CBN_GFX_THROW_INFO(D3DReadFileToBlob(path, &pBlob));
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader));
	}

	void Bind(Graphics& gfx) override
	{
		INFOMAN(gfx);
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->VSSetShader(pVertexShader.Get(), nullptr, 0u));
	}

	ID3DBlob* GetByteCode() { return pBlob.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
};