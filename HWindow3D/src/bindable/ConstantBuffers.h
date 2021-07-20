#pragma once

#include "Bindable.h"

template <typename C>
class ConstantBuffer : public Bindable
{
public:
	void Update(Graphics& gfx, const C& consts)
	{
		INFOMAN(gfx);

		D3D11_MAPPED_SUBRESOURCE msr{};
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr));
		memcpy(msr.pData, &consts, sizeof(consts));
		CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->Unmap(pConstantBuffer.Get(), 0u));
	}

	ConstantBuffer(Graphics* gfx, const C& consts)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC consbufferDesc{};
		consbufferDesc.ByteWidth = sizeof(consts);
		consbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		consbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		consbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		consbufferDesc.MiscFlags = 0u;
		consbufferDesc.StructureByteStride = sizeof(ConstantBuffer);
		D3D11_SUBRESOURCE_DATA conssubres{};
		conssubres.pSysMem = &consts;
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&consbufferDesc, &conssubres, &pConstantBuffer));
	}


	ConstantBuffer(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC consbufferDesc{};
		consbufferDesc.ByteWidth = sizeof(C);
		consbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		consbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		consbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		consbufferDesc.MiscFlags = 0u;
		consbufferDesc.StructureByteStride = sizeof(ConstantBuffer);
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&consbufferDesc, nullptr, &pConstantBuffer));
	}
protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
};

template <typename C>
class VertexConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) override 
	{
		GetContext(gfx)->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};

template <typename C>
class PixelConstantBuffer : public ConstantBuffer<C>
{
	using ConstantBuffer<C>::pConstantBuffer;
	using Bindable::GetContext;
public:
	using ConstantBuffer<C>::ConstantBuffer;
	void Bind(Graphics& gfx) override
	{
		GetContext(gfx)->PSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	}
};