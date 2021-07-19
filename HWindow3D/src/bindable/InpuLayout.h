#pragma once

#include "Bindable.h"

class InputLayout : public Bindable
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC> ieds)
	{
		//CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateInputLayout(ieds.data(), (UINT)(sizeof(D3D11_INPUT_ELEMENT_DESC) * ieds.size()), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	}

	void Bind(Graphics& gfx) override
	{
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetInputLayout(pInputLayout.Get()));
	}
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};