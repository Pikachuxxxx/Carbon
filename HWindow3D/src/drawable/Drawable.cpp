#include "Drawable.h"
#include "bindable/IndexBuffer.h"

void Drawable::Draw(Graphics& gfx) const
{
	for (auto& b : binds)
	{
		b->Bind(gfx);
	}
	for (auto& b : GetStaticBinds())
	{
		b->Bind(gfx);
	}
	gfx.DrawIndexed(pIndexBuffer->GetCount());
}

void Drawable::AddBind(std::unique_ptr<Bindable> bind)
{
	binds.push_back(std::move(bind));
}

void Drawable::AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf)
{
	pIndexBuffer = ibuf.get();
	binds.push_back(std::move(ibuf));
}
