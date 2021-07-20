#pragma once

#include "ConstantBuffers.h"
#include "drawable/Drawable.h"
#include <DirectXMath.h>

class TransformCBuffer : public Bindable
{
public:
	TransformCBuffer(Graphics& gfx, const Drawable& parent);

	void Bind(Graphics& gfx) override;
private:
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;

};