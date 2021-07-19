#pragma once

#include "Bindable.h"

class VertexBuffer : public Bindable
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		: stride(sizeof(V))
	{
		INFOMAN(gfx);

		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = (UINT)(sizeof(V) * vertices.size());
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0u;
		bufferDesc.MiscFlags = 0u;
		bufferDesc.StructureByteStride = sizeof(V);
		D3D11_SUBRESOURCE_DATA subres{};
		subres.pSysMem = vertices.data();
		CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&bufferDesc, &subres, &pVertexBuffer));
	}

	void Bind(Graphics& gfx) override;

private:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
};

