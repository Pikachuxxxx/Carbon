#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(Graphics& gfx, std::vector<unsigned short>& indices)
	: count((UINT) indices.size())
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC idxbufferDesc{};
	idxbufferDesc.ByteWidth = UINT(count * sizeof(unsigned short));
	idxbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	idxbufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	idxbufferDesc.CPUAccessFlags = 0u;
	idxbufferDesc.MiscFlags = 0u;
	idxbufferDesc.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA idxsubres{};
	idxsubres.pSysMem = indices.data();
	CBN_GFX_THROW_INFO(GetDevice(gfx)->CreateBuffer(&idxbufferDesc, &idxsubres, &pIndexBuffer));
}

void IndexBuffer::Bind(Graphics& gfx)
{
	INFOMAN(gfx);
	CBN_GFX_THROW_INFO_ONLY(GetContext(gfx)->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u));
}
