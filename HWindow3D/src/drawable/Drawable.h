#pragma once

#include "Graphics.h"
#include <DirectXMath.h>
#include <memory>

//class Bindable;
#include "bindable/Bindable.h"

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX GetTransformXM() const = 0;
	virtual void Update(float dt) = 0;
	void Draw(Graphics& gfx) const;
	void AddBind(std::unique_ptr<Bindable> bind);
	void AddIndexBuffer(std::unique_ptr<class IndexBuffer> ibuf);
private:
	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const = 0;
private:
	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;
};