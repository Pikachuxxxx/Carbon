#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(Graphics& gfx, const Drawable& parent)
	: vcbuf(gfx), parent(parent)
{}

void TransformCBuffer::Bind(Graphics& gfx)
{
	vcbuf.Update(gfx, DirectX::XMMatrixTranspose(parent.GetTransformXM() * gfx.GetProjection()));
	vcbuf.Bind(gfx);
}
